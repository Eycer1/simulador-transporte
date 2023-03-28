/*
Esta es la defincion de una lista enlazada
Se intenta que sea lo mas general posible para su reutilizacion
en todo el proyecto

Si se desea utilizar con otro contenido solo se debe agregar el
tipo del contenido al union listContent
*/

#ifndef LIST_HEADER
#define LIST_HEADER


typedef union listContent ListContent;
typedef struct List List;

#include <time.h>
#include "../service/bus.h"
#include "../service/service.h"
#include "../service/route.h"
#include "../charge/charge.h"


union listContent
{
    Charge* charge;
    Bus* bus;
    Service* service;
    Route* route;
};
struct List
{
    List* next;
    ListContent* content;
    int lenght;
};

int initList( List* list );
List* listPush( List* old, ListContent* content);
List* listPushAtEnd( List* old, ListContent* content);
List* reverseList( List* list );
void printList(List* list);

#endif