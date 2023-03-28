#if !defined(PARADE_H)
#define PARADE_H


typedef struct parade Parade;

#include <stdlib.h>
#include <string.h>
#include "../utils/list.h"
#include "charge.h"
#include <pthread.h>
#include <semaphore.h>

/*
Estructura Parada para simular el estado de una parada

Miembros:
cod: Es el codigo de la parada. Esta debe representar el lugar de ubicacion
    de la parada. por ejemplo BRT para baruta
waiting: Cantidad de personas esperando en la fila de la parada para abordar el autobus
        en todo caso debe ser igual a la suma de los pasajeros de todas las cargas que
        hay en la lista de cargas
waitingCharges: Lista de cargas de usuarios que han llegado a la parada
*/
struct parade
{
    List* waitingCharges;
    List* charges;
    int numWaiting;
    char buffer[6];    
};

/*
Se inicializa la parada
*/
void initParade( Parade* parade );

/*
Se agrega una carga a la fila de cargas en la parada
*/
void insertChargeToParade( Parade* parade, Charge* charge );

/*
Se agrega una carga a la fila de cargas esperando en la parada
*/
void insertChargeWaiting( Parade* parade );

/*
Se crea una parada dada la secuencia de cargas que llegaran al destino
en ctoken, y el el arrego de horas en las que llegaran las cargas a la parada
*/
Parade* createParade(char* ctoken, int* hours);

/*
Se retorna la Lista de cargas a subir de la parada al 
autobus y acualiza la cola de cargas en espera retirando las cargas
que subieron y separando las cargas que no subieron completamente
dejando en la cola al remanente
*/
List* popUsersFromParade( Parade* parade, int* free );
#endif