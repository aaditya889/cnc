#include <Arduino.h>
#include <math.h>


double positive_modf(double x, double y)
{
    if (x>0)
    while (x>=y) x -= y;
    else
    while (x<0) x += y;

    return x;
}

void initialise_queue_memory()
{   
    // Serial.printf("ASSIGNING MEMORY...\n");
    // for (int i = 0; i < MAX_GCODE_LINES; i++)
    // {
    //     GCODE_LINES[i] = (char *)malloc(30);
    // }
    // Serial.printf("MEMORY ASSIGNED!\n");
    ;
}