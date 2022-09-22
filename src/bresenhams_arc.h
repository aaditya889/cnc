#include <Arduino.h>
#include <math.h>



void draw_bresenham_arc(int start_x_steps, int start_y_steps, int end_x_steps, int end_y_steps, int center_x_steps, int center_y_steps, int radius_steps)
{
    wdt_disable();
    start_x_steps -= center_x_steps;
    start_y_steps -= center_y_steps;
    end_x_steps -= center_x_steps;
    end_y_steps -= center_y_steps;
    int current_x_steps = start_x_steps;
    int current_y_steps = start_y_steps;
    int *independent_dim, *dependent_dim, independent_dim_increment_step, dependent_dim_increment_step;
    int x_increment_step, y_increment_step;
    int previous_x_steps, previous_y_steps;
    int decision_factor;

    while (current_x_steps != end_x_steps || current_y_steps != end_y_steps)
    {
        float current_slope = atan2((double)((current_y_steps - center_y_steps)), (double)((current_x_steps - center_x_steps)));
        int starting_octant = -1;
        if (current_slope <= 0 && current_slope > -(float)PI/4.0) starting_octant = 0;
        else if (current_slope <= -(float)PI/4.0 && current_slope > -(float)PI/2.0) starting_octant = 1;
        else if (current_slope <= -(float)PI/2.0 && current_slope > -(float)3.0*PI/4.0) starting_octant = 2;
        else if (current_slope <= -(float)3.0*PI/4.0 && current_slope > -(float)PI) starting_octant = 3;
        else if (current_slope <= (float)PI && current_slope > (float)3.0*PI/4.0) starting_octant = 4;
        else if (current_slope <= (float)3.0*PI/4.0 && current_slope > (float)PI/2.0) starting_octant = 5;
        else if (current_slope <= (float)PI/2.0 && current_slope > (float)PI/4.0) starting_octant = 6;
        else if (current_slope <= (float)PI/4.0 && current_slope > 0) starting_octant = 7;

        switch (starting_octant)
        {
            case 0:
                independent_dim = &current_y_steps;
                dependent_dim = &current_x_steps;
                independent_dim_increment_step = -1;
                dependent_dim_increment_step = -1;
                decision_factor = 1;
                x_increment_step = -1;
                y_increment_step = -1;
                break;
            case 1:
                independent_dim = &current_x_steps;
                dependent_dim = &current_y_steps;
                independent_dim_increment_step = -1;
                dependent_dim_increment_step = -1;
                decision_factor = -1;
                x_increment_step = -1;
                y_increment_step = -1;
                break;
            case 2:
                independent_dim = &current_x_steps;
                dependent_dim = &current_y_steps;
                independent_dim_increment_step = -1;
                dependent_dim_increment_step = 1;
                decision_factor = 1;
                x_increment_step = -1;
                y_increment_step = 1;
                break;
            case 3:
                independent_dim = &current_y_steps;
                dependent_dim = &current_x_steps;
                independent_dim_increment_step = 1;
                dependent_dim_increment_step = -1;
                decision_factor = -1;
                x_increment_step = -1;
                y_increment_step = 1;
                break;
            case 4:
                independent_dim = &current_y_steps;
                dependent_dim = &current_x_steps;
                independent_dim_increment_step = 1;
                dependent_dim_increment_step = 1;
                decision_factor = 1;
                x_increment_step = 1;
                y_increment_step = 1;
                break;
            case 5:
                independent_dim = &current_x_steps;
                dependent_dim = &current_y_steps;
                independent_dim_increment_step = 1;
                dependent_dim_increment_step = 1;
                decision_factor = -1;
                x_increment_step = 1;
                y_increment_step = 1;
                break;
            case 6:
                independent_dim = &current_x_steps;
                dependent_dim = &current_y_steps;
                independent_dim_increment_step = 1;
                dependent_dim_increment_step = -1;
                decision_factor = 1;
                x_increment_step = 1;
                y_increment_step = -1;
                break;
            case 7:
                independent_dim = &current_y_steps;
                dependent_dim = &current_x_steps;
                independent_dim_increment_step = -1;
                dependent_dim_increment_step = 1;
                decision_factor = -1;
                x_increment_step = 1;
                y_increment_step = -1;
                break;
        }

        set_stepper_pin_directions(x_increment_step, y_increment_step);

        float decision = 2*((float)(sq((float)(*independent_dim) + (independent_dim_increment_step)))) + (float)(sq((float)(*dependent_dim))) + (float)(sq((float)(*dependent_dim) + (dependent_dim_increment_step))) - 2*((float)sq(radius_steps)); 
        
        // Serial.println("D7!");
        // Serial.print("DEBUG (radius, decision, start_x, start_y, x_increment, y_increment, current_slope, independent, dependent, decision factor, ((float)PI/2.0 - abs(current_slope))), decision(if):: ");
        // Serial.print(radius_steps);
        // Serial.print(" ");
        // Serial.print(decision);
        // Serial.print(" ");
        // Serial.print(current_x_steps);
        // Serial.print(" ");
        // Serial.print(current_y_steps);
        // Serial.print(" ");
        // Serial.print(x_increment_step);
        // Serial.print(" ");
        // Serial.print(y_increment_step);
        // Serial.print(" ");
        // Serial.print(current_slope);
        // Serial.print(" ");
        // Serial.print(*independent_dim);
        // Serial.print(" ");
        // Serial.print(*dependent_dim);
        // Serial.print(" ");
        // Serial.print(decision_factor);
        // Serial.print(" ");
        // Serial.print(((float)PI/2.0 - abs(current_slope)));
        // Serial.print(" ");
        // Serial.println(((float)PI/2.0 - abs(current_slope)) < (float)PI/4.0);

        while ((abs(*independent_dim) <= abs(*dependent_dim)))
        {
            previous_x_steps = current_x_steps;
            previous_y_steps = current_y_steps;

            // Serial.println("D8!");
            // Serial.print("DEBUG (radius, decision):: ");
            // Serial.print(radius_steps);
            // Serial.print(" ");
            // Serial.println(decision);

            if ((decision_factor*decision) >= 0) 
            {
                decision = decision + (4 * ((independent_dim_increment_step*(*independent_dim)) + (dependent_dim_increment_step*(*dependent_dim)))) + 10;
                (*dependent_dim) += (dependent_dim_increment_step);
            }
            else 
            {
                decision = decision + (4*(independent_dim_increment_step*(*independent_dim))) + 6;
            }
            (*independent_dim) += (independent_dim_increment_step);

            run_stepper_breesenham((current_x_steps-previous_x_steps), (current_y_steps-previous_y_steps));

            // Serial.print("X, Y:: ");
            // Serial.print(current_x_steps);
            // Serial.print(" ");
            // Serial.println(current_y_steps);

            if ((abs(*dependent_dim) == 0) || (abs(*independent_dim) == 0)) break;
            if (current_x_steps == end_x_steps && current_y_steps == end_y_steps) break;
        }
    }
    wdt_enable(1000);
}


void draw_circle_breesenham(int radius_steps)
{   
    radius_steps = 1000;
    int start_x = 0, start_y = radius_steps, end_x = 0, end_y = -radius_steps, center_x = 0, center_y = 0;
    Serial.println("D0!");
    Serial.print("DEBUG (radius):: ");
    Serial.print(radius_steps);
    Serial.print(" ");
    // Serial.println(decision);
    draw_bresenham_arc(start_x, start_y, end_x, end_y, center_x, center_y, radius_steps);
    draw_bresenham_arc(end_x, end_y, start_x, start_y, center_x, center_y, radius_steps);
}
