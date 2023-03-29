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
    char *leaveTime; // HH:MM
    char buffer[15]; // Buffer de barra de progreso
    List *charges;
};

/**
 * @brief Inicializa el autobús con su capacidad, la hora de partida,
 * la cantidad de puestos libres, la capacidad y la lista de cargas.
 *
 * @param bus Puntero al autobús a inicializar.
 * @param capacity Capacidad del autobús.
 * @param leaveTime Hora de partida del autobús.
 */
void initBus(Bus *bus, int capacity, char *leaveTime);

/**
 * @brief Establece el estado del autobús como esperando.
 *
 * @param bus Puntero al autobús a modificar.
 */
void setBusAsWaiting(Bus *bus);

/**
 * @brief Establece el estado del autobús como en camino a la parada.
 *
 * @param bus Puntero al autobús a modificar.
 */
void setBusAsGoing(Bus *bus);

/**
 * @brief Establece el estado del autobús como regresando a la universidad.
 *
 * @param bus Puntero al autobús a modificar.
 */
void setBusAsReturning(Bus *bus);

/**
 * @brief Establece el estado del autobús como finalizado.
 *
 * @param bus Puntero al autobús a modificar.
 */
void setBusAsFinished(Bus *bus);

/**
 * @brief Agrega carga al autobús desde la parada.
 *
 * @param parade Puntero a la parada de donde se tomarán las cargas.
 * @param bus Puntero al autobús a llenar con cargas.
 */
void fillBusWithChargesFromParade(Parade *parade, Bus *bus);
#endif // BUS_H
