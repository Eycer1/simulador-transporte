#include "hours.h"
#include <stdlib.h>
#include <stdio.h>

void incressHour(int* hour, int* min)
{
    *min = ((*min + 1) % 60);
    *hour = *min == 0 ?  *hour + 1 : *hour;
}

int timeToMinutes( char* travelTime )
{
    int hour, min;
    sscanf(travelTime, "%d:%d", &hour, &min);
    return min + hour*60;
}

/*
Diferencia de Horas.
hour1 debe estar despues o al mismo tiempo de hour2
*/
int hoursDifference( char* hour1, char* hour2 )
{
    int h1, h2, m1, m2;
    if (m1 < m2)
    {
        m1 = m1*60;
        h1--;
    }
        
    sscanf(hour1, "%d:%d", &h1, &m1);
    sscanf(hour2, "%d:%d", &h2, &m2);
    
    return (h1-h2)*60 + (m1-m2);
    
}

/*
Se suman dos horas
*/
void hourSum( char* result, char* hour1, char* hour2 )
{
    int h1,h2,m1,m2, h, m;
    sscanf(hour1, "%d:%d", &h1, &m1);
    sscanf(hour2, "%d:%d", &h2, &m2);
    m = ((m1 + m2) % 60);
    h = h1 + h2 + ((m1 + m2) / 60);
    sprintf(result, "%02d:%02d", h,m);
}
void incressHour2(char* result, char* hour)
{
    int h,m;
    sscanf(hour, "%d:%d", &h, &m);
    incressHour( &h, &m );
    sprintf(result,"%02d:%02d", h, m);
}