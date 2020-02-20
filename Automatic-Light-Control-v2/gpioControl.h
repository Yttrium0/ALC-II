#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>

#define SWITCH_LIGHT_GPIO 28
#define IR_SENSOR_GPIO 7

int lightControl(int cmd)
{
    wiringPiSetup();
    int lightStatus = digitalRead(SWITCH_LIGHT_GPIO);

    int cmdStatus = 99;
    int cmdStart = 1;
    int cmdStop = 0;

    if (cmd == cmdStatus)
    {
        return lightStatus;
    }
    else if (cmd == cmdStart || cmd == cmdStop)
    {
        if (lightStatus == 1)
        {
            digitalWrite(SWITCH_LIGHT_GPIO, 0);
            return 0;
        }
        else if (lightStatus == 0)
        {
            digitalWrite(SWITCH_LIGHT_GPIO, 1);
            return 1;
        }
        else
        {
            printf("Erreur Light Control");
            return -1;
        }
    }
    
}

int IRSensor()
{
    wiringPiSetup();
    int IRStatus = digitalRead(IR_SENSOR_GPIO);
    return IRStatus;
}
