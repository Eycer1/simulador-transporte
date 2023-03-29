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
    List *buses;
};

/**
 * @brief Inicializa el servicio con las listas vacías.
 *
 * @param service Puntero al servicio a inicializar.
 */
void initService(Service *service);

/**
 * @brief Inserta un autobús a la lista de autobuses del servicio.
 *
 * @param service Puntero al servicio donde se insertará el autobús.
 * @param bus Puntero al autobús a insertar.
 */
void insertBus(Service *service, Bus *bus);

/**
 * @brief Agrega un nuevo autobús a la lista de autobuses del servicio.
 *
 * @param service Puntero al servicio donde se agregará el autobús.
 * @param bus Puntero al autobús a agregar.
 */
void insertBusToService(Service *service, Bus *bus);

/**
 * @brief Crea un servicio a partir de una secuencia de autobuses.
 *
 * @param stoken Cadena que contiene la secuencia de autobuses.
 * @return Puntero al servicio creado.
 */
Service *createService(char *stoken);

#endif // SERVICE_H
