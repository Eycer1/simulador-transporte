#include "parade.h"

void initParade(Parade *parade)
{
    parade->charges = (List *)malloc(sizeof(List));
    parade->waitingCharges = (List *)malloc(sizeof(List));
    parade->numWaiting = 0;
    parade->buffer[0] = ' ';
    initList(parade->charges);
    initList(parade->waitingCharges);
}

void insertChargeWaiting(Parade *parade)
{
    ListContent *content = parade->charges->content;
    if (content)
    {
        parade->waitingCharges = listPush(parade->waitingCharges, content);
        parade->numWaiting++;
    }
}

void insertChargeToParade(Parade *parade, Charge *charge)
{
    ListContent *content = (ListContent *)malloc(sizeof(ListContent));
    content->charge = charge;
    parade->charges = listPushAtEnd(parade->charges, content);
}

List *popUsersFromParade(Parade *parade, int *freeSpace)
{

    List *list = (List *)malloc(sizeof(List));
    initList(list);
    List *queue = parade->waitingCharges;
    while (*freeSpace > 0 && parade->numWaiting > 0)
    {
        queue = parade->waitingCharges;
        if (queue->content->charge->numUsers > *freeSpace)
        {
            /*
            Se fracciona la carga en la cola y se sube unicamente
            la parte de la carga que cabe en el autobus
            */
            Charge *charge = (Charge *)malloc(sizeof(Charge));
            ListContent *content = (ListContent *)malloc(sizeof(ListContent));
            charge->numUsers = *freeSpace;
            charge->parateArrivalTime = queue->content->charge->parateArrivalTime;
            content->charge = charge;
            list = listPush(list, content);
            queue->content->charge->numUsers -= *freeSpace;
            parade->numWaiting -= *freeSpace;
            *freeSpace = 0;
        }
        else if (queue->content->charge->numUsers <= *freeSpace)
        {
            /*
            Se sube la ultima carga de la cola al autobus, y se elimina de
            la cola de cargas esperando en la parada
            */

            /*Se sube la carga al autobus y se saca de la cola*/
            parade->waitingCharges = parade->waitingCharges->next;
            queue->next = list;
            list = queue;
            parade->numWaiting -= list->content->charge->numUsers;
            *freeSpace -= list->content->charge->numUsers;
        }
    }
    return list;
}

Parade *createParade(char *ctoken, int *hours)
{
    Parade *parade = (Parade *)malloc(sizeof(Parade));
    initParade(parade);

    for (int i = 0; ctoken != NULL; i++)
    {

        int numUsers = atoi(ctoken);
        // No se guardan cargas vacias en la lista de cargas
        // que llegaran a la parada
        if (numUsers > 0)
        {
            Charge *charge = (Charge *)malloc(sizeof(Charge));
            initCharge(charge, hours[i], numUsers);
            insertChargeToParade(parade, charge);
        }
        ctoken = strtok(NULL, ",");
    }
    return parade;
}