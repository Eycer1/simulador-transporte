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
    sem_t *busesRead;
    sem_t *busesWrite;
};

/*Estructura paraa enviar los argumentos de las funciones de hilos*/
struct threadBusArg
{
    Bus *bus;
    Parade *parade;
    int *numBusesOnService;
    int *numBuffersReady;
    float timeFactor;
    char **timeRange;
    char *travelTime;
    char *leaveTime;
    char arrivalTime[6];
    char returnTime[6];
    char finishTime[6];
    sem_t *semRead;
    sem_t *semWrite;
};

// Punto de inicio del proceso hijo
void childProcessStart(Route *route, char *time[2]);

/**
 * Rutina de despacho de cargas de usuarios en la cola de la parada.
 *
 * @param arguments Puntero a una estructura threadBusArg que contiene los argumentos para la función.
 */
void *paradeTracker(void *arguments);

/**
 * Llena el buffer según el estado y el porcentaje de progreso.
 *
 * @param buffer Puntero a una cadena de caracteres que representa el buffer a llenar.
 * @param bufferSize Tamaño del buffer.
 * @param percentage Porcentaje de progreso.
 * @param status Estado actual. Puede ser -1, 0, 1 o 2.
 */
void fillBuffer(char *buffer, int bufferSize, int percentage, int status);

/**
 * Escribe en el buffer del autobús el progreso del viaje durante uno de los trayectos. El trayecto y la dirección se deciden según el estado del autobús.
 *
 * @param bus Puntero al objeto Bus cuyo buffer se actualizará.
 * @param parade Puntero al objeto Parade asociado al autobús.
 * @param clock Cadena de caracteres que representa el tiempo actual del reloj.
 * @param travelTime Cadena de caracteres que representa el tiempo de viaje entre paradas.
 * @param leaveTime Cadena de caracteres que representa el tiempo de salida del autobús desde la parada.
 * @param arrivalTime Cadena de caracteres que representa el tiempo de llegada del autobús a la parada.
 * @param returnTime Cadena de caracteres que representa el tiempo de regreso del autobús desde la parada.
 * @param finishTime Cadena de caracteres que representa el tiempo de finalización del viaje del autobús.
 */
void reportTravelProgress(Bus *bus, Parade *parade, char *clock, char *travelTime, char *leaveTime,
                          char *arrivalTime, char *returnTime, char *finishTime);

/**
 * Función de ejecución de hilo que hace el monitoreo y reporte del estado de cada autobús.
 *
 * @param arguments Puntero a una estructura threadBusArg que contiene los argumentos para la función.
 */
void *busTracker(void *arguments);

/**
 * Inicializa los semáforos de lectura y de escritura para cada hilo.
 *
 * @param sem Puntero a una estructura semaphores que contiene los semáforos a inicializar.
 * @param numBuses Número de autobuses para los que se inicializarán los semáforos.
 */
void initSemaphores( struct semaphores* sem, int numBuses );

/**
 * Espera todos los semáforos relacionados con la lectura.
 *
 * @param sem Puntero a una estructura semaphores que contiene los semáforos a esperar.
 * @param numBuses Número de autobuses para los que se esperarán los semáforos.
 */
void waitReadSemaphores( struct semaphores* sem, int numBuses);

/**
 * Libera los semáforos relacionados con la escritura.
 *
 * @param sem Puntero a una estructura semaphores que contiene los semáforos a liberar.
 * @param numBuses Número de autobuses para los que se liberarán los semáforos.
 */
void postWriteSemaphores( struct semaphores* sem, int numBuses);

/**
 * Calcula la cantidad de usuarios que llegaron a tiempo, los que llegaron tarde y los que no llegaron en esta ruta.
 *
 * @param route Puntero al objeto Route para el que se calcularán los resultados.
 * @param numUsersInTime Puntero a un entero que se actualizará con el número de usuarios que llegaron a tiempo.
 * @param numUsersOutTime Puntero a un entero que se actualizará con el número de usuarios que llegaron tarde.
 * @param numUsersFail Puntero a un entero que se actualizará con el número de usuarios que no llegaron.
 */
void findResults( Route* route, int* numUsersInTime, int* numUsersOutTime, int* numUsersFail );


/**
 * Escribe los resultados que se reportan al padre de la eficiencia del servicio.
 *
 * @param route Puntero al objeto Route para el que se escribirán los resultados.
 */
void writeResults( Route* route );

/**
 * Creador de los hilos de ejecución. Se creará un hilo por cada autobús de la ruta, se creará un autobús para incorporar cargas de usuarios y un hilo reloj para llevar la hora en la parada a la cola de espera en la parada.
 *
 * @param threads Arreglo de hilos a inicializar.
 * @param route Puntero al objeto Route para el que se crearán los hilos.
 * @param timeRange Arreglo de dos cadenas de caracteres que representan el rango de tiempo en el que se ejecutarán los hilos.
 * @param sem Puntero a una estructura semaphores que contiene los semáforos a utilizar por los hilos.
 */
void initThreads( pthread_t* threads, Route* route, char* timeRange[2], struct semaphores* sem );

/**
 * Espera los hilos que finalizan.
 *
 * @param threads Arreglo de hilos a esperar.
 * @param numBuses Número de autobuses para los que se esperarán los hilos.
 */
void joinThreads(pthread_t* threads, int numBuses);

/**
 * Proceso de monitoreo de autobuses.
 *
 * @param route Puntero al objeto Route para el que se realizará el monitoreo.
 * @param timeRange Arreglo de dos cadenas de caracteres que representan el rango de tiempo en el que se ejecutará el monitoreo.
 * @param sem Puntero a una estructura semaphores que contiene los semáforos a utilizar por el proceso de monitoreo.
 */
void monitorBusesDeamon( Route* route, char* timeRange[2], struct semaphores* sem );

/**
 * Este es el punto de inicio de los procesos hijos que se encargan de administrar las rutas que se les fue asignada por el proceso padre.
 *
 * @param route Puntero al objeto Route que se administrará.
 * @param timeRange Arreglo de dos cadenas de caracteres que representan el rango de tiempo en el que se ejecutará el proceso hijo.
 */
void childProcessStart( Route* route, char* timeRange[2] );
#endif // CHILD_P
