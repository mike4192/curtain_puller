#include "disabled_state.h"

#include "self_calibrate_state.h"
#include "utils.h"

void Disabled::init() {
  logIfEnabled("In disabled init");
  stepper_->setTargetPositionToStop();
  disable_driver();
}

bool Disabled::tick(const CommandData& command_data) {
  // In disabled sate we just blink the yellow LED as an indication
  // we are in disabled state. The only allowed transition is back to self calibrate
  // via the auto button. Not other transitions are possible as we have no idea
  // where the stepper motor may have been moved to in this state.
  if (millis() - led_toggle_time_ms > 500) {
    led_toggle_time_ms = millis();
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
  }

  // Handle Transitions
  bool ret_val = false;
  if (command_data.automatic) {
    next_state = std::make_unique<SelfCalibrate>();
    ret_val = true;
  }

  // Turn off LED if transitioning out of disabled
  if (ret_val) {
    digitalWrite(YELLOW_LED, LOW);
  }
  return ret_val;
}