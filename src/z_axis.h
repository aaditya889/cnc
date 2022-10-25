#include <Servo.h>


const int z_axis_velocity = 100;

void disengage_z_axis()
{
  Serial.printf("Disengaging Z Axis...\n");
  Z_AXIS.write(Z_AXIS_SERVO_DISENGAGE_ANGLE);
}

void engage_z_axis()
{
  Serial.printf("Engaging Z Axis...\n");

  // for (int i = 0; i < Z_AXIS_SERVO_ENGAGE_ANGLE; i++)
  // {
  //   Z_AXIS.write(i);
  //   delay(1000/z_axis_velocity);
  // }
  Z_AXIS.write(Z_AXIS_SERVO_ENGAGE_ANGLE);
}

void initialise_z_axis()
{
  Serial.printf("Initialising Z Axis...\n");
  Z_AXIS.attach(Z_AXIS_PIN);
  Z_AXIS.write(Z_AXIS_SERVO_DISENGAGE_ANGLE);
}


void test_z_axis() 
{
  // Z_AXIS.attach(D8);
  int vals[5] = {0, -30, -45, 50, 60};

  for (int i = 0; i < 5; i++)
  {
    Serial.printf("Writing: %d\n", vals[i]);
    Z_AXIS.write(vals[i]);
    delay(2000);
  }
}

