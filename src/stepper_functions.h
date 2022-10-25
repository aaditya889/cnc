#include <Arduino.h>

const int STEPPER_MOTOR_VELOCITY_NUMERATOR = 10000;

void initialise_stepper_objects()
{
    STEPPER_X.setCurrentPosition(0);
    STEPPER_Y.setCurrentPosition(0);
}

void set_stepper_pin_directions(int x_direction, int y_direction)
{
    short positive_x_direction = HIGH;
    short positive_y_direction = HIGH; 
    // Serial.printf("Got args: x_steps: %d and y_steps: %d\n", x_direction, y_direction);
    if (x_direction < 0) 
    {
        // Serial.printf("Setting NOT positive_x_direction\n");
        digitalWrite(DIRECTION_PIN_1, !positive_x_direction);
    }
    else 
    {
        // Serial.printf("Setting positive_x_direction\n");
        digitalWrite(DIRECTION_PIN_1, positive_x_direction);
    }

    if (y_direction < 0) 
    {
        // Serial.printf("Setting NOT positive_y_direction\n");
        digitalWrite(DIRECTION_PIN_2, !positive_y_direction);
    }
    else 
    {
        // Serial.printf("Setting positive_y_direction\n");
        digitalWrite(DIRECTION_PIN_2, positive_y_direction);
    }
}


void run_stepper_breesenham(int x_steps, int y_steps)
{
    STEPPER_X.setCurrentPosition(STEPPER_X.currentPosition() + x_steps);
    STEPPER_Y.setCurrentPosition(STEPPER_Y.currentPosition() + y_steps);
    x_steps = abs(x_steps);
    y_steps = abs(y_steps);

    if (x_steps == 1 && y_steps == 1)
    {
        digitalWrite(STEP_PIN_1, HIGH);
        digitalWrite(STEP_PIN_2, HIGH);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
    }
    else
    {
        if (x_steps == 1) 
        {
            digitalWrite(STEP_PIN_1, HIGH);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
            digitalWrite(STEP_PIN_1, LOW);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        }
        if (y_steps == 1) 
        {
            digitalWrite(STEP_PIN_2, HIGH);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
            digitalWrite(STEP_PIN_2, LOW);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        }
    } 
}


void run_stepper_directly(int x_steps, int y_steps)
{
    // Serial.printf("Running Steppers directly (%d, %d)...\n", x_steps, y_steps);
    set_stepper_pin_directions(x_steps, y_steps);
    STEPPER_X.setCurrentPosition(STEPPER_X.currentPosition() + x_steps);
    STEPPER_Y.setCurrentPosition(STEPPER_Y.currentPosition() + y_steps);
    x_steps = abs(x_steps);
    y_steps = abs(y_steps);

    while (x_steps-- > 0 && y_steps-- > 0)
    {
        // Serial.printf("1:: X_Steps: %d and Y_Steps: %d\n", x_steps, y_steps);
        digitalWrite(STEP_PIN_1, HIGH);
        digitalWrite(STEP_PIN_2, HIGH);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        yield();
    }

    while (x_steps-- > 0)
    {
        // Serial.printf("2:: X_Steps: %d and Y_Steps: %d\n", x_steps, y_steps);
        digitalWrite(STEP_PIN_1, HIGH);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        digitalWrite(STEP_PIN_1, LOW);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        yield();
    }

    while(y_steps-- > 0)
    {
        // Serial.printf("3:: X_Steps: %d and Y_Steps: %d\n", x_steps, y_steps);
        digitalWrite(STEP_PIN_2, HIGH);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        digitalWrite(STEP_PIN_2, LOW);
        delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
        yield();
    }
}


void run_stepper_for_arc_old(int x_steps, int y_steps)
{
    STEPPER_X.setCurrentPosition(STEPPER_X.currentPosition() + x_steps);
    STEPPER_Y.setCurrentPosition(STEPPER_Y.currentPosition() + y_steps);

    x_steps = abs(x_steps);
    y_steps = abs(y_steps);

    while (x_steps != 0 || y_steps != 0)
    {

        if (x_steps > 0) 
        {
            digitalWrite(STEP_PIN_1, HIGH);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
            digitalWrite(STEP_PIN_1, LOW);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
            x_steps--;
        }
        if (y_steps > 0) 
        {
            digitalWrite(STEP_PIN_2, HIGH);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
            digitalWrite(STEP_PIN_2, LOW);
            delayMicroseconds(STEPPER_MOTOR_VELOCITY_NUMERATOR/STEPPER_VELOCITY);
            y_steps--;
        }
    }
}