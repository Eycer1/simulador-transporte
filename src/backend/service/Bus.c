#include "bus.h"
/*
Se inicializa el autobus con su capacidad, la hora de partida
la cantidad de puestos libres, la capacidad y la lista
de cargas 
*/
void initBus(Bus* bus, int capacity, char* leaveTime)
{
    if (capacity < 1)
    {   
        perror("Error, bus capacity most be greater than 0");
        abort();
    }
    
    bus->capacity = capacity;
    bus->free = capacity;
    bus->leaveTime = (char*) malloc(sizeof(char)*6);
    bus->charges = (List*) malloc(sizeof(List));
    strcpy( bus->leaveTime, leaveTime );
    initList( bus->charges );
}

/*
Establece el estatus del autobus como esperando
*/
void setBusAsWaiting( Bus* bus)
{
    bus->status = 0;
}

/*
Establece el estatus del autobus como en camino a la parada
*/
void setBusAsGoing( Bus* bus )
{
    bus->status = -1;
}

/*
Establece el estatus del autobus como regresando a la universidad
*/
void setBusAsReturning( Bus* bus )
{
    bus->status = 1;
}

/*
Establece el estatus del autobus como finalizado
*/
void setBusAsFinished( Bus* bus )
{
    bus->status = 2;
}

/*
Se agrega carga al autobus
*/
void fillBusWithChargesFromParade( Parade* parade, Bus* bus )
{
    if (bus->free > 0 && parade->numWaiting > 0 )
    {
        List* list = popUsersFromParade(parade, &bus->free);
        while (list->next) list = list->next;
        list->next = bus->charges;
    }
}