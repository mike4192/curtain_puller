#include <Arduino.h>

#include "Button.h"
#include "FlexyStepper.h"
#include "command_data.h"
#include "constants.h"
#include "init_state.h"
#include "state.h"
#include "state_controller.h"

FlexyStepper stepper;
CommandData command_data;

StateController state_controller{};

Button override_open_button(OVERRIDE_OPEN_BUTTON_PIN);
Button override_close_button(OVERRIDE_CLOSE_BUTTON_PIN);
Button auto_or_disable_button(AUTO_DISABLE_BUTTON_PIN);
Button ir_sensor_short_wire(IR_SENSOR_SHORT_WIRE_PIN);
Button ir_sensor_long_wire(IR_SENSOR_LONG_WIRE_PIN);
Button on_box_limit_switch(ON_BOX_LIMIT_SWITCH, DEBOUNCE_TIME_MS);
Button off_box_limit_switch(OFF_BOX_LIMIT_SWITCH, DEBOUNCE_TIME_MS);

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
  // Reset pin remains permanently high
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  // Note for sleep mode pin: LOW->disabled, HIGH->enabled
  pinMode(SLEEP_PIN, OUTPUT);
  digitalWrite(SLEEP_PIN, LOW);
  // Set microstepping mode to half step
  pinMode(MS1_HALF_STEP_PIN, OUTPUT);
  digitalWrite(MS1_HALF_STEP_PIN, HIGH);
}

// Loop forever
void loop() {
  pollInputs();

  state_controller.tick(command_data);

  command_data.clearButtonStates();
}
