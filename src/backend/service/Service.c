#include "service.h"
#include <string.h>

void initService(Service *service)
{
    service->buses = (List *)malloc(sizeof(List));
    initList(service->buses);
}

void insertBus(Service *service, Bus *bus)
{
    ListContent *busContent = (ListContent *)malloc(sizeof(ListContent));
    busContent->bus = bus;
    listPush(service->buses, busContent);
}

void insertBusToService(Service *service, Bus *bus)
{
    ListContent *content = (ListContent *)malloc(sizeof(ListContent));
    content->bus = bus;
    service->buses = listPush(service->buses, content);
}

Service *createService(char *stoken)
{
    Service *service = (Service *)malloc(sizeof(Service));
    initService(service);
    // Se leen los autobuses
    for (int i = 0; stoken != NULL; i++)
    {
        Bus *bus = (Bus *)malloc(sizeof(Bus));
        int capacity;
        int hour;
        int min;
        char time[6];
        sscanf(stoken, "%d:%d(%d)", &hour, &min, &capacity);
        sprintf(time, "%02d:%02d", hour, min);
        initBus(bus, capacity, time);
        insertBusToService(service, bus);
        stoken = strtok(NULL, " ");
    }
    return service;
}
