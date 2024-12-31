#pragma once

#define STEPPER_PIN 0 // GP0
#define DIR_PIN 1 // GP1
#define AUTO_DISABLE_PIN 13 // GPX
#define OVERRIDE_CLOSE_PIN 14
#define OVERRIDE_OPEN_PIN 15 
#define IR_SENSOR_1_PIN 5 // GPX
#define IR_SENSOR_2_PIN 6 // GPX
#define LEFT_LIMIT_SWITCH 7 // GPXX
#define RIGHT_LIMIT_SWITCH 8 // GPXX
#define GREEN_LED 16
#define RED_LED 17 
#define YELLOW_LED 18
#define BLUE_LED 19

#define SPEED 100 // steps/second
#define ACCEL 100 // steps/second/second
#define DEBOUNCE_DELAY 200 // milliseconds