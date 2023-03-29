#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "../backend/utils/hours.h"
/*
Cabecera del Programa
*/
void printHeader()
{
    printf("\tSIMULADOR TRANSPORTE USB\n");
}


/*
Reporte Final de la ejecucion
*/

void printfFinalReport( int inTime, int outTime, int fail )
{
    /*
    Se Muestran los resultados obtenidos
    */
    printf("###### RESULTADOS DE LA SIMULACION ######\n");
    printf("Numero de personas que llegaron a tiempo a la Universidad: %d\n", inTime);
    printf("Numero de personas que llegaron tarde a la Universidad: %d\n", outTime);
    printf("Numero de personas que No llegaron a la Universidad: %d\n", fail);
    
}