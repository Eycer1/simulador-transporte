#if !defined(PARADE_H)
#define PARADE_H

typedef struct parade Parade;

#include <stdlib.h>
#include <string.h>
#include "../utils/list.h"
#include "charge.h"
#include <pthread.h>
#include <semaphore.h>

/**
 * Estructura Parade para simular el estado de una parada.
 *
 * Miembros:
 * waitingCharges: Lista de cargas de usuarios que han llegado a la parada.
 * charges: Lista de cargas en la parada.
 * numWaiting: Cantidad de personas esperando en la fila de la parada para abordar el autobús. En todo caso, debe ser igual a la suma de los pasajeros de todas las cargas que hay en la lista de cargas.
 * buffer: Código de la parada. Esta debe representar el lugar de ubicación de la parada. Por ejemplo, BRT para Baruta.
 */
struct parade
{
    List *waitingCharges;
    List *charges;
    int numWaiting;
    char buffer[6];
};

/**
 * Se inicializa la parada con el codigo, la cantidad de personas esperando se inicia en 0 y
 * la lista de cargas vacias
 *
 * @param parade Puntero al objeto Parade que se va a inicializar.
 */
void initParade(Parade *parade);

/**
 * Agrega una nueva carga a la lista de cargas que llegarán a la parada.
 *
 * @param parade Puntero al objeto Parade al que se agregará la carga.
 * @param charge Puntero al objeto Charge que se agregará a la lista de cargas que llegarán a la parada.
 */
void insertChargeToParade(Parade *parade, Charge *charge);

/**
 * Agrega una nueva carga a la lista de cargas en espera en la parada. Se extrae directamente de la lista de cargas que llegarán a la parada.
 *
 * @param parade Puntero al objeto Parade al que se agregará la carga en espera.
 */
void insertChargeWaiting(Parade *parade);

/**
 * Crea una parada dada la secuencia de cargas que llegarán al destino en ctoken y el arreglo de horas en las que llegarán las cargas a la parada.
 *
 * @param ctoken Cadena de caracteres que representa la secuencia de cargas que llegarán al destino. Las cargas están separadas por comas.
 * @param hours Arreglo de enteros que representa las horas en las que llegarán las cargas a la parada.
 * @return Puntero al objeto Parade creado.
 */
Parade *createParade(char *ctoken, int *hours);

/**
 * Retorna la lista de cargas a subir de la parada al autobús y actualiza la cola de cargas en espera retirando las cargas que subieron y separando las cargas que no subieron completamente, dejando en la cola al remanente.
 *
 * @param parade Puntero al objeto Parade del que se extraerán las cargas.
 * @param freeSpace Puntero a un entero que representa el espacio libre en el autobús. Se actualizará con el espacio libre restante después de subir las cargas.
 * @return Lista de cargas a subir al autobús.
 */
List *popUsersFromParade(Parade *parade, int *free);
#endif