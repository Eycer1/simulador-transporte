#if !defined(UTILS_H)
#define UTILS_H

/**
 * @brief Calcula el porcentaje de progreso de un viaje.
 *
 * @param startTime Cadena que contiene la hora de inicio del viaje.
 * @param endTime Cadena que contiene la hora de fin del viaje.
 * @param travelTime Cadena que contiene el tiempo total del viaje.
 * @param clock Cadena que contiene la hora actual.
 * @return Porcentaje de progreso del viaje.
 */
int getProgressPercentage(char *startTime, char *endTime, char *travelTime, char *clock);

#endif // UTILS_H
