#include <unistd.h>
#include "child.h"
#include "../service/route.h"
#include "../service/bus.h"
#include "../service/service.h"
#include "../charge/charge.h"
#include "../charge/parade.h"
#include "../utils/hours.h"
#include "../utils/utils.h"
#include "../../frontend/main.h"
#define MSGSIZE 1024
#define BUS_BUFFER_SIZE 50

/*
Rutina de despacho de cargas de usuarios 
en la cola de la parada
*/
void* paradeTracker( void* arguments )
{
    printf("Despachador de parada Iniciado\n");
    // Inicialzacion
    struct threadBusArg* args = (struct threadBusArg*) arguments;
    Parade* parade = args->parade;
    List* charges =  args->parade->charges;
   
    /*
    Se obtiene apuntador al reloj de la ruta
    que modificara el hilo del reloj
    */
    char clock[6];
    strcpy(clock, args->timeRange[0]);
    while ( strcmp( clock, args->timeRange[1] ) <= 0 && charges->next )
    {   
        char arrivalTime[6] ={0};

        sprintf(arrivalTime, "%02d:00", charges->content->charge->parateArrivalTime );
        if ( strcmp(clock, arrivalTime) == 0 )
        {
            parade->waitingCharges = listPush( parade->waitingCharges, charges->content );
            parade->numWaiting += charges->content->charge->numUsers;
            charges = charges->next;
            
        }

        sem_wait(args->semWrite);
        sprintf(  parade->buffer, "%d", parade->numWaiting );
        sem_post(args->semRead);
        incressHour2(clock, clock);
    }
    pthread_exit(NULL);
}

/*
Se llena el buffer segun el estatus y el porcentaje de progreso
*/
void fillBuffer(char* buffer, int bufferSize, int percentage, int status)
{
    char sign = status == -1 ? '>' : '-';
    sign = status == 1 ? '<' : sign;

    if (status == -1 && percentage < 100)
    {
        strcpy(buffer, " [----------] ");
        for (int i = 2; i < percentage/10+2; i++)
        {
            buffer[i] = sign;
        }
    }
    else if( status == 1 && percentage < 100 )
    {
        strcpy(buffer, " [----------] ");
        for (int i = 0; i < percentage/10+1; i++)
        {
            buffer[11-i] = sign;
        }
    }
    else if( status == 0 ){
        strcpy(buffer, " [--Waiting-] ");
    }
    else if( status == 2 )
    {
        strcpy(buffer, " [--Finish-] ");
    }
    
}

/*
Se escribe en el buffer del autobus el progreso del viaje
durante uno de los trayectos.
El trayecto y la direccion se decide segun el estatus del autobus
*/
void reportTravelProgress( Bus* bus, Parade* parade, char* clock, char* travelTime, char* leaveTime,
                         char* arrivalTime, char* returnTime, char* finishTime )
{
    /*
    Se obtiene el tiempo de inicio y finalizacion del viaje
    */
    char* startTime = clock;
    char* endTime = clock;
    if (bus->status == -1)
    {
        startTime = leaveTime;
        endTime = arrivalTime;
    }
    else if( bus->status == 1 )
    {
        startTime = returnTime;
        endTime = finishTime;
    }
    
    /*
    Se obtiene porcentaje progreso del viaje 
    para luego llenar el buffer que leera el hilo principal
    */
    int percentage = 0;
    percentage = getProgressPercentage( startTime, endTime, travelTime, clock);
    fillBuffer(bus->buffer, BUS_BUFFER_SIZE, percentage, bus->status);
}


/*
Funcion de Ejecucion de hilo que hace el monitoreo y
reporte del estatus de cada autobus
*/
void* busTracker( void * arguments )
{
    struct threadBusArg* args = (struct threadBusArg*) arguments;
    Bus* bus = args->bus;
    char clock[6];
    strcpy(clock, args->timeRange[0]);
    printf("Iniciado hilo de autobus\n");
    printf("%s %s %s %s %s %s\n", clock, args->leaveTime, args->travelTime, args->arrivalTime, args->returnTime, args->finishTime);

    while ( strcmp( clock, args->timeRange[1] ) <= 0 )
    {
        if ( strcmp(clock, args->leaveTime) == 0 )
        {
            setBusAsGoing(bus);
            *args->numBusesOnService++;
            
        }
        else if (strcmp(clock, args->returnTime) == 0)
        {
            setBusAsReturning(bus);
        }
        else if (strcmp(clock, args->arrivalTime) == 0)
        {
            setBusAsWaiting(bus);
        }
        else if (strcmp(clock, args->finishTime) == 0)
        {
            setBusAsFinished(bus);
            *args->numBusesOnService--;
        }

        /*
        Si el autobus se encuentra esperando se suben 
        las cargas al autobus si es que existe alguna
        */
        if (bus->status == 0) 
        {
            //fillBusWithChargesFromParade( args->parade, bus );
        }
        
        sem_wait(args->semWrite);
        reportTravelProgress(bus, args->parade, clock,
            args->travelTime, args->leaveTime, args->arrivalTime,
            args->returnTime, args->finishTime);

        sem_post(args->semRead);
        incressHour2(clock, clock);
    }
    pthread_exit(NULL);
}


