#if !defined(CHILD_P)
#define CHILD_P
#include <pthread.h>
#include <semaphore.h>
#include "../service/route.h"
#include "../service/bus.h"
#include "../charge/parade.h"
#include "../charge/charge.h"
/*
Archivo de definicion de los procesos hijo
los cuales son los encargados de administrar una ruta cada uno.

Mediante hilos simula el movimiento de los autobuses y le informa al padre el
estado actual cada minuto de simulacion mediante su pipe asociado
almacenado en la estructura Route que administra
*/
/*Estructura para almacenar todos los semaforos por subproceso*/
struct semaphores
{
    sem_t paradeRead;
    sem_t paradeWrite;
    sem_t* busesRead;
    sem_t* busesWrite; 
};

/*Estructura paraa enviar los argumentos de las funciones de hilos*/
struct threadBusArg
{
    Bus* bus;
    Parade* parade;
    int* numBusesOnService;
    int* numBuffersReady;
    float timeFactor;
    char** timeRange;
    char* travelTime;
    char* leaveTime;
    char arrivalTime[6];
    char returnTime[6];
    char finishTime[6];
    sem_t* semRead;
    sem_t* semWrite;
};

// Punto de inicio del proceso hijo
void childProcessStart(Route* route, char* time[2]);
#endif // CHILD_P
