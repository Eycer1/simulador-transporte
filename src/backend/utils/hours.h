#if !defined(HOURS_H)
#define HOURS_H

/**
 * @brief Incrementa en un minuto la hora dada.
 *
 * @param hour Puntero a la hora a incrementar.
 * @param min Puntero al minuto a incrementar.
 */
void incressHour(int *hour, int *min);

/**
 * @brief Convierte una cadena de tiempo en minutos.
 *
 * @param travelTime Cadena que contiene el tiempo a convertir.
 * @return Tiempo en minutos.
 */
int timeToMinutes(char *travelTime);

/**
 * @brief Calcula la diferencia en minutos entre dos horas.
 * La hora1 debe estar después o al mismo tiempo que la hora2.
 *
 * @param hour1 Cadena que contiene la primera hora.
 * @param hour2 Cadena que contiene la segunda hora.
 * @return Diferencia en minutos entre las dos horas.
 */
void hourSum(char *result, char *hour1, char *hour2);

/**
 * @brief Suma dos horas y almacena el resultado en una cadena.
 *
 * @param result Cadena donde se almacenará el resultado.
 * @param hour1 Cadena que contiene la primera hora a sumar.
 * @param hour2 Cadena que contiene la segunda hora a sumar.
 */
int hoursDifference(char *hour1, char *hour2);

/**
 * @brief Incrementa en un minuto la hora dada y almacena el resultado en una cadena.
 *
 * @param result Cadena donde se almacenará el resultado.
 * @param hour Cadena que contiene la hora a incrementar.
 */
void incressHour2(char *result, char *hour);

#endif // HOURS_H
