#include <wireless_operations.h>
#include "constants.h"
#include "utils.h"
#include "stepper_functions.h"
#include "z_axis.h"
#include "bresenhams_arc.h"
#include "line_functions.h"
#include "arc_functions.h"
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
    initialise_z_axis();
    initialise_queue_memory();
}

void test_axes_directions()
{   
    int max_steps = 1000;
    // X AXIS
    Serial.printf("Running +X AXIS for %d steps\n", max_steps);
    engage_z_axis();
    run_stepper_directly(max_steps, 0);
    delay(1000);
    Serial.printf("Running -X AXIS for %d steps\n", max_steps);
    run_stepper_directly(-max_steps, 0);
    disengage_z_axis();
    delay(2000);

    // Y AXIS
    Serial.printf("Running +Y AXIS for %d steps\n", max_steps);
    engage_z_axis();
    run_stepper_directly(0, max_steps);
    delay(1000);
    Serial.printf("Running -Y AXIS for %d steps\n", max_steps);
    run_stepper_directly(0, -max_steps);
    disengage_z_axis();
    delay(2000);

    // BOTH SAME DIRECTION
    Serial.printf("Running both axes in the +same direction for %d steps\n", max_steps);
    engage_z_axis();
    run_stepper_directly(max_steps, max_steps);
    delay(1000);
    Serial.printf("Running both axes in the -same direction for %d steps\n", max_steps);
    run_stepper_directly(-max_steps, -max_steps);
    disengage_z_axis();
    delay(2000);

    // BOTH OPPOSITE DIRECTION
    
    Serial.printf("Running both axes in the +opposite direction for %d steps\n", max_steps);
    engage_z_axis();
    run_stepper_directly(max_steps, -max_steps);
    delay(1000);
    Serial.printf("Running both axes in the -opposite direction for %d steps\n", max_steps);
    run_stepper_directly(-max_steps, max_steps);
    disengage_z_axis();
}


void loop() {
    while (1)
    {
        // delay(3000);
        // test_axes_directions();
        read_and_execute_gcode_lines();
        // SERVER.cleanupClients();
        // if (DEBUG_SWITCHES[VERBOSE_LOGGING_IDX]) Serial.printf("Verbose printing is ON!\n");
        yield();
        // Serial.printf("Heap Stats: %ld\n", ESP.getHeapStats());
        // engage_z_axis();
        // delay(5000);
        // disengage_z_axis();
        // delay(5000);
    }
}
