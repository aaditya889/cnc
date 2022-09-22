#include <Arduino.h>

// // Define pin connections & motor's steps per revolution
// const int DIRECTION_PIN = A3;
// const int STEP_PIN_1 = A4;
// const int enable_pin = A2;
// const int sleep_pin = A1;
// const int reset_pin = A0;

// // const int sleep_pin = 
// const int steps_per_revolution = 200;

// void setup()
// {
// 	Serial.begin(9600);
// 	// Declare pins as Outputs
// 	pinMode(STEP_PIN_1, OUTPUT);
// 	pinMode(DIRECTION_PIN, OUTPUT);
// 	pinMode(enable_pin, OUTPUT);
// 	pinMode(sleep_pin, OUTPUT);
// 	pinMode(reset_pin, OUTPUT);

// 	digitalWrite(enable_pin, LOW);
// 	digitalWrite(sleep_pin, HIGH);
// 	digitalWrite(reset_pin, HIGH);
// }

void run_stepper(int steps_per_revolution, int delay_microseconds)
{
	wdt_disable();
	// Set motor direction clockwise
	digitalWrite(DIRECTION_PIN_1, HIGH);

	// Spin motor slowly
  	Serial.println("Spinning motor forward...");

	for(int x = 0; x < steps_per_revolution; x++)
	{
		digitalWrite(STEP_PIN_1, HIGH);
		delayMicroseconds(delay_microseconds);
		digitalWrite(STEP_PIN_1, LOW);
		delayMicroseconds(delay_microseconds);
	}
	delay(2000); // Wait a second

	// Set motor direction counterclockwise
	digitalWrite(DIRECTION_PIN_1, LOW);

	// Spin motor quickly
	Serial.println("Spinning motor in reverse...");
	for(int x = 0; x < steps_per_revolution; x++)
	{
		digitalWrite(STEP_PIN_1, HIGH);
		delayMicroseconds(delay_microseconds);
		digitalWrite(STEP_PIN_1, LOW);
		delayMicroseconds(delay_microseconds);
	}
	delay(2000); // Wait a second
	wdt_enable(1);
}

void run_stepper_temp_2(int steps_per_revolution, int delay_microseconds)
{
	wdt_disable();
	// Set motor direction clockwise
	digitalWrite(DIRECTION_PIN_1, HIGH);

	// Spin motor slowly
  	Serial.println("Spinning motor forward...");

	for(int x = 0; x < steps_per_revolution; x++)
	{
		digitalWrite(STEP_PIN_1, HIGH);
		delayMicroseconds(delay_microseconds);
		digitalWrite(STEP_PIN_1, LOW);
		delayMicroseconds(delay_microseconds);
	}
	delay(2000); // Wait a second

	// Set motor direction counterclockwise
	digitalWrite(DIRECTION_PIN_1, LOW);

	// Spin motor quickly
	Serial.println("Spinning motor in reverse...");
	for(int x = 0; x < steps_per_revolution; x++)
	{
		digitalWrite(STEP_PIN_1, HIGH);
		delayMicroseconds(delay_microseconds);
		digitalWrite(STEP_PIN_1, LOW);
		delayMicroseconds(delay_microseconds);
	}
	delay(2000); // Wait a second
	wdt_enable(1);
}