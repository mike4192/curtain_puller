#pragma once

// Numbers correspond to Pico GPIO pin numbers
// NOTE GP13 pin on pico is probably dead due to accidental short
#define STEPPER_PIN 14
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
#define MS1_MICROSTEP_PIN 5
#define MS2_MICROSTEP_PIN 4

// Time that last high PIR sensor state will be persisted
#define PIR_SENSOR_TIME_PERSISTENCE_MS 20000
#define DEBOUNCE_TIME_MS 25

#define FAST_SPEED 800    // steps/second
#define SLOW_SPEED 100    // steps/decond
#define ACCEL 1750        // steps/second/second
#define ESTOP_ACCEL 3000  // steps/second/second, applied when limit switch is hit
// Number of steps before the limit switch position the motion profile first targett.
// Roughly when this target is hit, speed drops to SLOW_SPEED
#define UNDERSHOOT_STEPS 100

#define HOMING_MAX_DISTANCE 5000  // steps
#define HOMING_SPEED 300          // steps/second

#define INITIAL_HOMING_DIR 1

#define LOGGING_ENABLED false

// If true, swap the limit switches in code so the cmd state for
// on_box_limit_switch actually corresponds to the off_box_limit_switch
// and vice versa. This can be used if the limit switch stopper is used on the
// opposite belt side of the curtain connection.
#define SWAP_LIMIT_SWITCHES true

// This defines the microstepping mode
enum class StepMode { HALF, QUARTER, EIGHTH, SIXTEENTH };
#define STEP_MODE StepMode::HALF

// Looking from basement towards curtain:
// RIGHT_TO_LEFT: Left side curtain is fixed, open = curtain pulled to left, close = curtain pushed
// to right LEFT_TO_RIGHT: Right side curtain is fixed, open = curtain pulled to right, close =
// curtain pushed to left
enum class OpenMode { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
#define OPEN_MODE OpenMode::LEFT_TO_RIGHT