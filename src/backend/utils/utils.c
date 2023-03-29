#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "hours.h"

int getProgressPercentage(char *startTime, char *endTime, char *travelTime, char *clock)
{
    int total = timeToMinutes(travelTime);
    int past = hoursDifference(clock, startTime);
    return past * 100 / total;
}
