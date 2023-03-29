#if !defined(BUS_H)
#define BUS_H

typedef struct Bus Bus;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/list.h"
#include "../charge/parade.h"

/*
Esta estructura especifica un bus de servicio
Miembros:
- capacity: capacidad de personas que pueden abordar el autobus
- leaveTime: arreglo de caracteres con la hora y minuto de salida del autobus 
    desde la universidad
- free: numero de acientos libres
- status: {
        -2: Si no ha comenzado a prestar el servicio
        -1: "En camino de ida a la parada"
        0: "Esperando en la parada los 10 minutos"
        1: "En camino de regreso"
        2: "Finalizo el servicio" o finalizo el servicio
    }
- progress: progreso del estatus actual (del 1 al 10)
- charges: Lista de cargas de pasajeros que abordaron el autobus
*/
struct Bus
{
    int capacity;
    int free;
    int status;
    int progress;
    char* leaveTime; // HH:MM
    char buffer[15]; //Buffer de barra de progreso
    List* charges; 
};

void initBus(Bus* bus, int capacity, char* leaveTime);
void setBusAsWaiting( Bus* bus);
void setBusAsGoing( Bus* bus );
void setBusAsReturning( Bus* bus );
void setBusAsFinished( Bus* bus );
void fillBusWithChargesFromParade( Parade* parade, Bus* bus );
#endif // BUS_H
