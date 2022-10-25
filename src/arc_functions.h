#include <Arduino.h>
#include <math.h>



void draw_arc_ijk_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm, float center_x_mm, float center_y_mm)
{
    // int start_x_steps = ((float)start_x_mm/STEP_QUANTA_MM);
    // int start_y_steps = ((float)start_y_mm/STEP_QUANTA_MM);
    int start_x_steps = STEPPER_X.currentPosition();
    int start_y_steps = STEPPER_Y.currentPosition();
    int end_x_steps = ((float)end_x_mm/STEP_QUANTA_MM);
    int end_y_steps = ((float)end_y_mm/STEP_QUANTA_MM);
    int center_x_steps = ((float)center_x_mm/STEP_QUANTA_MM); 
    int center_y_steps = ((float)center_y_mm/STEP_QUANTA_MM);

    float radius_steps_start = sqrtf((sq(center_x_steps-start_x_steps)) + (sq(center_y_steps-start_y_steps)));
    float radius_steps_end = sqrtf((sq(center_x_steps-end_x_steps)) + (sq(center_y_steps-end_y_steps)));
    int radius_steps = (radius_steps_start + radius_steps_end)/2;

    PRINT_EQUAL_BREAK;
    Serial.printf("Current steps: (%ld, %ld)\n", STEPPER_X.currentPosition(), STEPPER_Y.currentPosition());
    Serial.printf("Going from (%d, %d) -> (%d, %d), with center at (%d, %d) and radius = (%d)...\n", start_x_steps, start_y_steps, end_x_steps, end_y_steps, center_x_steps, center_y_steps, radius_steps);

    draw_bresenham_arc(start_x_steps, start_y_steps, end_x_steps, end_y_steps, center_x_steps, center_y_steps, radius_steps);
    Serial.printf("Stopped at:: (%ld, %ld)\n", STEPPER_X.currentPosition(), STEPPER_Y.currentPosition());
    // Serial.printf("Moving to (using G0):: (%ld, %ld)\n", end_x_steps, end_y_steps);
    // disengage_z_axis();
    // draw_line_mm(INT_MIN, INT_MIN, end_x_mm, end_y_mm);
    Serial.printf("DONE!\n");
}


