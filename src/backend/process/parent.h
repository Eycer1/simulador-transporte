#if !defined(PARENT_PROCESS_H)
#define PARENT_PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"

/*
 * Esta función crea subprocesos y asigna una ruta a cada uno.
 * Cada subproceso se encarga de administrar su propia ruta.
 *
 * @param routeElementList: Una lista de elementos de ruta.
 * @param timeRange: Un arreglo de dos elementos que especifica el rango de tiempo.
 * @param timeFactor: Un factor de tiempo que se utiliza en el cálculo del tiempo de espera.
 */
void createChildren(List *routeElementList, char *timeRange[2], float timeFactor);

/*
 * Esta función espera a que todos los subprocesos hijos del proceso padre terminen.
 *
 * @param routeElementList: Una lista de elementos de ruta.
 */
void waitChildren(List *routeElementList);

/*
 * Esta función lee los pipes de todas las rutas y devuelve un informe de estado.
 * Cada subproceso encargado de administrar una ruta debe generar un informe cada minuto.
 *
 * @param routesList: Una lista de rutas.
 * @return: Una cadena de caracteres que contiene el informe de estado de todas las rutas.
 */
char *getRoutesStatus(List *routesList);

/*
 * Esta función monitorea el estado de los subprocesos hijos y se ejecuta en un ciclo cada timeFactor segundos.
 *
 * @param routesList: Una lista de rutas.
 * @param timeFactor: Un factor de tiempo que determina la frecuencia de ejecución del ciclo.
 * @param timeRange: Un arreglo de dos elementos que especifica el rango de tiempo.
 */
void monitorRoutesDeamon(List *routesList, float timeFactor, char *timeRange[2]);

/*
 * Esta función obtiene los resultados de los subprocesos hijos y los almacena en las variables proporcionadas.
 *
 * @param routesList: Una lista de rutas.
 * @param inTime: Un puntero a una variable donde se almacenará el número de procesos que terminaron a tiempo.
 * @param outTime: Un puntero a una variable donde se almacenará el número de procesos que no terminaron a tiempo.
 * @param fail: Un puntero a una variable donde se almacenará el número de procesos que fallaron.
 */
void getChildrenResults(List *routesList, int *inTime, int *outTime, int *fail);

/*
 * Esta función es el punto de arranque del proceso padre.
 * El proceso padre genera tantos subprocesos hijos como rutas existan y se encarga de reportar los mensajes de los subprocesos hijos.
 * Los mensajes se imprimen en pantalla dependiendo de los mensajes de los subprocesos hijos.
 *
 * @param routesList: Una lista de rutas.
 * @param timeFactor: Un factor de tiempo que se utiliza en el cálculo del tiempo de espera.
 * @param timeRange: Un arreglo de dos elementos que especifica el rango de tiempo.
 * @param inTime: Un puntero a una variable donde se almacenará el número de procesos que terminaron a tiempo.
 * @param outTime: Un puntero a una variable donde se almacenará el número de procesos que no terminaron a tiempo.
 * @param fail: Un puntero a una variable donde se almacenará el número de procesos que fallaron.
 */
void parentProcessStart(List *routesList, float timeFactor, char *timeRange[2],
                        int *inTime, int *outTime, int *fail);

#endif // PARENT_PROCESS_H
