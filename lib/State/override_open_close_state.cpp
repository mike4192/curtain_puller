#include "override_open_close_state.h"

#include "FlexyStepper.h"
#include "auto_state.h"
#include "constants.h"
#include "override_open_close_state.h"
#include "utils.h"

void OverrideOpenClose::init() {
  logIfEnabled("In override open close's init()");

  // Set LED to green or red if in override open or close mode, respectively.
  if (cmd == MotionCmd::OPEN) {
    digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(RED_LED, HIGH);
  }
}

bool OverrideOpenClose::tick(const CommandData& command_data) {
  // float current_velocity = stepper_->getCurrentVelocityInStepsPerSecond();
  // float current_abs_velocity = std::abs(current_velocity);

  // switch (motion_state) {
  //   case MotionState::INIT:
  //     logIfEnabled("OOC State: In INIT, going to ACCELERATE");
  //     // Set sleep pin to HIGH to enable motor driver
  //     digitalWrite(SLEEP_PIN, HIGH);
  //     stepper_->setSpeedInStepsPerSecond(FAST_SPEED);

  //     {
  //       long target_pos{0L};

  //       if (is_open_commanded_) {
  //         target_pos = open_pos_ - UNDERSHOOT_STEPS;
  //       } else {
  //         target_pos = close_pos_ + UNDERSHOOT_STEPS;
  //       }
  //       stepper_->setTargetPositionInSteps(target_pos);
  //       auto msg = "OOC State: In INIT, Curret pos: " + String(stepper_->getCurrentPositionInSteps()) + ", Going to position:" + String(target_pos);
  //       logIfEnabled(msg.c_str());
  //     }

  //     motion_state = check_limit_switches(MotionState::ACCELERATE, command_data);
  //     break;
      
  //   case MotionState::ACCELERATE:
  //     {
  //       auto next_motion_state = MotionState::ACCELERATE; 
  //       if (current_abs_velocity > SLOW_SPEED) {
  //         logIfEnabled("OOC State: Going to FAST_MOTION");
  //         next_motion_state = MotionState::FAST_MOTION;
  //       }
  //       motion_state = check_limit_switches(next_motion_state, command_data);
  //     }
  //     break;
    
  //   case MotionState::FAST_MOTION:
  //     {
  //       auto next_motion_state = MotionState::FAST_MOTION;
  //       if (current_abs_velocity < SLOW_SPEED) {
  //         logIfEnabled("OOC State: Going to CREEP_MOTION");
  //         stepper_->setSpeedInStepsPerSecond(SLOW_SPEED);
  //         if (is_open_commanded_) {
  //           stepper_->setTargetPositionInSteps(open_pos_);
  //         } else {
  //           stepper_->setTargetPositionInSteps(close_pos_); 
  //         }
  //         next_motion_state = MotionState::CREEP_MOTION;
  //       motion_state = check_limit_switches(next_motion_state, command_data);
  //       }
  //     }
  //     break;
    
  //   case MotionState::CREEP_MOTION:
  //     {
  //       auto next_motion_state = MotionState::CREEP_MOTION;
  //       if (stepper_->motionComplete()) {
  //         logIfEnabled("OOC State: Going to COMPLETE");
  //         next_motion_state = MotionState::COMPLETE;
  //         // Set sleep pin low to disable motor diver 
  //         digitalWrite(SLEEP_PIN, LOW);
  //       }
  //       motion_state = check_limit_switches(next_motion_state, command_data);
  //     }
  //     break;
    
  //   case MotionState::COMPLETE:
  //     // Do nothing
  //     break;
  // }

  // stepper_->processMovement();

  // In override open or close state, the command will never change from tick to tick, 
  // so just set the prev_cmd input and the current cmd
  process_motion_profile(cmd, cmd, command_data);

  // Handle State transitions
  bool ret_val = false;

  if (cmd == MotionCmd::OPEN && command_data.override_close) {
    logIfEnabled("Transitioning to override close from override open");
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::CLOSE);
    ret_val = true;
  } else if ((cmd == MotionCmd::CLOSE) && command_data.override_open) {
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


// MotionState OverrideOpenClose::check_limit_switches(const MotionState motion_state, const CommandData cmd_data) {
//   // Passes through the inputted motion state, unless limit switches are hit, upon
//   // which the ESTOP motion state is returned

//   if ( (is_open_commanded_ && cmd_data.on_box_limit_switch) ||
//        (!is_open_commanded_ && cmd_data.off_box_limit_switch)) {
//     logIfEnabled("OOC State: Limit switch hit. Going to ESTOP motion state.");
//     return MotionState::ESTOP;
//   }
//   return motion_state;
// }
