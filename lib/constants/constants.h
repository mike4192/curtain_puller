#pragma once

// Numbers correspond to GPIO pin numbers 
#define STEPPER_PIN 13
#define DIR_PIN 12
#define AUTO_DISABLE_BUTTON_PIN 26
#define OVERRIDE_CLOSE_BUTTON_PIN 27
#define OVERRIDE_OPEN_BUTTON_PIN 22 
#define IR_SENSOR_SHORT_WIRE_PIN 8
#define IR_SENSOR_LONG_WIRE_PIN 7
#define ON_BOX_LIMIT_SWITCH 10
#define OFF_BOX_LIMIT_SWITCH 9
#define GREEN_LED 21
#define RED_LED 18
#define YELLOW_LED 20
#define BLUE_LED 19
#define SLEEP_PIN 11
#define RESET_PIN 6
// TODO: Add pin for stepper microstepping

#define FAST_SPEED 2000 // steps/second
#define SLOW_SPEED 400 // steps/decond
#define ACCEL 1600 // steps/second/second