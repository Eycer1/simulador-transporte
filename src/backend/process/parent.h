#if !defined(PARENT_PROCESS_H)
#define PARENT_PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"


void parentProcessStart( List* routesList, float timeFactor, char* timeRange[2],
                            int* inTime, int* outTime, int* fail );

#endif // PARENT_PROCESS_H
