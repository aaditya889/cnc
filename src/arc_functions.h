#include <Arduino.h>
#include <math.h>



// void draw_arc_steps(int start_x_steps, int start_y_steps, int end_x_steps, int end_y_steps, int center_x_steps, int center_y_steps)
// {

//     // Serial.print("centerx_steps, center_y_steps, startx_steps ,starty_steps, endx_steps, endy_steps:: ");
//     // Serial.print(" ");
//     // Serial.print(center_x_steps);
//     // Serial.print(" ");
//     // Serial.print(center_y_steps);
//     // Serial.print(" ");
//     // Serial.print(start_x_steps);
//     // Serial.print(" ");
//     // Serial.print(start_y_steps);
//     // Serial.print(" ");
//     // Serial.print(end_x_steps);
//     // Serial.print(" ");
//     // Serial.print(end_y_steps);
//     // Serial.println(" ");
//     // Serial.println("Drawing an arc...");
//     float curr_x_steps = start_x_steps, curr_y_steps = start_y_steps;
//     float previous_x_steps = start_x_steps, previous_y_steps = start_y_steps;
//     float radius_steps_start = sqrtf((sq(center_x_steps-start_x_steps)) + (sq(center_y_steps-start_y_steps)));
//     float radius_steps_end = sqrtf((sq(center_x_steps-end_x_steps)) + (sq(center_y_steps-end_y_steps)));
//     int radius_steps = (radius_steps_start + radius_steps_end)/2;
//     // Serial.print("Radius:: ");
//     // Serial.println(radius_steps);

//     // find the quadrant of the starting point
//     float quadrant = atan2((start_y_steps-center_y_steps), (start_x_steps-center_x_steps));

//     int add_step_x = (start_x_steps <= end_x_steps) ? 1 : -1;
//     int y_factor = (quadrant >= 0.0 && quadrant < PI) ? 1 : -1;
//     float sqrt_equation;

//     while (curr_x_steps != end_x_steps)
//     {
//         curr_x_steps += add_step_x;
//         sqrt_equation = (sq(radius_steps) - sq(curr_x_steps - center_x_steps));
//         // Circle equation
//         if (sqrt_equation >= 0) curr_y_steps = center_y_steps + ((float)y_factor * sqrtf(sqrt_equation));
//         else curr_y_steps = end_y_steps;

//         draw_line_steps(previous_x_steps, previous_y_steps, (int)curr_x_steps, (int)curr_y_steps);
//         // Serial.print(curr_x_steps);
//         // Serial.print(" ");
//         // Serial.println(curr_y_steps);
//         previous_x_steps = curr_x_steps;
//         previous_y_steps = curr_y_steps;
//     }
// }


void draw_arc_ijk_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm, float center_x_mm, float center_y_mm)
{
    int start_x_steps = ceil((float)start_x_mm/STEP_QUANTA_MM);
    int start_y_steps = ceil((float)start_y_mm/STEP_QUANTA_MM);
    int end_x_steps = ceil((float)end_x_mm/STEP_QUANTA_MM);
    int end_y_steps = ceil((float)end_y_mm/STEP_QUANTA_MM);
    int center_x_steps = ceil((float)center_x_mm/STEP_QUANTA_MM); 
    int center_y_steps = ceil((float)center_y_mm/STEP_QUANTA_MM);

    float radius_steps_start = sqrtf((sq(center_x_steps-start_x_steps)) + (sq(center_y_steps-start_y_steps)));
    float radius_steps_end = sqrtf((sq(center_x_steps-end_x_steps)) + (sq(center_y_steps-end_y_steps)));
    int radius_steps = (radius_steps_start + radius_steps_end)/2;
    
    Serial.print("Drawing arc (steps):: x1, y1, x2, y2, cx, cy:: ");
    Serial.print(start_x_steps);
    Serial.print(" ");
    Serial.print(start_y_steps);
    Serial.print(" ");
    Serial.print(end_x_steps);
    Serial.print(" ");
    Serial.print(end_y_steps);
    Serial.print(" ");
    Serial.print(center_x_steps);
    Serial.print(" ");
    Serial.print(center_y_steps);
    Serial.println(" ");
    draw_bresenham_arc(start_x_steps, start_y_steps, end_x_steps, end_y_steps, center_x_steps, center_y_steps, radius_steps);
    Serial.println("DONE!");
}


void draw_arc_radius_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm, float radius_mm)
{
    float m, cl, A, B, C, center_x_mm, center_y_mm, center_x_mm_2, center_y_mm_2;
    if (start_x_mm == INT_MIN) start_x_mm = STEPPER_X.currentPosition()*STEP_QUANTA_MM;
    if (start_y_mm == INT_MIN) start_y_mm = STEPPER_Y.currentPosition()*STEP_QUANTA_MM;

    m = (float)(start_x_mm-end_x_mm)/(float)(end_y_mm-start_y_mm);
    cl = ((float)((end_y_mm*end_y_mm)-(start_y_mm*start_y_mm) + (end_x_mm*end_x_mm)-(start_x_mm*start_x_mm))/(float)(2*(end_y_mm-start_y_mm)));
    A = 1 + (float)(m*m);
    B = (float)(2*m*cl) - (float)(2*m*start_y_mm)-(2*start_x_mm);
    C = (float)(start_x_mm*start_x_mm)+(start_y_mm*start_y_mm)+(cl*cl)-(2*cl*start_y_mm)-(radius_mm*radius_mm);
    center_x_mm = (-B + (float)sqrt((float)(B*B) - (float)(4*A*C)))/(float)(2*A);
    center_x_mm_2 = (-B - (float)sqrt((float)(B*B) - (float)(4*A*C)))/(float)(2*A);

    center_y_mm = (float)m*center_x_mm + (float)cl;
    center_y_mm_2 = (float)m*center_x_mm_2 + (float)cl;

    // Serial.print("Drawing arc (mm):: x1, y1, x2, y2, cx, cy:: ");
    // Serial.print(start_x_mm);
    // Serial.print(" ");
    // Serial.print(start_y_mm);
    // Serial.print(" ");
    // Serial.print(end_x_mm);
    // Serial.print(" ");
    // Serial.print(end_y_mm);
    // Serial.print(" ");
    // Serial.print(center_x_mm);
    // Serial.print(" ");
    // Serial.print(center_y_mm);
    // Serial.println(" ");
    draw_arc_ijk_mm(start_x_mm, start_y_mm, end_x_mm, end_y_mm, center_x_mm, center_y_mm);
}
