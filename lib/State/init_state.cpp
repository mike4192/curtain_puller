#include "init_state.h"

#include "constants.h"
#include "self_calibrate_state.h"
#include "utils.h"

void Init::init() {
  logIfEnabled("In Init state's init()");
  // Ensure stepper is stopped, set driver to sleep mode
  stepper_->setTargetPositionToStop();
  digitalWrite(SLEEP_PIN, LOW);
}

bool Init::tick(const CommandData& command_data) {
  // Do nothing in init state and just blink all LEDs for indication
  // of being in init state
  if (millis() - led_toggle_time > 250) {
    led_toggle_time = millis();
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
  }

  if (command_data.automatic) {
    logIfEnabled("Transitioning to self calibrate state");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    next_state = std::make_unique<SelfCalibrate>();
    return true;
  }
  return false;
}