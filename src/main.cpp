#include <Arduino.h>
#include "Button.h"
#include "command_data.h"
#include "constants.h"
#include "FlexyStepper.h"
#include "state.h"
#include "state_controller.h"

FlexyStepper stepper;
CommandData command_data;
StateController state_controller(std::make_unique<Init>(stepper));

Button override_open_button(OVERRIDE_OPEN_PIN);
Button override_close_button(OVERRIDE_CLOSE_PIN);
Button auto_or_disable_button(AUTO_DISABLE_PIN);
Button ir_sensor_1(IR_SENSOR_1_PIN);

void pollInputs() {
  if (auto_or_disable_button.pressed()) {
    command_data.setAutomaticOrDisable();
  } else if (override_open_button.pressed()) {
    command_data.setOverrideOpen();
  } else if (override_close_button.pressed()) {
    command_data.setOverrideClose();
  }

  if (ir_sensor_1.toggled()) {
      command_data.ir_sensor_1 = ir_sensor_1.read();
  }

  // TODO: Check limit switches and IR sensors
}

void setup() {
  Serial.begin(9600);

  // Setup buttons
  override_open_button.begin();
  override_close_button.begin();
  auto_or_disable_button.begin();
  // TODO: setup limit switch inputs

  // Setup pins for IR sensors manually as button library only 
  // supports pullup by default
  pinMode(IR_SENSOR_1_PIN, INPUT_PULLUP); // TODO: Check if should be pullup or down
  pinMode(IR_SENSOR_2_PIN, INPUT_PULLUP); // TODO: Check if should be pullup or down
  
  // initialize LED pins as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  stepper.connectToPins(STEPPER_PIN, DIR_PIN);
  stepper.setSpeedInStepsPerSecond(SPEED);
  stepper.setAccelerationInStepsPerSecondPerSecond(ACCEL);
}

// the loop routine runs over and over again forever:
void loop() {
  pollInputs();

  state_controller.tick(command_data);

  command_data.clearButtonStates();
}

