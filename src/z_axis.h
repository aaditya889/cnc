#include <Servo.h>


Servo servo;


void test_z_axis() 
{

	servo.attach(D4);
  int vals[5] = {0, 30, 45, 50, 60};

  for (int i = 0; i < 5; i++)
  {
    Serial.printf("Writing: %d\n", vals[i]);
    servo.write(vals[i]);
    delay(2000);
  }
}