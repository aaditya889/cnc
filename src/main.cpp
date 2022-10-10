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


void loop() {
    while (1)
    {
        read_and_execute_gcode_lines();
        // if (DEBUG_SWITCHES[VERBOSE_LOGGING_IDX]) Serial.printf("Verbose printing is ON!\n");
        yield();
    }
}
