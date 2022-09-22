// #include <Stepper.h>
// #include <Arduino.h>

// // Define pin connections
// const int direction_pin = A3;
// const int step_pin = A4;
// const int enable_pin = A2;
// const int sleep_pin = A1;
// const int reset_pin = A0;

// const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// // for your motor

// // initialize the stepper library on pins 8 through 11:
// Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// void setup() {
//   // set the speed at 60 rpm:
//   myStepper.setSpeed(60);
//   // initialize the serial port:
//   Serial.begin(9600);
// }

// void loop() {
//   // step one revolution  in one direction:
//   Serial.println("clockwise");
//   myStepper.step(stepsPerRevolution);
//   delay(500);

//   // step one revolution in the other direction:
//   Serial.println("counterclockwise");
//   myStepper.step(-stepsPerRevolution);
//   delay(500);
// }