void draw_arc_radius_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm, float radius_mm)
{
    double m, cl, A, B, C, principal_root = INT_MIN, center_x_mm, center_y_mm, center_x_mm_2, center_y_mm_2, selected_center_x, selected_center_y;
    double xc, yc;
    double start_line_angle_1, start_line_angle_2, end_line_angle_1, end_line_angle_2, subtended_angle_1, subtended_angle_2;
    boolean select_longer = false;
    double t1, t2, t3, t4, t5;

    if (radius_mm < 0.0)
    {
        select_longer = true;
        radius_mm = abs(radius_mm);
    }
    
    if (start_x_mm == INT_MIN) start_x_mm = STEPPER_X.currentPosition()*STEP_QUANTA_MM;
    if (start_y_mm == INT_MIN) start_y_mm = STEPPER_Y.currentPosition()*STEP_QUANTA_MM;

    xc = (float)(start_x_mm + end_x_mm)/2.0;
    yc = (float)(start_y_mm + end_y_mm)/2.0;

    if ((start_y_mm == end_y_mm) || (float)(start_x_mm-end_x_mm)/(float)(end_y_mm-start_y_mm) > 100000) 
    {
        center_x_mm = center_x_mm_2 = xc;
        center_y_mm = (double)yc - (double)sqrtl((double)(radius_mm*radius_mm) - ((double)(xc-start_x_mm)*(xc-start_x_mm)));
        center_y_mm_2 = (double)yc + (float)sqrtl((double)(radius_mm*radius_mm) - ((double)(xc-start_x_mm)*(xc-start_x_mm)));
        m = cl = A = B = C = 0.0;
    } 
    else
    {
        m = (double)(start_x_mm-end_x_mm)/(double)(end_y_mm-start_y_mm);
        cl = ((double)((end_y_mm*end_y_mm)-(start_y_mm*start_y_mm) + (end_x_mm*end_x_mm)-(start_x_mm*start_x_mm))/(float)(2*(end_y_mm-start_y_mm)));
        A = 1 + (double)(m*m);
        B = (double)(2*m*cl) - (double)(2*m*start_y_mm)-(2*start_x_mm);
        C = (double)(start_x_mm*start_x_mm)+(start_y_mm*start_y_mm)+(cl*cl)-(2*cl*start_y_mm)-(radius_mm*radius_mm);
        principal_root = (double)(B*B) - (double)(4*A*C);
        center_x_mm = (-B + (double)sqrtl(principal_root))/(double)(2*A);
        center_x_mm_2 = (-B - (double)sqrtl(principal_root))/(double)(2*A);

        center_y_mm = (double)m*center_x_mm + (double)cl;
        center_y_mm_2 = (double)m*center_x_mm_2 + (double)cl;
    }

    // PRINT_EQUAL_BREAK;
    // Serial.printf("In steps:: Current position: (%ld, %ld)\n", STEPPER_X.currentPosition(), STEPPER_Y.currentPosition());

    t1 = (start_y_mm-center_y_mm);
    t2 = (start_x_mm-center_x_mm);
    t3 = (double)atan2((float)(start_y_mm-center_y_mm), (float)(start_x_mm-center_x_mm));
    t4 = positive_modf(atan2((start_y_mm-center_y_mm), (start_x_mm-center_x_mm)), (2.0*PI));
    t5 = positive_modf(atan2((start_y_mm-center_y_mm), (start_x_mm-center_x_mm)), (2.0*PI)) *180/PI;

    // Serial.printf("\nt1: %4f, t2: %4f, t3: %4f, t4: %4f, t5: %4f\n", t1, t2, t3, t4, t5);

    start_line_angle_1 = positive_modf(atan2l((start_y_mm-center_y_mm), (start_x_mm-center_x_mm)), (2.0*PI)) *180/PI;
    end_line_angle_1 = positive_modf(atan2l((end_y_mm-center_y_mm), (end_x_mm-center_x_mm)), (2.0*PI)) *180/PI;
    start_line_angle_2 = positive_modf(atan2l((start_y_mm-center_y_mm_2), (start_x_mm-center_x_mm_2)), (2.0*PI)) *180/PI;
    end_line_angle_2 = positive_modf(atan2l((end_y_mm-center_y_mm_2), (end_x_mm-center_x_mm_2)), (2.0*PI)) *180/PI;

    subtended_angle_1 = abs(end_line_angle_1 - start_line_angle_1);
    subtended_angle_2 = abs(end_line_angle_2 - start_line_angle_2);

    if (start_line_angle_1 > end_line_angle_1) subtended_angle_1 = 360.0 - subtended_angle_1;
    if (start_line_angle_2 > end_line_angle_2) subtended_angle_2 = 360.0 - subtended_angle_1;

    // Serial.printf("For line (%4f, %4f) -> (%4f, %4f), and line (%4f, %4f) -> (%4f, %4f):: start_angle = %4f, end_angle = %4f, subtended_angle = %4f\n", 
    // start_x_mm, start_y_mm, center_x_mm, center_y_mm, end_x_mm, end_y_mm, center_x_mm, center_y_mm, start_line_angle_1, end_line_angle_1, 
    // subtended_angle_1);
    // Serial.printf("For line (%4f, %4f) -> (%4f, %4f), and line (%4f, %4f) -> (%4f, %4f):: start_angle = %4f, end_angle = %4f, subtended_angle = %4f\n", 
    // start_x_mm, start_y_mm, center_x_mm_2, center_y_mm_2, end_x_mm, end_y_mm, center_x_mm_2, center_y_mm_2, start_line_angle_2, end_line_angle_2, 
    // subtended_angle_2);

    if (select_longer)
    {
        if (subtended_angle_1 > subtended_angle_2) 
        {
            selected_center_x = center_x_mm;
            selected_center_y = center_y_mm;
        }
        else
        {
            selected_center_x = center_x_mm_2;
            selected_center_y = center_y_mm_2;
        }
    }
    else
    {
        if (subtended_angle_1 < subtended_angle_2) 
        {
            selected_center_x = center_x_mm;
            selected_center_y = center_y_mm;
        }
        else
        {
            selected_center_x = center_x_mm_2;
            selected_center_y = center_y_mm_2;
        }
    }
    // Serial.printf("In millimeters:: (%4f, %4f) -> (%4f, %4f), radius = %4f, m = %4f, cl = %4f, A = %4f, B = %4f, C = %4f, \
    // principal_root = %4f, c1 = (%4f, %4f), c2 = (%4f, %4f), subtended_angles = (%4f, %4f), selected = (%4f, %4f)\n", 
    // start_x_mm, start_y_mm, end_x_mm, end_y_mm, radius_mm, m, cl, A, B, C, 
    // principal_root, center_x_mm, center_y_mm, center_x_mm_2, center_y_mm_2, subtended_angle_1, subtended_angle_2, selected_center_x, selected_center_y);


    draw_arc_ijk_mm(start_x_mm, start_y_mm, end_x_mm, end_y_mm, selected_center_x, selected_center_y);
}


void draw_circle(float radius)
{
    float center_x = radius;
    float center_y = 0.0;

    draw_arc_ijk_mm(0.0, 0.0, center_x*2, center_y*2, center_x, center_y);
    draw_arc_ijk_mm(center_x*2, center_y*2, 0.0, 0.0, center_x, center_y);
    
}
