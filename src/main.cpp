#include <wireless_operations.h>
#include "constants.h"
#include "stepper_functions.h"
#include "wireless_utils.h"
#include "stepper_temp.h"
#include "bresenhams_arc.h"


void setup() {
    Serial.begin(115200);
    connect_AP(SSID, PASSWORD);
    // Wire.begin(I2C_SDA, I2C_SCL);
    delay(200);	// Declare pins as Outputs
    init_server();
    pinMode(STEP_PIN_1, OUTPUT);
    pinMode(DIRECTION_PIN_1, OUTPUT);
    pinMode(STEP_PIN_2, OUTPUT);    
    pinMode(DIRECTION_PIN_2, OUTPUT);
    initialise_stepper_objects();
    // pinMode(enable_pin, OUTPUT);
    // pinMode(sleep_pin, OUTPUT);
    // pinMode(reset_pin, OUTPUT);

    // digitalWrite(enable_pin, LOW);
    // digitalWrite(sleep_pin, HIGH);
    // digitalWrite(reset_pin, HIGH);
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


void test_function()
{
    // run_stepper(4000, 100);
    draw_circle_breesenham(80);
}

void loop() {

    delay(2000);

    // while (1)
    // {
    //     read_and_execute_gcode_lines();
    //     delay(2000);
    // }

    // while (1)
    // {
    //     test_function();
    //     delay(2000);
    // }

    while (1)
    {
        delay(3000);
        draw_line_mm(0.0, 0.0, 0.0, 0.0);
        delay(3000);

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
