#include <stdlib.h>
#include "utils.h"
#include "hours.h"
/*
Calculadora del Porcentaje de progreso
*/
int getProgressPercentage( char* startTime, char* endTime, char* travelTime, char* clock)
{
    int total = timeToMinutes(travelTime);
    int past = hoursDifference( endTime, clock );
    return past*100/total;
}
