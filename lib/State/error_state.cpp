#include "error_state.h"

#include "constants.h"
#include "self_calibrate_state.h"
#include "utils.h"

void Error::init() {
  logIfEnabled("In Error state's init()");
  // Ensure stepper is stopped, set driver to disabled mode
  stepper_->setTargetPositionToStop();
  disable_driver();
}

bool Error::tick(const CommandData& command_data) {
  // Do nothing in error state and just blink red LED
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