/*
Se inicializan los semaforos de lectura y de escritura
para cada hilo
*/
void initSemaphores( struct semaphores* sem, int numBuses )
{
    sem_init( &sem->paradeRead, 0, 0 );
    sem_init( &sem->paradeWrite, 0, 1 );
    sem->busesRead = (sem_t*) malloc( sizeof(sem_t)*numBuses );
    sem->busesWrite = (sem_t*) malloc( sizeof(sem_t)*numBuses );
    for (int i = 0; i < numBuses; i++)
    {
        sem_init( &sem->busesRead[i], 0, 0 );
        sem_init( &sem->busesWrite[i], 0, 1 );
    }

}

/*
Se esperan todos los semaforos relacionados con la lectura
*/
void waitReadSemaphores( struct semaphores* sem, int numBuses)
{
    sem_wait(&sem->paradeRead);
    for (int i = 0; i < numBuses; i++)
    {
        sem_wait(&sem->busesRead[i]);
    }
}

/*
Se liberan los semaforos relacionados con la escritura
*/
void postWriteSemaphores( struct semaphores* sem, int numBuses)
{
    sem_post(&sem->paradeWrite);
    for (int i = 0; i < numBuses; i++)
    {
        sem_post(&sem->busesWrite[i]);
    }
}

/*
Creador de los hilos de ejecucion.
Se creara un hilo por cada autobus de la ruta,
se creara un autobus para incorporar cargas de usuarios
y un hilo reloj para llevar la hora en la parada
a la cola de espera en la parada
*/
void initThreads( pthread_t* threads, Route* route, char* timeRange[2], struct semaphores* sem )
{
    
    // Se inicializan los semaforos y los hilos
    int numBuses = route->service->buses->lenght;
    initSemaphores( sem, numBuses );

    // Inicializamos los hilos de Autobuses
    List* bus = route->service->buses;
    int i = 0;
    while( bus->next )
    {
        struct threadBusArg* argument = (struct threadBusArg*) malloc(sizeof(struct threadBusArg));
        argument->timeRange = timeRange;
        argument->bus = bus->content->bus;
        argument->parade = route->parade;
        argument->travelTime = route->travelTime;
        argument->leaveTime = bus->content->bus->leaveTime;
        argument->semRead = &sem->busesRead[i];
        argument->semWrite = &sem->busesWrite[i];
        hourSum( argument->arrivalTime, argument->leaveTime, argument->travelTime ); //Hora de llegada a la parada
        hourSum( argument->returnTime, argument->arrivalTime, "00:10" ); //Hora de retorno desde la parada
        hourSum( argument->finishTime, argument->returnTime, argument->travelTime ); //Hora de llegada a la universidad
        pthread_create( &threads[i], NULL, busTracker, (void*) argument );
        bus = bus->next;
        i++;
    }

    // Inicializamos hilos de Parada
    struct threadBusArg* argumentParade = (struct threadBusArg*) malloc(sizeof(struct threadBusArg));
    argumentParade->parade = route->parade;
    argumentParade->timeRange = timeRange;
    argumentParade->semRead = &sem->paradeRead;
    argumentParade->semWrite = &sem->paradeWrite;
    pthread_create( &threads[numBuses], NULL, paradeTracker, (void*) argumentParade );
}
/*
Se esperan los hilos que finalizan
*/
void joinThreads(pthread_t* threads, int numBuses)
{
    for (int i = 0; i < numBuses+1; i++)
    {
        pthread_join( threads[i], NULL );
    }
}


/* Proceso de Monitoreo de Autobuses*/
void monitorBusesDeamon( Route* route, char* timeRange[2], struct semaphores* sem )
{
    int flag = 1;
    int fd = route->pip[1];
    int numBuses = route->service->buses->lenght;
    int numReadyBuses = 0;
    int maxMin = hoursDifference(timeRange[1], timeRange[0]);
    do
    {
        /*
        Cabecera de identificacion de la ruta
        */
        char identifierString[BUS_BUFFER_SIZE] = {0};
        sprintf(identifierString, "%s: ", route->cod);

        /*
        Se prepara en buffer lo que se escribira en el pipe
        */
        char buffer[MSGSIZE] = {0};
        strcat( buffer, identifierString );

        /*
        Esperar a los escritores de los hilos
        */
        waitReadSemaphores(sem, numBuses);

        /*
        Se escribe la cantidad de personas esperando en la
        parada
        */
        strcat( buffer, route->parade->buffer );

        /*
        Se recorren los buffers de los autobuses y se guarda el
        resultado recibido por cada uno de ellos
        */
        List* bus = route->service->buses;
        while (bus->next)
        {
            strcat( buffer, bus->content->bus->buffer );
            bus = bus->next;
        }

        postWriteSemaphores( sem, numBuses );

        // Se escribe el buffer en el pipe
        strcat( buffer, "\n" );
        flag = write(fd, buffer, MSGSIZE);
        maxMin--;
    }
    while ( flag != -1 && maxMin > 0);
}

/*
Este es la el punto de inicio de los procesos hijos
que se encargan de administrar las rutas que se les fue asignada por el 
proceso padre
*/
void childProcessStart( Route* route, char* timeRange[2] )
{
    /*
    Declaracion de arreglo de hilos,
    numBuses hilos de autobuses mas un hilo para la parada
    los semaforos e iniciamos el monitoreo de los hilos, al teminar
    se recogen los hilos
    */
   printf("Inicio de Proceso Hijo\n");
    struct semaphores sem;
    int numBuses = route->service->buses->lenght;
    pthread_t threads[numBuses+1];
    initThreads( threads, route, timeRange, &sem );
    monitorBusesDeamon( route, timeRange, &sem );
    joinThreads( threads, numBuses );
    printf("Finalizo Proceso Hijo\n");
}