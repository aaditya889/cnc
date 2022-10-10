#include <Arduino.h>
#include <math.h>



void draw_arc_ijk_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm, float center_x_mm, float center_y_mm)
{
    int start_x_steps = ((float)start_x_mm/STEP_QUANTA_MM);
    int start_y_steps = ((float)start_y_mm/STEP_QUANTA_MM);
    int end_x_steps = ((float)end_x_mm/STEP_QUANTA_MM);
    int end_y_steps = ((float)end_y_mm/STEP_QUANTA_MM);
    int center_x_steps = ((float)center_x_mm/STEP_QUANTA_MM); 
    int center_y_steps = ((float)center_y_mm/STEP_QUANTA_MM);

    float radius_steps_start = sqrtf((sq(center_x_steps-start_x_steps)) + (sq(center_y_steps-start_y_steps)));
    float radius_steps_end = sqrtf((sq(center_x_steps-end_x_steps)) + (sq(center_y_steps-end_y_steps)));
    int radius_steps = (radius_steps_start + radius_steps_end)/2;

    Serial.printf("Going from (%d, %d) -> (%d, %d), with center at (%d, %d) and radius = (%d)...\n", start_x_steps, start_y_steps, end_x_steps, end_y_steps, center_x_steps, center_y_steps, radius_steps);
    PRINT_EQUAL_BREAK;

    draw_bresenham_arc(start_x_steps, start_y_steps, end_x_steps, end_y_steps, center_x_steps, center_y_steps, radius_steps);
    Serial.printf("DONE!\n");
}


void draw_arc_radius_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm, float radius_mm)
{
    float m, cl, A, B, C, center_x_mm, center_y_mm, center_x_mm_2, center_y_mm_2;
    float xc, yc;
    if (start_x_mm == INT_MIN) start_x_mm = STEPPER_X.currentPosition()*STEP_QUANTA_MM;
    if (start_y_mm == INT_MIN) start_y_mm = STEPPER_Y.currentPosition()*STEP_QUANTA_MM;

    xc = (float)(start_x_mm + end_x_mm)/2.0;
    yc = (float)(start_y_mm + end_y_mm)/2.0;

    if ((start_y_mm == end_y_mm) || (float)(start_x_mm-end_x_mm)/(float)(end_y_mm-start_y_mm) > 100000) 
    {
        center_x_mm = center_x_mm_2 = xc;
        center_y_mm = (float)yc - (float)sqrtf((float)(radius_mm*radius_mm) - ((float)(xc-start_x_mm)*(xc-start_x_mm)));
        center_y_mm_2 = (float)yc + (float)sqrtf((float)(radius_mm*radius_mm) - ((float)(xc-start_x_mm)*(xc-start_x_mm)));
        m = cl = A = B = C = 0.0;
    } 
    else
    {
        m = (float)(start_x_mm-end_x_mm)/(float)(end_y_mm-start_y_mm);
        cl = ((float)((end_y_mm*end_y_mm)-(start_y_mm*start_y_mm) + (end_x_mm*end_x_mm)-(start_x_mm*start_x_mm))/(float)(2*(end_y_mm-start_y_mm)));
        A = 1 + (float)(m*m);
        B = (float)(2*m*cl) - (float)(2*m*start_y_mm)-(2*start_x_mm);
        C = (float)(start_x_mm*start_x_mm)+(start_y_mm*start_y_mm)+(cl*cl)-(2*cl*start_y_mm)-(radius_mm*radius_mm);
        center_x_mm = (-B + (float)sqrt((float)(B*B) - (float)(4*A*C)))/(float)(2*A);
        center_x_mm_2 = (-B - (float)sqrt((float)(B*B) - (float)(4*A*C)))/(float)(2*A);

        center_y_mm = (float)m*center_x_mm + (float)cl;
        center_y_mm_2 = (float)m*center_x_mm_2 + (float)cl;
    }

    PRINT_EQUAL_BREAK;
    Serial.printf("In steps:: Current position: (%ld, %ld)", STEPPER_X.currentPosition(), STEPPER_Y.currentPosition());
    Serial.printf("In millimeters:: (%4f, %4f) -> (%4f, %4f), radius = %4f, m = %4f, cl = %4f, A = %4f, B = %4f, C = %4f, c1 = (%4f, %4f), c2 = (%4f, %4f)\n", start_x_mm, start_y_mm, end_x_mm, end_y_mm, radius_mm, m, cl, A, B, C, center_x_mm, center_y_mm, center_x_mm_2, center_y_mm_2);
    draw_arc_ijk_mm(start_x_mm, start_y_mm, end_x_mm, end_y_mm, center_x_mm, center_y_mm);
}


void draw_circle(float radius)
{
    float center_x = radius;
    float center_y = 0.0;

    draw_arc_ijk_mm(0.0, 0.0, center_x*2, center_y*2, center_x, center_y);
    draw_arc_ijk_mm(center_x*2, center_y*2, 0.0, 0.0, center_x, center_y);
    
}
