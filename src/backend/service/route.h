#if !defined(ROUTE_H)
#define ROUTE_H

/*
Una ruta es en conjunto el servicio que oferta el transporte y la parada quien mantiene la demanda de este
Esta es la estructura que usan los procesos hijos para administrar y notificar al proceso padre
para que este reporte por consola.
*/
typedef struct route Route;

#include <unistd.h>
#include "../charge/parade.h"
#include "service.h"

struct route
{
    char cod[4];
    char name[50];
    char travelTime[6];
    int pip[2]; // Pipe de comunicacion con el proceso padre
    Parade *parade;
    Service *service;
};

/**
 * @brief Inicializa una ruta.
 *
 * @param route Puntero a la ruta a inicializar.
 * @param cod Código de la ruta.
 * @param name Nombre de la ruta.
 * @param travelTime Tiempo de viaje de la ruta.
 * @param parade Puntero a la parada asociada a la ruta.
 * @param service Puntero al servicio asociado a la ruta.
 */
void initRoute(Route *route, char *cod, char *name, char *travelTime, Parade *parade, Service *service);

/**
 * @brief Crea una lista de rutas correspondiente a los archivos especificados.
 * La lista contendrá estructuras Route que están explicadas en su archivo correspondiente.
 *
 * @param serviceFile Nombre del archivo de servicios.
 * @param loadFile Nombre del archivo de cargas.
 * @return Puntero a la lista de rutas creada.
 */
List *createRoutes(char *serviceFile, char *loadFile);

#endif // ROUTE_H
