#include <Arduino.h>
#include <math.h>
#include "bresenhams_line.h"


void run_line_stepper(float x_multiplier, float y_multiplier, int stepsx, int stepsy)
{
    int base_speed = 5000, max_speed = 5000;
    float x_speed = ceil((float)base_speed * (float)x_multiplier);
    float y_speed = ceil((float)base_speed * (float)y_multiplier);

    STEPPER_X.setMaxSpeed(max_speed);
    STEPPER_X.moveTo(STEPPER_X.currentPosition() + stepsx);
    STEPPER_X.setSpeed(x_speed);
	// STEPPER_X.setAcceleration(500);
	// STEPPER_X.moveTo(stepsx);

    STEPPER_Y.setMaxSpeed(max_speed);
    STEPPER_Y.moveTo(STEPPER_Y.currentPosition() + stepsy);
    STEPPER_Y.setSpeed(y_speed);
	// STEPPER_Y.setAcceleration(500);

    // Serial.print("Setting speed (x, y): ");
    // Serial.print(x_speed);
    // Serial.print(" ");
    // Serial.println(y_speed);

    // while (1)
    // {
    //     delay(1);
    // }
    // int steps = 0;
    // wdt_disable();
    while ((STEPPER_X.distanceToGo() != 0) || (STEPPER_Y.distanceToGo() != 0))
    {
        // Serial.print("Inside!");
        // Serial.print("Current X: ");
        // Serial.print(STEPPER_X.currentPosition());
        // Serial.print(" ");
        // Serial.print("To Go X: ");
        // Serial.print(STEPPER_X.distanceToGo());
        // Serial.println(" ");

        // Serial.print("Current Y: ");
        // Serial.print(STEPPER_Y.currentPosition());
        // Serial.print(" ");
        // Serial.print("To Go Y: ");
        // Serial.print(STEPPER_Y.distanceToGo());
        // Serial.println(" ");

        // Serial.println(STEPPER_X.distanceToGo());
        // Serial.println(STEPPER_Y.distanceToGo());

        if (STEPPER_X.distanceToGo() != 0) STEPPER_X.runSpeed();
        if (STEPPER_Y.distanceToGo() != 0) STEPPER_Y.runSpeed();
        // delay(1);
        
    }
    // wdt_enable(1);
    // Serial.print("Steps moved: ");
    // Serial.println(steps);
}


void draw_line_steps(int start_x_steps, int start_y_steps, int end_x_steps, int end_y_steps)
{
    // Serial.println("INSIDE DRAW LINE!");
    delay(1);
    int run_x_steps = end_x_steps - start_x_steps;
    int run_y_steps = end_y_steps - start_y_steps;

    if (run_x_steps < X_STEPS_LOW_LIMIT || run_x_steps > X_STEPS_HIGH_LIMIT) 
    while(1)  
    {
        Serial.print("ERROR in x steps:: run_x_steps, start_x_steps, start_y_steps, end_x_steps, end_y_steps:: ");
        Serial.print(run_x_steps);
        Serial.print(" ");
        Serial.print(start_x_steps);
        Serial.print(" ");
        Serial.print(start_y_steps);
        Serial.print(" ");
        Serial.print(end_x_steps);
        Serial.print(" ");
        Serial.print(end_y_steps);
        Serial.println(" ");
        Serial.println("ERROR!");
        delay(10000);
    }
    if (run_y_steps < Y_STEPS_LOW_LIMIT || run_y_steps > Y_STEPS_HIGH_LIMIT) 
    while(1)  
    {
        Serial.println("ERROR in y steps:: run_x_steps, start_x_steps, start_y_steps, end_x_steps, end_y_steps:: ");
        Serial.print(run_y_steps);
        Serial.print(" ");
        Serial.print(start_x_steps);
        Serial.print(" ");
        Serial.print(start_y_steps);
        Serial.print(" ");
        Serial.print(end_x_steps);
        Serial.print(" ");
        Serial.print(end_y_steps);
        Serial.println(" ");
        Serial.println("ERROR!");
        delay(10000);
    }

    float x_multiplier = 1.0;
    float y_multiplier = 1.0;
    float slope = ((float)(end_y_steps - start_y_steps))/((float)(end_x_steps - start_x_steps));
    
    if (abs(slope) < 1.0) 
    {
        y_multiplier = abs(slope);
    }
    else 
    {
        x_multiplier = abs(1.0/slope);
    }
    if (end_y_steps < start_y_steps) y_multiplier *= -1;
    if (end_x_steps < start_x_steps) x_multiplier *= -1;
    
    // int stepsx = ((end_x_steps - start_x_steps)/STEP_QUANTA_MM);
    // int stepsy = ((end_y - start_y)/STEP_QUANTA_MM);
    // Serial.print("stx, sty, enx, eny, slope, x_mult, y_mult:: ");
    // Serial.print(start_x_steps);
    // Serial.print(" ");
    // Serial.print(start_y_steps);
    // Serial.print(" ");
    // Serial.print(end_x_steps);
    // Serial.print(" ");
    // Serial.print(end_y_steps);
    // Serial.print(" ");
    // Serial.print(slope);
    // Serial.print(" ");
    // Serial.print(x_multiplier);
    // Serial.print(" ");
    // Serial.println(y_multiplier);


    // Serial.print("Current X and Y Steps:");
    // Serial.print(STEPPER_X.currentPosition());
    // Serial.print(" ");
    // Serial.println(STEPPER_Y.currentPosition());

    // Serial.print("Steps to move (X & Y): ");
    // Serial.print(end_x_steps - start_x_steps);
    // Serial.print(" ");
    // Serial.println(end_y_steps - start_y_steps);
    // Serial.println("slope, x_multiplier, y_multiplier, startx ,starty, endx, endy");
    // Serial.println(slope);
    // Serial.println(x_multiplier);
    // Serial.println(y_multiplier);
    // Serial.println(start_x);
    // Serial.println(start_y);
    // Serial.println(end_x);
    // Serial.println(end_y);
    // Serial.print("Stepsx and y: ");
    // Serial.println(stepsx);
    // Serial.println(stepsy);
    
    run_line_stepper(x_multiplier, y_multiplier, end_x_steps - start_x_steps, end_y_steps - start_y_steps);
}


void draw_line_mm(int start_x_mm, int start_y_mm, int end_x_mm, int end_y_mm)
{
    int start_x_steps, start_y_steps, end_x_steps, end_y_steps;
    if (start_x_mm == INT_MIN) start_x_steps = STEPPER_X.currentPosition();
    else start_x_steps = start_x_mm/STEP_QUANTA_MM;
    if (start_y_mm == INT_MIN) start_y_steps = STEPPER_Y.currentPosition();
    else start_y_steps = start_y_mm/STEP_QUANTA_MM;

    end_x_steps = end_x_mm/STEP_QUANTA_MM;
    end_y_steps = end_y_mm/STEP_QUANTA_MM;
    Serial.print("Drawing line (steps):: x1, y1, x2, y2:: ");
    Serial.print(start_x_steps);
    Serial.print(" ");
    Serial.print(start_y_steps);
    Serial.print(" ");
    Serial.print(end_x_steps);
    Serial.print(" ");
    Serial.print(end_y_steps);
    Serial.println(" ");
    
    draw_line_steps_2(start_x_steps, start_y_steps, end_x_steps, end_y_steps);
    Serial.println("DONE!");
}
