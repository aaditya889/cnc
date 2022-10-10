#include <Arduino.h>
#include <math.h> 


void plot_point(int x1, int y1, int x2, int y2, int dx, int dy, int decide)
{
    //pk is initial decision making parameter
    //Note:x1&y1,x2&y2, dx&dy values are interchanged
    //and passed in plot_point function so
    //it can handle both cases when m>1 & m<1
    int pk = 2 * dy - dx;
    int plot_x, plot_y;
    if (!decide)
    {
        plot_x = x1;
        plot_y = y1;
        set_stepper_pin_directions((x2 - STEPPER_X.currentPosition()), (y2 - STEPPER_Y.currentPosition()));
    }
    else 
    {
        plot_x = y1;
        plot_y = x1;
        set_stepper_pin_directions((y2 - STEPPER_X.currentPosition()), (x2 - STEPPER_Y.currentPosition()));
    }
    for (int i = 0; i <= dx; i++)
    {
        yield();
        if (!decide)
        {
          plot_x = x1;
          plot_y = y1;
        }
        else 
        {
          plot_x = y1;
          plot_y = x1;
        }
        run_stepper_breesenham((plot_x - STEPPER_X.currentPosition()), (plot_y - STEPPER_Y.currentPosition()));
        //checking either to decrement or increment the value
        //if we have to plot from (0,100) to (100,0)
        x1 < x2 ? x1++ : x1--;
        if (pk < 0)
        {
            //decision value will decide to plot
            //either  x1 or y1 in x's position 
            if (decide == 0)
            {
               // putpixel(x1, y1, RED);
                pk = pk + 2 * dy;
            }
            else
            {
                //(y1,x1) is passed in xt
               // putpixel(y1, x1, YELLOW);
                pk = pk + 2 * dy;
            }
        }
        else
        {
            y1 < y2 ? y1++ : y1--;
            if (decide == 0)
            {
                //putpixel(x1, y1, RED);
            }
            else
            {
              //  putpixel(y1, x1, YELLOW);
            }
            pk = pk + 2 * dy - 2 * dx;
        }
    }
}
 

void draw_bresenham_line_steps(int x1, int y1, int x2, int y2)
{
   // int gd = DETECT, gm;
   // initgraph(&gd, &gm, "xxx");
    int dx, dy;
    //cin cout
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    //If slope is less than one
    if (dx > dy)
    {
        //passing argument as 0 to plot(x,y)
        plot_point(x1, y1, x2, y2, dx, dy, 0);
    }
    //if slope is greater than or equal to 1
    else
    {
        //passing argument as 1 to plot (y,x)
        plot_point(y1, x1, y2, x2, dy, dx, 1);
    }
}


void draw_line_mm(float start_x_mm, float start_y_mm, float end_x_mm, float end_y_mm)
{
    int start_x_steps, start_y_steps, end_x_steps, end_y_steps;
    if (start_x_mm == INT_MIN) start_x_steps = STEPPER_X.currentPosition();
    else start_x_steps = start_x_mm/STEP_QUANTA_MM;
    if (start_y_mm == INT_MIN) start_y_steps = STEPPER_Y.currentPosition();
    else start_y_steps = start_y_mm/STEP_QUANTA_MM;

    end_x_steps = end_x_mm/STEP_QUANTA_MM;
    end_y_steps = end_y_mm/STEP_QUANTA_MM;

    PRINT_EQUAL_BREAK;
    Serial.printf("Current steps: (%ld, %ld)\n", STEPPER_X.currentPosition(), STEPPER_Y.currentPosition());
    Serial.printf("Drawing line (in mm) till:: (%4f, %4f)\n", end_x_mm, end_y_mm);
    Serial.printf("Drawing line in steps:: (%d, %d) -> (%d, %d)\n", start_x_steps, start_y_steps, end_x_steps, end_y_steps);
    PRINT_EQUAL_BREAK;
    
    draw_bresenham_line_steps(start_x_steps, start_y_steps, end_x_steps, end_y_steps);
    Serial.println("DONE!");
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
