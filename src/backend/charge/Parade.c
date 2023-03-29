#include "parade.h"

/*
Se inicializa la parada con el codigo, la cantidad de personas esperando se inicia en 0 y
la lista de cargas vacias
*/
void initParade( Parade* parade)
{
    parade->charges = (List*) malloc(sizeof(List));
    parade->waitingCharges = (List*) malloc(sizeof(List));
    parade->numWaiting = 0;
    parade->buffer[0] = ' ';
    initList( parade->charges );
    initList( parade->waitingCharges );
}

/*
Se agrega una nueva carga a la lista de cargas en la parada.
se extrae directamente de la lista de cargas que llegaran a la parada
*/
void insertChargeWaiting( Parade* parade ){
    ListContent* content = parade->charges->content;
    if (content)
    {
        parade->waitingCharges = listPush( parade->waitingCharges, content );
        parade->numWaiting++;
    }
}

/*
Se agrega una nueva carga a la lista de cargas que llegaran a la parada
*/
void insertChargeToParade( Parade* parade, Charge* charge ){
    ListContent* content = (ListContent*) malloc(sizeof(ListContent));
    content->charge = charge;
    parade->charges = listPushAtEnd( parade->charges, content );
}

/*
Se retorna la Lista de cargas a subir de la parada al 
autobus y acualiza la cola de cargas en espera retirando las cargas
que subieron y separando las cargas que no subieron completamente
dejando en la cola al remanente
*/
List* popUsersFromParade( Parade* parade, int* freeSpace )
{
    if ( parade->numWaiting > 0 && *freeSpace > 0)
    {
        List* list = (List*) malloc( sizeof(List) );
        initList( list );
        List* queue = parade->waitingCharges;
        while ( *freeSpace > 0  && parade->numWaiting > 0)
        {
            queue = parade->waitingCharges;
            printf("%d\n", queue->content->charge->parateArrivalTime);
            if (queue->content->charge->numUsers > *freeSpace)
            {
                /*
                Se fracciona la carga en la cola y se sube unicamente
                la parte de la carga que cabe en el autobus
                */
                Charge* charge = (Charge*) malloc( sizeof(Charge) );
                ListContent* content = (ListContent*) malloc( sizeof(ListContent) );
                charge->numUsers = *freeSpace;
                charge->parateArrivalTime = queue->content->charge->parateArrivalTime;
                content->charge = charge;
                listPush(list, content);
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
                parade->waitingCharges = queue->next;
                queue->next = list;
                list = queue;
                parade->numWaiting -= list->content->charge->numUsers;
                *freeSpace -= list->content->charge->numUsers;
            }
        }
        return list;
        
    }
    return NULL;
    
}

/*
Se crea una parada dada la secuencia de cargas que llegaran al destino
en ctoken, y el el arrego de horas en las que llegaran las cargas a la parada
*/
Parade* createParade(char* ctoken, int* hours)
{
    Parade* parade = (Parade*) malloc(sizeof(Parade));
    initParade(parade);

    for (int i = 0; ctoken != NULL; i++)
    {
        
        int numUsers = atoi(ctoken);
        // No se guardan cargas vacias en la lista de cargas
        // que llegaran a la parada
        if(numUsers>0)
        {
            Charge* charge = (Charge*) malloc(sizeof(Charge));
            initCharge(charge, hours[i], numUsers);
            insertChargeToParade(parade, charge);
        }        
        ctoken = strtok(NULL, ",");
    }
    return parade;
}