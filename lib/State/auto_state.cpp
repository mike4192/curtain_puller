#include "auto_state.h"

#include "constants.h"
#include "disabled_state.h"
#include "override_open_close_state.h"
#include "utils.h"

void Automatic::init() {
  logIfEnabled("In automatic init");
  digitalWrite(BLUE_LED, HIGH);
}

bool Automatic::tick(const CommandData& command_data) {
  MotionCmd current_motion_cmd{MotionCmd::CLOSE};

  if (command_data.ir_sensor_short_wire || command_data.ir_sensor_long_wire) {
    current_motion_cmd = MotionCmd::OPEN;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    current_motion_cmd = MotionCmd::OPEN;
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  process_motion_profile(current_motion_cmd, prev_motion_cmd_, command_data);
  prev_motion_cmd_ = current_motion_cmd;

  // Handle Transitions
  bool ret_val = false;
  if (command_data.override_open) {
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::OPEN);
    ret_val = true;
  } else if (command_data.override_close) {
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::CLOSE);
    ret_val = true;
  } else if (command_data.automatic) {
    next_state = std::make_unique<Disabled>();
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