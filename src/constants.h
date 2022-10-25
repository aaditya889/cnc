#include <Arduino.h>
// Include the AccelStepper Library
#include <AccelStepper.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Servo.h>
#include <BasicLinearAlgebra.h>


#define VERBOSE_LOGGING_IDX 0
#define IS_INCHES_IDX 1


// Define pin connections
const int DIRECTION_PIN_1 = D0;
const int STEP_PIN_1 = D1;
const int DIRECTION_PIN_2 = D2;
const int STEP_PIN_2 = D3;
const int Z_AXIS_PIN = D4;
// const int reset_pin = A0;
const int STEPPER_VELOCITY = 25;

// const int enable_pin = A2;
// const int sleep_pin = A1;

// Define motor interface type
#define motorInterfaceType 1

Servo Z_AXIS;
AccelStepper STEPPER_X(motorInterfaceType, STEP_PIN_1, DIRECTION_PIN_1);
AccelStepper STEPPER_Y(motorInterfaceType, STEP_PIN_2, DIRECTION_PIN_2);

// const float STEP_QUANTA_MM = 0.2881;
// const float STEP_QUANTA_MM = 0.1428;
// const float STEP_QUANTA_MM = 0.008925*5;
const double STEP_QUANTA_MM = 0.0087771496;

const int base_limit = 70000;
const int X_STEPS_LOW_LIMIT = -base_limit;
const int X_STEPS_HIGH_LIMIT = base_limit;
const int Y_STEPS_LOW_LIMIT = -base_limit;
const int Y_STEPS_HIGH_LIMIT = base_limit;

// const float mm_per_unit = 0.14;

const char *SSID = "Home889";
const char *PASSWORD = "H0me$889";
// const char *SSID = "aaditya_iphone";
// const char *PASSWORD = "yoyoyoyo";
uint16_t SERVER_PORT = 80;
char REMOTE_IP[] = "192.161.1.68";
uint16_t REMOTE_PORT = 8001;
WiFiUDP udp_client;
AsyncWebServer SERVER(SERVER_PORT);

#define PRINT_EQUAL_BREAK Serial.println("====================================================================================================================================================");
boolean DEBUG_SWITCHES[5] = {false, false, false, false, false};

// Global vars
const int MAX_GCODE_LINES = 500;
char GCODE_LINES[MAX_GCODE_LINES][30];
int END_GCODE_LINE_IDX = 0;
int START_GCODE_LINE_IDX = 0;
int Z_AXIS_SERVO_DISENGAGE_ANGLE = 60;
int Z_AXIS_SERVO_ENGAGE_ANGLE = 0;

const BLA::Matrix<2, 2> ROTATION_MATRIX= {0.7071, 0.7071,
                                          0.7071, -0.7071};

const float INCH_TO_MM_FACTOR = 25.4; 