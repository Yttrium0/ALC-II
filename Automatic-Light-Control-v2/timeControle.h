#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int localTime(char search[])
{
    time_t timestamp;
    time(&timestamp);
    struct tm moment;
    moment = *localtime(&timestamp);

    char annee[10] = "annee";
    char mois[10] = "mois";
    char jour[10] = "jour";
    char heure[10] = "heure";
    char minute[10] = "minute";
    int result;

    if (strcmp(search, annee) == 0)
    {
        result = (int)moment.tm_year + 1900;
    }
    else if (strcmp(search, mois) == 0)
    {
        int mon = (int)moment.tm_mon;
        if(mon == 0)
            result = 12;
        else
            result = mon;      
    }
    else if (strcmp(search, jour) == 0)
    {
        result = (int)moment.tm_mday;
    }
    else if (strcmp(search, heure) == 0)
    {
        result = (int)moment.tm_hour;
    }
    else if (strcmp(search, minute) == 0)
    {
        result = (int)moment.tm_min;
    }

    return result;
}