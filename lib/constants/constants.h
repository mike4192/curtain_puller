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
#define MS1_HALF_STEP_PIN 2
#define PIR_SENSOR_TIME_PERSISTENCE_MS 10000
#define DEBOUNCE_TIME_MS 25

#define FAST_SPEED 2000 // steps/second
#define SLOW_SPEED 600 // steps/decond
#define ACCEL 1500 // steps/second/second
#define ESTOP_ACCEL 5000
// TODO: Probably need a ESTOP accel value
#define UNDERSHOOT_STEPS 300

#define HOMING_MAX_DISTANCE 20000 // steps
#define HOMING_SPEED 600 // steps/second
#define INITIAL_HOMING_DIR 1

#define LOGGING_ENABLED true