#include "state.h"

#include "utils.h"

// Initialize the static stepper pointer. No protections are put in
// place on this pointer as we assume it is properly assigned with a real object
// at the beginning of the program.
FlexyStepper* State::stepper_ = nullptr;

// Initialize the various positions. Assume these will be set in the
// self calibrate state upon initial homing
long State::on_box_limit_switch_pos_ = 0L;
long State::off_box_limit_switch_pos_ = 0L;
long State::open_pos_ = 0L;
long State::close_pos_ = 0L;

void State::process_motion_profile(MotionCmd open_close_cmd, MotionCmd prev_cmd,
                                   const CommandData cmd_data) {
  // If the open/close command changes, reset the motion profile to the init state
  if (open_close_cmd != prev_cmd) {
    logIfEnabled("Motion command changed, resetting motion profile");
    motion_state_ = MotionState::INIT;
  }

  float current_velocity = stepper_->getCurrentVelocityInStepsPerSecond();
  float current_abs_velocity = std::abs(current_velocity);

  switch (motion_state_) {
    case MotionState::INIT:
      logIfEnabled("Motion Profile: In INIT, going to ACCELERATE");
      enable_driver();
      stepper_->setSpeedInStepsPerSecond(FAST_SPEED);

      {
        long target_pos{0L};
        if (open_close_cmd == MotionCmd::OPEN) {
          target_pos = open_pos_ + UNDERSHOOT_STEPS;
        } else {
          target_pos = close_pos_ - UNDERSHOOT_STEPS;
        }
        stepper_->setTargetPositionInSteps(target_pos);
        logIfEnabled(("Motion Profile: In INIT, Curret pos: " +
                      String(stepper_->getCurrentPositionInSteps()) +
                      ", Going to position:" + String(target_pos))
                         .c_str());
      }

      motion_state_ = check_limit_switches(MotionState::ACCELERATE, cmd_data, open_close_cmd);
      break;

    case MotionState::ACCELERATE: {
      auto next_motion_state = MotionState::ACCELERATE;
      if (current_abs_velocity > SLOW_SPEED) {
        logIfEnabled("OOC State: Going to FAST_MOTION");
        next_motion_state = MotionState::FAST_MOTION;
      }
      motion_state_ = check_limit_switches(next_motion_state, cmd_data, open_close_cmd);
    } break;

    case MotionState::FAST_MOTION: {
      auto next_motion_state = MotionState::FAST_MOTION;
      if (current_abs_velocity < SLOW_SPEED) {
        logIfEnabled("OOC State: Going to CREEP_MOTION");
        stepper_->setSpeedInStepsPerSecond(SLOW_SPEED);
        if (open_close_cmd == MotionCmd::OPEN) {
          stepper_->setTargetPositionInSteps(open_pos_);
        } else {
          stepper_->setTargetPositionInSteps(close_pos_);
        }
        next_motion_state = MotionState::CREEP_MOTION;
      }
      motion_state_ = check_limit_switches(next_motion_state, cmd_data, open_close_cmd);
    } break;

    case MotionState::CREEP_MOTION: {
      auto next_motion_state = MotionState::CREEP_MOTION;
      if (stepper_->motionComplete()) {
        logIfEnabled("OOC State: Going to COMPLETE");
        next_motion_state = MotionState::COMPLETE;
        disable_driver();
      }
      motion_state_ = check_limit_switches(next_motion_state, cmd_data, open_close_cmd);
    } break;

    case MotionState::ESTOP:
      if (stepper_->motionComplete()) {
        stepper_->setAccelerationInStepsPerSecondPerSecond(ACCEL);
        motion_state_ = MotionState::COMPLETE;
        disable_driver();
      }
      break;

    case MotionState::COMPLETE:
      // Do nothing
      break;
  }

  stepper_->processMovement();
}

MotionState State::check_limit_switches(const MotionState motion_state, const CommandData cmd_data,
                                        MotionCmd open_close_cmd) {
  // Passes through the inputted motion state, unless limit switches are hit, upon
  // which the ESTOP motion state is returned

  if (((open_close_cmd == MotionCmd::OPEN) && cmd_data.on_box_limit_switch) ||
      ((open_close_cmd == MotionCmd::CLOSE) && cmd_data.off_box_limit_switch)) {
    logIfEnabled(
        "Motion Profile: Limit switch hit. Setting estop accel and stopping cmd, going to ESTOP "
        "state.");
    stepper_->setAccelerationInStepsPerSecondPerSecond(ESTOP_ACCEL);
    stepper_->setTargetPositionToStop();
    return MotionState::ESTOP;
  }
  return motion_state;
}