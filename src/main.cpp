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

Button override_open_button(OVERRIDE_OPEN_BUTTON_PIN);
Button override_close_button(OVERRIDE_CLOSE_BUTTON_PIN);
Button auto_or_disable_button(AUTO_DISABLE_BUTTON_PIN);
Button ir_sensor_short_wire(IR_SENSOR_SHORT_WIRE_PIN);
Button ir_sensor_long_wire(IR_SENSOR_LONG_WIRE_PIN);
Button on_box_limit_switch(ON_BOX_LIMIT_SWITCH);
Button off_box_limit_switch(OFF_BOX_LIMIT_SWITCH);

void pollInputs() {
  if (auto_or_disable_button.pressed()) {
    command_data.setAutomaticOrDisable();
  } else if (override_open_button.pressed()) {
    command_data.setOverrideOpen();
  } else if (override_close_button.pressed()) {
    command_data.setOverrideClose();
  }
  
  // Check IR Sensor Inputs
  command_data.ir_sensor_short_wire = ir_sensor_short_wire.read();
  command_data.ir_sensor_long_wire = ir_sensor_long_wire.read();

  // Check limit switches
  command_data.on_box_limit_switch = on_box_limit_switch.read();
  command_data.off_box_limit_switch = off_box_limit_switch.read();
}

void setup() {
  Serial.begin(9600);

  // Setup buttons
  override_open_button.begin();
  override_close_button.begin();
  auto_or_disable_button.begin();
  on_box_limit_switch.begin();
  off_box_limit_switch.begin();

  // Setup pins for IR sensors manually as button library only 
  // supports pullup by default
  pinMode(IR_SENSOR_SHORT_WIRE_PIN, INPUT_PULLUP); // TODO: Check if should be pullup or down
  pinMode(IR_SENSOR_LONG_WIRE_PIN, INPUT_PULLUP); // TODO: Check if should be pullup or down
  
  // initialize LED pins as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Other setup
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  pinMode(SLEEP_PIN, OUTPUT);
  digitalWrite(SLEEP_PIN, LOW); // LOW: disabled, HIGH: enabled

  stepper.connectToPins(STEPPER_PIN, DIR_PIN);
  stepper.setSpeedInStepsPerSecond(FAST_SPEED);
  stepper.setAccelerationInStepsPerSecondPerSecond(ACCEL);
}

// the loop routine runs over and over again forever:
void loop() {
  pollInputs();

  state_controller.tick(command_data);

  command_data.clearButtonStates();
}

