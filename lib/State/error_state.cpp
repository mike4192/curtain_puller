#include "error_state.h"

#include "constants.h"
#include "self_calibrate_state.h"
#include "utils.h"

void Error::init() {
  logIfEnabled("In Error state's init()");
  // Ensure stepper is stopped, set driver to sleep mode
  stepper_->setTargetPositionToStop();
  digitalWrite(SLEEP_PIN, LOW);
}

bool Error::tick(const CommandData& command_data) {
  // Do nothing in init state and just blink red LED
  if (millis() - led_toggle_time > 250) {
    led_toggle_time = millis();
    digitalWrite(RED_LED, !digitalRead(RED_LED));
  }

  if (command_data.automatic) {
    logIfEnabled("Transitioning to self calibrate state from Error");
    digitalWrite(RED_LED, LOW);
    next_state = std::make_unique<SelfCalibrate>();
    return true;
  }
  return false;
}