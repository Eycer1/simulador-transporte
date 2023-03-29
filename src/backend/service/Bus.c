#include "bus.h"

void initBus(Bus *bus, int capacity, char *leaveTime)
{
    if (capacity < 1)
    {
        perror("Error, bus capacity most be greater than 0");
        abort();
    }

    bus->capacity = capacity;
    bus->free = capacity;
    bus->leaveTime = (char *)malloc(sizeof(char) * 6);
    bus->charges = (List *)malloc(sizeof(List));
    bus->status = -2;
    strcpy(bus->leaveTime, leaveTime);
    initList(bus->charges);
}

void setBusAsWaiting(Bus *bus)
{
    bus->status = 0;
}

void setBusAsGoing(Bus *bus)
{
    bus->status = -1;
}

void setBusAsReturning(Bus *bus)
{
    bus->status = 1;
}

void setBusAsFinished(Bus *bus)
{
    bus->status = 2;
}

void fillBusWithChargesFromParade(Parade *parade, Bus *bus)
{
    if (bus->free > 0 && parade->numWaiting > 0)
    {
        List *list = popUsersFromParade(parade, &bus->free);
        mergeList(&bus->charges, list);
    }
}