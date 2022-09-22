
#include <Arduino.h> 


// void run_line_stepper_2(int stepsx, int stepsy)
// {
//     int base_speed = 5000, base_x_speed=base_speed, base_y_speed=base_speed;
//     int max_speed = 5000, max_x_speed=max_speed, max_y_speed=max_speed;
    
//     if (stepsx < 0) 
//     {
//       base_x_speed = -base_speed;
//       max_x_speed = -max_speed;
//     }
//     if (stepsy < 0) 
//     {
//       base_y_speed = -base_speed;
//       max_y_speed = -max_speed;
//     }

//     // Serial.print("Props:: speed_x, speed_y, current_x, current_y, steps_x, steps_y:: ");
//     // Serial.print(max_x_speed);
//     // Serial.print(" ");
//     // Serial.print(max_y_speed);
//     // Serial.print(" ");
//     // Serial.print(STEPPER_X.currentPosition());
//     // Serial.print(" ");
//     // Serial.print(STEPPER_Y.currentPosition());
//     // Serial.print(" ");
//     // Serial.print(stepsx);
//     // Serial.print(" ");
//     // Serial.print(stepsy);
//     // Serial.println(" ");

//     STEPPER_X.setMaxSpeed(max_x_speed);
//     STEPPER_X.moveTo(STEPPER_X.currentPosition() + stepsx);
//     STEPPER_X.setSpeed(max_x_speed);

//     STEPPER_Y.setMaxSpeed(max_y_speed);
//     STEPPER_Y.moveTo(STEPPER_Y.currentPosition() + stepsy);
//     STEPPER_Y.setSpeed(max_y_speed);

//     wdt_disable();
//     while ((STEPPER_X.distanceToGo() != 0) || (STEPPER_Y.distanceToGo() != 0))
//     {
//         // Serial.print("Inside!");
//         // Serial.print("Current X: ");
//         // Serial.print(STEPPER_X.currentPosition());
//         // Serial.print(" ");
//         // Serial.print("To Go X: ");
//         // Serial.print(STEPPER_X.distanceToGo());
//         // Serial.println(" ");

//         // Serial.print("Current Y: ");
//         // Serial.print(STEPPER_Y.currentPosition());
//         // Serial.print(" ");
//         // Serial.print("To Go Y: ");
//         // Serial.print(STEPPER_Y.distanceToGo());
//         // Serial.println(" ");

//         // Serial.println(STEPPER_X.distanceToGo());
//         // Serial.println(STEPPER_Y.distanceToGo());

//         if (STEPPER_X.distanceToGo() != 0) STEPPER_X.runSpeed();
//         if (STEPPER_Y.distanceToGo() != 0) STEPPER_Y.runSpeed();
//         // delay(1);
//     }
//     wdt_enable(1000);
// }


void plot_pixel(int x1, int y1, int x2, int y2, int dx, int dy, int decide)
{
    //pk is initial decision making parameter
    //Note:x1&y1,x2&y2, dx&dy values are interchanged
    //and passed in plot_pixel function so
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
 

void draw_line_steps_2(int x1, int y1, int x2, int y2)
{
   // int gd = DETECT, gm;
   // initgraph(&gd, &gm, "xxx");
    int dx, dy, pk;
    //cin cout
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    //If slope is less than one
    if (dx > dy)
    {
        //passing argument as 0 to plot(x,y)
        plot_pixel(x1, y1, x2, y2, dx, dy, 0);
    }
    //if slope is greater than or equal to 1
    else
    {
        //passing argument as 1 to plot (y,x)
        plot_pixel(y1, x1, y2, x2, dy, dx, 1);
    }
   // getch();
}