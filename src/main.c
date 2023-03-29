#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "backend/charge/charge.h"
#include "backend/charge/parade.h"
#include "backend/service/bus.h"
#include "backend/service/service.h"
#include "backend/service/route.h"
#include "backend/utils/list.h"
#include "backend/utils/hours.h"
#include "backend/process/parent.h"

/*
Se configuran y validan las opciones enviadas por el usuario, y se almacena la configuracion
recibida en las variables conrrespondientes siguientes:

- serviceFile: Archivo de caracterizacion del servicio
- loadFile: Archivo de caracterizacion de las cargas de la parada
- timeFactor: Corresponde a la duracion en segundos de un minuto en la simulacion
*/
int getOptions( int argc, char *argv[], char** serviceFile, char** loadFile, float* timeFactor )
{
    int sFlag = 0;
    int opt; // Opcion seleccionada por el usuario
   // Bucle que recorre las opciones especificadas al llamar al programa
    while ((opt = getopt(argc, argv, "s:c:t:")) != -1) {
        switch (opt) {
            case 's':
                sFlag = 1;
                // Si la cadena no termina en ".txt", agregarlo
                if (strstr(optarg, ".txt") == NULL) {
                    *serviceFile = malloc(strlen(optarg) + 5);
                    sprintf(*serviceFile, "%s.txt", optarg);
                } else {
                    *serviceFile = optarg;
                }
                break;
            case 'c':
                // Si la cadena no termina en ".csv", agregarlo
                if (strstr(optarg, ".csv") == NULL) {
                    *loadFile = malloc(strlen(optarg) + 5);
                    sprintf(*loadFile, "%s.csv", optarg);
                } else {
                    *loadFile = optarg;
                }
                break;
            case 't':
                *timeFactor = atof(optarg);
                break;
            default:
                // Si se especificó una opción incorrecta, mostrar mensaje de uso y salir del programa
                fprintf(stderr, "Sintaxis: %s -s <archivo> [-c <archivo>] [-t <num>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verificar que se especificó el archivo de servicio
    if (!sFlag) {
        fprintf(stderr, "Debe especificar el archivo de caracterizacion del servicio con -s\n");
        exit(EXIT_FAILURE);
    }
    
    // Abrir archivo de carga y verificar si se pudo abrir correctamente
    FILE *cfile = fopen(*loadFile, "r");
    if (cfile == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo %s\n", *loadFile);
        exit(EXIT_FAILURE);
    }
    fclose(cfile);

    // Abrir archivo de servicio y verificar si se pudo abrir correctamente
    FILE *sfile = fopen(*serviceFile, "r");
    if (sfile == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo %s\n", *serviceFile);
        exit(EXIT_FAILURE);
    }
    fclose(sfile);

    // Mostrar información de los archivos y el factor de tiempo
    printf("##### Opciones Recibidas #####\n");
    printf("Archivo de servicio: %s\n", *serviceFile);
    printf("Archivo de carga: %s\n", *loadFile);
    printf("Factor de tiempo: %.2f seg/min\n\n", *timeFactor);

    return 1;
}

/*
Se obtiene la menor hora y la mayor hora para
conocer el punto de inicio y final estimado de la
simulacion
*/
char** getTimeRange( List* routesList )
{
    char** timeRange = (char**) malloc( sizeof(char*)*2 );
    int n = routesList->lenght;
    char* minimHour = (char*) malloc( sizeof(char)*6 );
    char* maxHour = (char*) malloc( sizeof(char)*6 );
    strcpy(minimHour, "99:99");
    strcpy(maxHour, "00:00");
    List* routes = routesList;
    while ( routes->next )
    {
        List* bus = routes->content->route->service->buses;
        while (bus->next)
        {
            char leaveTime[6];
            char travelTime[6];
            char arrivalTime[6];
            char returnTime[6];
            char finishTime[6];
            strcpy(leaveTime, bus->content->bus->leaveTime);
            strcpy(travelTime, routes->content->route->travelTime);
            hourSum( arrivalTime, leaveTime, travelTime ); //Hora de llegada a la parada
            hourSum( returnTime, arrivalTime, "00:11" ); //Hora de retorno desde la parada mas un minuto para terminar de cerrar el hilo
            hourSum( finishTime, returnTime, travelTime ); //Hora de llegada a la universidad
            if (strcmp(leaveTime, minimHour) < 0)
            {
                strcpy(minimHour, leaveTime);
            }
            if (strcmp(finishTime, maxHour) > 0)
            {
                strcpy(maxHour, finishTime);
            }
            bus = bus->next;
        }
        
        routes = routes->next;
    }

    routes = routesList;
    while ( routes->next )
    {
        List* charge = routes->content->route->parade->charges;
        while (charge->next)
        {
            char hour[6] = {0};
            sprintf(hour, "%02d:00", charge->content->charge->parateArrivalTime);
            if (strcmp(hour, minimHour) < 0)
            {
                strcpy(minimHour, hour);
            }
            if (strcmp(hour, maxHour) > 0)
            {
                strcpy(maxHour, hour);
            }
            charge = charge->next;
        }
        
        routes = routes->next;
    }
    timeRange[0] = minimHour;
    timeRange[1] = maxHour;
    return timeRange;
}

void printfFinalReport( List* routes )
{
    List* route = routes;
    int numUsersInTime = 0;
    int numUsersOutTime = 0;
    int numUsersFail = 0;
    while (route->next)
    {
        // Usuarios en la universidad (que abordaron algun bus)
        List* bus = route->content->route->service->buses;
        while (bus->next)
        {
            List* charge = bus->content->bus->charges;
            printf("Bus de %s en %p, libre %d\n", bus->content->bus->leaveTime, bus->content->bus, bus->content->bus->status);
            char leaveTime[6];
            char travelTime[6];
            char arrivalTime[6];
            char returnTime[6];
            char finishTime[6];
            strcpy(leaveTime, bus->content->bus->leaveTime);
            strcpy(travelTime, routes->content->route->travelTime);
            hourSum( arrivalTime, leaveTime, travelTime ); //Hora de llegada a la parada
            hourSum( returnTime, arrivalTime, "00:10" ); //Hora de retorno desde la parada mas un minuto para terminar de cerrar el hilo
            hourSum( finishTime, returnTime, travelTime ); //Hora de llegada a la universidad

            while (charge->next)
            {
                printf("entro\n");
                char expected[6];
                char paradeArrivalTime[6];
                sprintf(paradeArrivalTime, "%02d:00", charge->content->charge->parateArrivalTime);
                hourSum( expected, paradeArrivalTime, "01:30" ); // Se espera llegar 1:30 despues de llegar a la parada
                printf("Expected: %s\n", expected);

                if (strcmp( expected, finishTime ) <= 0)
                {
                    numUsersInTime += charge->content->charge->numUsers;
                }
                else
                {
                    numUsersOutTime += charge->content->charge->numUsers;
                }
                
                charge = charge->next;
            }
            
            bus = bus->next;
        }
        // Usuarios en la parada (que no abordaron ningun bus)

        Parade* parade = route->content->route->parade;
        List* charge = parade->waitingCharges;
        while (charge->next)
        {
            numUsersFail += charge->content->charge->numUsers;
            charge = charge->next;
        }
        
        route = route->next;
    }
    /*
    Se Muestran los resultados obtenidos
    */
    printf("###### RESULTADOS DE LA SIMULACION ######\n");
    printf("Numero de personas que llegaron a tiempo a la Universidad: %d\n", numUsersInTime);
    printf("Numero de personas que llegaron tarde a la Universidad: %d\n", numUsersOutTime);
    printf("Numero de personas que No llegaron a la Universidad: %d\n", numUsersFail);
    
}

int main(int argc, char *argv[])
{
    // Declaración de variables y valores por defecto
    char* serviceFile = NULL; // Archivo de caracterización del servicio
    char* loadFile = "carga.csv"; // Archivo de carga
    float timeFactor = 0.25; // Factor de tiempo
    int maxMinutes = 10; // Minutos de duracion de la simulacion

    // Se obtienen las opciones introducidas por el usuario y en caso de error 
    // se imprime el error y se finaliza la ejecucion
    getOptions( argc, argv, &serviceFile, &loadFile, &timeFactor );

    /*
    Se Crea una lista de Rutas.
    para esto se leen simultaneamente los archivos de servicio y de carga y se van
    generando las estucturas Routes que iran dentro de la lista
    */
    List routes = *createRoutes(serviceFile, loadFile);

    /*
    Se obtiene el rango de tiempo en el que se ejecutara la simulacion
    para esto se recorren las rutas y se obtiene la hora minima y la hora maxima
    de las cargas y de los autobuses
    */
    char** timeRange = getTimeRange(&routes);

    /*
    Se inicia la ejecucion del proceso padre quien creara a los 
    procesos hijos, y ejecutara la simulacion
    */
    parentProcessStart(&routes, timeFactor, timeRange);

    /*
    Se imprime el Reporte Final Una ver el proceso termine, y espere a sus hijos
    */
   printfFinalReport(&routes);

    return 0;
}
