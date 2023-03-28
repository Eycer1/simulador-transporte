#include "charge.h"

void initCharge( Charge* charge, int time, int numUsers )
{
    if (numUsers < 0)
    {
        perror("Negative users in Charge is not allowed!");
        abort();
    }
    charge->numUsers = numUsers;
    charge->parateArrivalTime = time;
}