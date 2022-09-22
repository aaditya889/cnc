#include <wireless_operations.h>
#include "constants.h"
#include "stepper_functions.h"
#include "bresenhams_arc.h"
#include "arc_functions.h"
#include "line_functions.h"
#include "gcode_interpreter.h"
#include "wireless_utils.h"



void setup() {
    Serial.begin(115200);
    connect_AP(SSID, PASSWORD);
    delay(200);
    init_server();
    pinMode(STEP_PIN_1, OUTPUT);
    pinMode(DIRECTION_PIN_1, OUTPUT);
    pinMode(STEP_PIN_2, OUTPUT);    
    pinMode(DIRECTION_PIN_2, OUTPUT);
    initialise_stepper_objects();

}


void draw_rectangle(float side_a, float side_b)
{
    
    draw_line_mm(0.0, 0.0, side_a, 0.0);
    delay(1000);
    draw_line_mm(side_a, 0.0, side_a, side_b);
    delay(1000);
    draw_line_mm(side_a, side_b, 0.0, side_b);
    delay(1000);
    draw_line_mm(0.0, side_b, 0.0, 0.0);
    delay(1000);
    draw_line_mm(0.0, 0.0, side_a, side_b);
    delay(1000);
    draw_line_mm(side_a, side_b, 0.0, 0.0);

}

void draw_circle(float radius)
{
    float center_x = radius;
    float center_y = 0.0;

    draw_arc_ijk_mm(0.0, 0.0, center_x*2, center_y*2, center_x, center_y);
    draw_arc_ijk_mm(center_x*2, center_y*2, 0.0, 0.0, center_x, center_y);
    
}


void loop() {

    delay(2000);

    while (1)
    {
        // delay(3000);
        draw_line_mm(0.0, 0.0, 0.0, 0.0);
        // delay(3000);

        draw_rectangle(10.0, 20.0);
        delay(2000);
        draw_rectangle(11.23, 12.46);
        delay(2000);
        draw_rectangle(15.0, 20.0);
        delay(2000);
        draw_circle(20.0);
        delay(2000);
        draw_circle(21.23);
        delay(2000);
        draw_circle(22.46);
        delay(2000);
        draw_circle(25.0);
    }
}
