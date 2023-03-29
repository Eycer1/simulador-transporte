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
    Charge *charge;
    Bus *bus;
    Service *service;
    Route *route;
};
struct List
{
    List *next;
    ListContent *content;
    int lenght;
};

/**
 * @brief Inicializa una lista.
 *
 * @param list Puntero a la lista a inicializar.
 * @return 0 si la inicialización fue exitosa.
 */
int initList(List *list);

/**
 * @brief Agrega un nodo al principio de la lista.
 *
 * @param old Puntero a la lista donde se agregará el nodo.
 * @param content Contenido del nodo a agregar.
 * @return Puntero a la nueva lista con el nodo agregado.
 */
List *listPush(List *old, ListContent *content);

/**
 * @brief Agrega un nodo al final de la lista.
 *
 * @param old Puntero a la lista donde se agregará el nodo.
 * @param content Contenido del nodo a agregar.
 * @return Puntero a la nueva lista con el nodo agregado.
 */
List *listPushAtEnd(List *old, ListContent *content);

/**
 * @brief Invierte el orden de una lista.
 *
 * @param list Puntero a la lista a invertir.
 * @return Puntero a la nueva lista invertida.
 */
List *reverseList(List *list);

/**
 * @brief Imprime una lista elemento por elemento, incluyendo el nodo final.
 *
 * @param list Puntero a la lista a imprimir.
 */
void mergeList(List **answer, List *list);

/**
 * @brief Fusiona dos listas en una sola.
 *
 * @param answer Puntero al puntero de la lista donde se almacenará el resultado.
 * @param list Puntero a la segunda lista a fusionar.
 */
void printList(List *list);

#endif