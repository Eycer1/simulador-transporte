#if !defined(PARENT_PROCESS_H)
#define PARENT_PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"


void parentProcessStart( List* routes, float timeFactor, char* timeRange[2] );

#endif // PARENT_PROCESS_H
