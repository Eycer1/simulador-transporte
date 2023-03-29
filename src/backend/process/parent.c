#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "parent.h"
#include "child.h"
#include "../utils/hours.h"
#include "../../frontend/main.h"
#define MSGSIZE 1024

void createChildren(List *routeElementList, char *timeRange[2], float timeFactor)
{
    while (routeElementList->next)
    {
        if (fork() == 0)
        {
            // Se cierra el descriptor de lectura en los procesos hijo
            close(routeElementList->content->route->pip[0]);
            childProcessStart(routeElementList->content->route, timeRange);
            close(routeElementList->content->route->pip[1]);
            exit(0);
        }
        // Para el proceso padre se cierra el descriptor de escritura
        close(routeElementList->content->route->pip[1]);
        routeElementList = routeElementList->next;
    }
}

void waitChildren(List *routeElementList)
{
    int n = routeElementList->lenght;
    for (int i = 0; i < n; i++)
    {
        wait(NULL);
    }
}

char *getRoutesStatus(List *routesList)
{
    int i = 0;
    char *prtStr = (char *)malloc(sizeof(char));
    while (routesList->next)
    {
        i++;
        prtStr = realloc(prtStr, sizeof(char) * MSGSIZE * i);
        int fd = routesList->content->route->pip[0];
        char buffer[MSGSIZE] = {0};
        int flag;
        /* Si en el pipe no hay mensaje, esperar hasta que llegue el mensaje
        y volver a intentarlo */
        while ((flag = read(fd, buffer, MSGSIZE)) == 0)
            ;
        strcat(prtStr, buffer);
        routesList = routesList->next;
    }
    return prtStr;
}

void monitorRoutesDeamon(List *routesList, float timeFactor, char *timeRange[2])
{
    int minutes = 0;
    int maxMin = hoursDifference(timeRange[1], timeRange[0]);
    int hour, min;
    sscanf(timeRange[0], "%d:%d", &hour, &min);
    hoursDifference(timeRange[1], timeRange[0]);
    while (minutes < maxMin)
    {
        system("clear");
        printHeader();
        printf("%02d:%02d\n", hour, min);
        char *prtString = getRoutesStatus(routesList);
        printf("%s\n", prtString);
        usleep(timeFactor * 1000000);
        incressHour(&hour, &min);
        minutes++;
    }
}

void getChildrenResults(List *routesList, int *inTime, int *outTime, int *fail)
{
    while (routesList->next)
    {
        int inTimeResult, outTimeResult, failResult;
        int fd = routesList->content->route->pip[0];
        char buffer[MSGSIZE] = {0};

        /* Se obtiene el resultado que dejo el proceso hijo antes de morir*/
        read(fd, buffer, MSGSIZE);
        sscanf(buffer, "%d %d %d", &inTimeResult, &outTimeResult, &failResult);
        *inTime += inTimeResult;
        *outTime += outTimeResult;
        *fail += failResult;
        routesList = routesList->next;
    }
}

void parentProcessStart(List *routesList, float timeFactor, char *timeRange[2],
                        int *inTime, int *outTime, int *fail)
{
    *inTime = 0;
    *outTime = 0;
    *fail = 0;
    printf("Iniciando proceso padre...\n");
    sleep(1);
    createChildren(routesList, timeRange, timeFactor);
    monitorRoutesDeamon(routesList, timeFactor, timeRange);
    waitChildren(routesList);
    getChildrenResults(routesList, inTime, outTime, fail);
}
