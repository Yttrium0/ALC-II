#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpioControl.h"
#include "timeControle.h"
#include "ephemeride.h"

const float timer = 60; //nb sec avant arret automatique lumière

int main(void)
{
        int tZ = 1;//decalage horaire(été/hiver)
        int tcr = 0;//type de crépuscule
        int la = 48.9149;//latitude géographique
        int lon = 2.2526599999999917;//longitude géographique
        int alt = 0;//altitude

    while (1)
    {
        //initialisation des variables temps

        int a = localTime("annee");
        int m = localTime("mois");
        int j = localTime("jour");
        int heureLocal = localTime("heure");
        int minuteLocal = localTime("minute");
        float hr_minLocal = (float)heureLocal + ((float)minuteLocal / 60);
        int heureLeveSoleil = lmvSoleil(a, m, j, tZ, tcr, la, lon, alt, 0, 0);
        int minuteLeveSoleil = lmvSoleil(a, m, j, tZ, tcr, la, lon, alt, 0, 1);
        float hr_minLSoleil = (float)heureLeveSoleil + ((float)minuteLeveSoleil / 60);
        int heureCoucheSoleil = lmvSoleil(a, m, j, tZ, tcr, la, lon, alt, 1, 0);
        int minuteCoucheSoleil = lmvSoleil(a, m, j, tZ, tcr, la, lon, alt, 1, 1);
        float hr_minCSoleil = (float)heureCoucheSoleil + ((float)minuteCoucheSoleil / 60); 
        int chronometre = 0;

        //savoir si c jour ou nuit

        if (hr_minLSoleil >= hr_minLocal || hr_minLocal >= hr_minCSoleil){

            if (!lightControl(99) && IRSensor() && CAMSensor()) {
                lightControl(1); //eteint + mouv
            }else if (lightControl(99) && (IRSensor() || CAMSensor())) {
                chronometre = 0;//allumé + mouv
            }else if (lightControl(99) && (!IRSensor() && !CAMSensor())) {
                sleep(0.5);//allumé + eteint
                chronometre = chronometre + 0.5;
            }

            if (chronometre == timer) {
                lightControl(0);
            }
        }

    return 0;
}



