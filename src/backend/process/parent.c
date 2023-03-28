#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "parent.h"
#include "child.h"
#include "../utils/hours.h"
#include "../../frontend/main.h"
#define MSGSIZE 1024

/*
Se crean subprocesos y a cada uno se le asigna una ruta
Cada proceso se encarga de administrar su ruta
*/
void createChildren( List* routeElementList, char* timeRange[2], float timeFactor )
{
    while ( routeElementList->next )
    {
        if(fork() == 0)
        {
            // Se cierra el descriptor de lectura en los procesos hijo
            close(routeElementList->content->route->pip[0]);
            childProcessStart( routeElementList->content->route, timeRange );
            close(routeElementList->content->route->pip[1]);
            exit(0);
        }
        // Para el proceso padre se cierra el descriptor de escritura
        close(routeElementList->content->route->pip[1]);
        routeElementList = routeElementList->next;
    }
}

/*
Esperar a todos los hijos del proceso padre
*/
void waitChildren( List* routeElementList )
{
    while(wait(NULL) > 0);
}

/*
Se leen los pipes de todas las rutas. 
Cada Subproceso encargado de administrar una
ruta debe generar un reporte cada minuto
*/
char* getRoutesStatus( List* routesList )
{
    int i = 0;
    char* prtStr = (char* ) malloc( sizeof(char) );
    while ( routesList->next )
    {
        i++;
        prtStr = realloc( prtStr, sizeof(char)*MSGSIZE*i );
        int fd = routesList->content->route->pip[0];
        char buffer[MSGSIZE] = {0};
        int flag;
        /* Si en el pipe no hay mensaje, esperar hasta que llegue el mensaje 
        y volver a intentarlo */
        while( (flag = read( fd, buffer, MSGSIZE )) == 0 );
        strcat( prtStr, buffer );
        routesList = routesList->next;
    }
    return prtStr;    
}


/*
Monitor del estado de los procesos hijos
con ciclo de ejecucion cada timeFactor segundos
*/
void monitorRoutesDeamon( List* routesList, float timeFactor, char* timeRange[2] )
{
    int minutes = 0;
    int hour, min, maxHour, maxMin;
    sscanf( timeRange[0], "%d:%d", &hour, &min );
    sscanf( timeRange[1], "%d:%d", &maxHour, &maxMin );
    while( minutes < 300 )
    {
        //system("clear");
        printHeader();
        printf("%02d:%02d\n", hour, min);
        char* prtString = getRoutesStatus(routesList);
        printf("%s\n", prtString);
        usleep(timeFactor*1000000);
        incressHour( &hour, &min );
    }
}

/*
Punto de Arranque del proceso padre
El proceso padre sera el mismo proceso llamador, este
genera tantos procesos hijos como rutas existan y 
se encarga de reportar los mensajes de los procesos hijos
por la salida estandar con el formato indicado
y los imprimira en pantalla dependiendo de los mensajes 
de los hijos
*/
void parentProcessStart( List* routesList, float timeFactor, char* timeRange[2] )
{
    printf("Iniciando proceso padre...\n");
    sleep(2);
    createChildren( routesList, timeRange, timeFactor);
    monitorRoutesDeamon(routesList, timeFactor, timeRange);
    // waitChildren( routesList );
}
