#if !defined(CHARGE_H)
#define CHARGE_H

typedef struct charge Charge;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Estructura Charge para representar una carga de usuarios en una parada.
 *
 * Miembros:
 * parateArrivalTime: Hora de llegada de la carga a la parada.
 * numUsers: Número de usuarios en la carga.
 */
struct charge
{
    int parateArrivalTime;
    int numUsers;
};

/**
 * Inicializa un objeto Charge con el tiempo y el número de usuarios dados. Representa los lotes de usuarios que
 * llegan a la parada en un tiempo determinado y se dirigen a la universidad.
 * Cuando el autobus llega a la parada, el autobus agregara a su lista de cargas de pasajeros las cargas que esperan en la parada
 * hasta alcanzar su capacidad (al abordar se debe separar la carga de la parada en dos y subir los asientos disponibles y
 * dejar en la parada el restante de la carga) y cuando un conjunto de pasajeros llega a la parada se crea una carga con el numero de
 * usuarios que llegaron y la hora de llegada
 * @param charge Puntero al objeto Charge que se va a inicializar.
 * @param time El tiempo que se establecerá para el objeto Charge.
 * @param numUsers El número de usuarios que se establecerá para el objeto Charge. Debe ser no negativo.
 */
void initCharge(Charge *charge, int time, int numUsers);

#endif // CHARGE_H
