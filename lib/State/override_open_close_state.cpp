#include "override_open_close_state.h"

#include "FlexyStepper.h"
#include "auto_state.h"
#include "constants.h"
#include "override_open_close_state.h"
#include "utils.h"

void OverrideOpenClose::init() {
  logIfEnabled("In override open close's init()");

  // Set LED to green or red if in override open or close mode, respectively.
  if (open_close_cmd_ == MotionCmd::OPEN) {
    digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(RED_LED, HIGH);
  }
}

bool OverrideOpenClose::tick(const CommandData& command_data) {
  // In override open or close state, the command will never change from tick to tick,
  // so just set the prev_cmd input and the current cmd
  process_motion_profile(open_close_cmd_, open_close_cmd_, command_data);

  // Handle State transitions
  bool ret_val = false;

  if (open_close_cmd_ == MotionCmd::OPEN && command_data.override_close) {
    logIfEnabled("Transitioning to override close from override open");
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::CLOSE);
    ret_val = true;
  } else if ((open_close_cmd_ == MotionCmd::CLOSE) && command_data.override_open) {
    logIfEnabled("Transitioning to override open from override close");
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::OPEN);
    ret_val = true;
  } else if (command_data.automatic) {
    logIfEnabled("Transitioning to auto from override open or close");
    next_state = std::make_unique<Automatic>();
    ret_val = true;
  }

  // Turn off LED's if transitioning state
  if (ret_val == true) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }

  return ret_val;
}