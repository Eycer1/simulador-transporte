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
    int pip[2]; //Pipe de comunicacion con el proceso padre
    Parade* parade;
    Service* service;
};

/*
Se inicializa una ruta:
- route: ruta a inicializar
- cod: codigo de la ruta
- name: nombre de la localidad al que se le presta el servicio
- parade: Parada a cubrir
- service: transportes que cubren la ruta
*/
void initRoute( Route* route, char* cod, char* name, char* travelTime, Parade* parade, Service* service );

/*
Se Crea una lista de rutas correspondiente a los archivos especificados
La lista contendra estructuras Route
*/
List* createRoutes( char* serviceFile, char* loadFile);

#endif // ROUTE_H
