#include "auto_state.h"

#include "constants.h"
#include "override_open_close_state.h"

void Automatic::init() {
  Serial.println("In automatic init");
  digitalWrite(BLUE_LED, HIGH);
}

bool Automatic::tick(const CommandData& command_data) {
  if (command_data.ir_sensor_short_wire) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  // Handle Transitions
  bool ret_val = false;
  if (command_data.override_open) {
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::OPEN);
    ret_val = true;
  } else if (command_data.override_close) {
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::CLOSE);
    ret_val = true;
  }

  // Turn off LED's if transitioning out of auto
  if (ret_val) {
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
  return ret_val;
}