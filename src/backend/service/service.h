#if !defined(SERVICE_H)
#define SERVICE_H

typedef struct Service Service;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/list.h"

/*
Un servicio es la abstraccion del servicio de transporte
Por ejemplo los transportes hacia BRT

Miembros:
- travelTime: Duracion del recorrido
- buses: Lista de autobuses asociados a la ruta (cada autobus conoce su hora de salida,
    esto permite que diferentes rutas puedan tener diferentes horas de salida de autobuses).
*/

struct Service
{
    List* buses;
};

void initService( Service* service );
/*
Insertar Bus a la lista de buses del servicio
*/
void insertBus( Service* service, Bus* bus );
/*
Se inserta un autobus al servicio
*/
void insertBusToService( Service* service, Bus* bus );
/*
Dada una secuencia de autobuses se llena la lista de autobuses
que prestaran el
*/
Service* createService( char* stoken );

#endif // SERVICE_H
