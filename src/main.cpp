#include <Arduino.h>

#include "Button.h"
#include "FlexyStepper.h"
#include "command_data.h"
#include "constants.h"
#include "init_state.h"
#include "state.h"
#include "state_controller.h"
#include "utils.h"

FlexyStepper stepper;
CommandData command_data;

StateController state_controller{};

Button override_open_button(OVERRIDE_OPEN_BUTTON_PIN);
Button override_close_button(OVERRIDE_CLOSE_BUTTON_PIN);
Button auto_or_disable_button(AUTO_DISABLE_BUTTON_PIN);
Button ir_sensor_short_wire(IR_SENSOR_SHORT_WIRE_PIN);
Button ir_sensor_long_wire(IR_SENSOR_LONG_WIRE_PIN);
const int on_box_limit_switch_pin =
    SWAP_LIMIT_SWITCHES ? OFF_BOX_LIMIT_SWITCH : ON_BOX_LIMIT_SWITCH;
const int off_box_limit_switch_pin =
    SWAP_LIMIT_SWITCHES ? ON_BOX_LIMIT_SWITCH : OFF_BOX_LIMIT_SWITCH;

Button on_box_limit_switch(on_box_limit_switch_pin, DEBOUNCE_TIME_MS);
Button off_box_limit_switch(off_box_limit_switch_pin, DEBOUNCE_TIME_MS);

void pollInputs() {
  if (auto_or_disable_button.pressed()) {
    command_data.setAutomaticOrDisable();
  } else if (override_open_button.pressed()) {
    command_data.setOverrideOpen();
  } else if (override_close_button.pressed()) {
    command_data.setOverrideClose();
  }

  // Check PIR Sensor Inputs with software controlled time persistence. The command
  // state remains high for a specified time period after the PIR input was last high
  static unsigned long short_wire_last_high_time = 0;
  static unsigned long long_wire_last_high_time = 0;

  if (ir_sensor_short_wire.read()) {
    short_wire_last_high_time = millis();
  }
  if (ir_sensor_long_wire.read()) {
    long_wire_last_high_time = millis();
  }

  command_data.ir_sensor_short_wire =
      (millis() - short_wire_last_high_time) < PIR_SENSOR_TIME_PERSISTENCE_MS;
  command_data.ir_sensor_long_wire =
      (millis() - long_wire_last_high_time) < PIR_SENSOR_TIME_PERSISTENCE_MS;

  // Check limit switches
  command_data.on_box_limit_switch = !on_box_limit_switch.read();
  command_data.off_box_limit_switch = !off_box_limit_switch.read();
}

void set_microstepping_mode() {
  digitalWrite(MS1_MICROSTEP_PIN, LOW);
  digitalWrite(MS2_MICROSTEP_PIN, LOW);

  switch (STEP_MODE) {
    case StepMode::HALF:
      digitalWrite(MS1_MICROSTEP_PIN, HIGH);
      break;
    case StepMode::QUARTER:
      digitalWrite(MS2_MICROSTEP_PIN, HIGH);
      break;
    case StepMode::EIGHTH:
      // Both MS1 and MS2 low
      break;
    case StepMode::SIXTEENTH:
      digitalWrite(MS1_MICROSTEP_PIN, HIGH);
      digitalWrite(MS2_MICROSTEP_PIN, HIGH);
      break;
  }
}

void setup() {
  if (LOGGING_ENABLED) {
    Serial.begin(9600);
  }

  stepper.connectToPins(STEPPER_PIN, DIR_PIN);

  // Set the static stepper pointer in the State class
  State::setStepper(&stepper);

  // Set the initial state for the state controller
  state_controller.initialize(std::make_unique<Init>());

  // Setup buttons
  override_open_button.begin();
  override_close_button.begin();
  auto_or_disable_button.begin();
  on_box_limit_switch.begin();
  off_box_limit_switch.begin();

  // Setup pins for IR sensors explicitly as button library only
  // supports initializing as pullup by default. We set it as INPUT_PULLUP
  // as it doesn't really matter as the HC-SR501 based PIR sensor has a push-pull
  // based output.
  pinMode(IR_SENSOR_SHORT_WIRE_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_LONG_WIRE_PIN, INPUT_PULLUP);

  // initialize LED pins as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Other pin setup
  // Note for enable pin: LOW->ON, HIGH->OFF
  pinMode(SLEEP_PIN, OUTPUT);
  disable_driver();

  // Set microstepping pins as outputs
  pinMode(MS1_MICROSTEP_PIN, OUTPUT);
  pinMode(MS2_MICROSTEP_PIN, OUTPUT);

  // Set desired microstepping mode
  set_microstepping_mode();
}

// Loop forever
void loop() {
  pollInputs();

  state_controller.tick(command_data);

  command_data.clearButtonStates();
}
