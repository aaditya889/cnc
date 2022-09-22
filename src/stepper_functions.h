#include <Arduino.h>


void initialise_stepper_objects()
{
    STEPPER_X.setCurrentPosition(0);
    STEPPER_Y.setCurrentPosition(0);
}

void set_stepper_pin_directions(int x_direction, int y_direction)
{
    // Serial.println("D5.1");
    // Serial.print("Current X, Y Positions:: ");
    // Serial.print(STEPPER_X.currentPosition());
    // Serial.print(" ");
    // Serial.print(STEPPER_Y.currentPosition());
    // Serial.println(" ");

    // Serial.print("Going to (X, Y):: ");
    // Serial.print(x_direction);
    // Serial.print(" ");
    // Serial.print(y_direction);
    // Serial.println(" ");

    if (x_direction < 0) digitalWrite(DIRECTION_PIN_1, LOW);
    else digitalWrite(DIRECTION_PIN_1, HIGH);

    if (y_direction < 0) digitalWrite(DIRECTION_PIN_2, LOW);
    else digitalWrite(DIRECTION_PIN_2, HIGH);
}


void run_stepper_breesenham(int x_steps, int y_steps)
{
    // Serial.print("Current X, Y positions:: ");
    // Serial.print(STEPPER_X.currentPosition());
    // Serial.print(" ");
    // Serial.print(STEPPER_Y.currentPosition());
    // Serial.println(" ");

    STEPPER_X.setCurrentPosition(STEPPER_X.currentPosition() + x_steps);
    STEPPER_Y.setCurrentPosition(STEPPER_Y.currentPosition() + y_steps);

    // Serial.print("Going to (X, Y):: ");
    // Serial.print(STEPPER_X.currentPosition());
    // Serial.print(" ");
    // Serial.print(STEPPER_Y.currentPosition());
    // Serial.println(" ");

    x_steps = abs(x_steps);
    y_steps = abs(y_steps);

    if (x_steps == 1 && y_steps == 1)
    {
        digitalWrite(STEP_PIN_1, HIGH);
        digitalWrite(STEP_PIN_2, HIGH);
        delayMicroseconds(150);
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        delayMicroseconds(150);
    }
    else
    {
        if (x_steps == 1) 
        {
            digitalWrite(STEP_PIN_1, HIGH);
            delayMicroseconds(150);
            digitalWrite(STEP_PIN_1, LOW);
            delayMicroseconds(150);
        }
        if (y_steps == 1) 
        {
            digitalWrite(STEP_PIN_2, HIGH);
            delayMicroseconds(150);
            digitalWrite(STEP_PIN_2, LOW);
            delayMicroseconds(150);
        }
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
        // Serial.println("D4!");
        // Serial.print("x_steps, y_steps:: ");
        // Serial.print(x_steps);
        // Serial.print(" ");
        // Serial.println(y_steps);

        if (x_steps > 0) 
        {
            digitalWrite(STEP_PIN_1, HIGH);
            delayMicroseconds(100);
            digitalWrite(STEP_PIN_1, LOW);
            delayMicroseconds(100);
            x_steps--;
        }
        if (y_steps > 0) 
        {
            digitalWrite(STEP_PIN_2, HIGH);
            delayMicroseconds(100);
            digitalWrite(STEP_PIN_2, LOW);
            delayMicroseconds(100);
            y_steps--;
        }
    }
}