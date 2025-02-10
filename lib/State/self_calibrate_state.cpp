#include "self_calibrate_state.h"

#include "FlexyStepper.h"
#include "constants.h"
#include "error_state.h"
#include "override_open_close_state.h"
#include "utils.h"

void SelfCalibrate::init() {
  Serial.println("In calibrate state's init()");
  digitalWrite(YELLOW_LED, HIGH);
  stepper_->setSpeedInStepsPerSecond(HOMING_SPEED);
}

bool SelfCalibrate::tick(const CommandData& command_data) {
  // If both limit switches are depressed, this is indicative of an
  // error and Error state should be entered.
  if (command_data.on_box_limit_switch && command_data.off_box_limit_switch) {
    logIfEnabled("Both limit switches depressed. Transitioning to error state.");
    next_state = std::make_unique<Error>();
    return true;
  }

  if (!first_homing_complete_) {
    if (!first_homing_commanded_) {
      logIfEnabled("First homing commanded");
      first_homing_commanded_ = true;
      stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR);
    }
    if (command_data.on_box_limit_switch) {
      stepper_->setTargetPositionToStop();
      // Record the position
      on_box_limit_switch_pos_ = stepper_->getCurrentPositionInSteps();
      auto msg = "First homing completed. On box limit switch position: " + String(on_box_limit_switch_pos_);
      logIfEnabled(msg.c_str());
      first_homing_complete_ = true;
    }
  } else if (!second_homing_complete_) {
    if (!second_homing_commanded_) {
      logIfEnabled("Second homing commanded");
      second_homing_commanded_ = true;
      stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR * -1);
    }
    if (command_data.off_box_limit_switch) {
      logIfEnabled("Off box limit switch hit");
      off_box_limit_switch_pos_ = stepper_->getCurrentPositionInSteps();
      stepper_->setTargetPositionToStop();
      second_homing_complete_ = true;
      // Recalculate on box limit switch position and then set current motor position to 0
      on_box_limit_switch_pos_ = on_box_limit_switch_pos_ - off_box_limit_switch_pos_;
      stepper_->setCurrentPositionInSteps(0L);
    }
  }

  stepper_->processMovement();

  // Handle transitions
  if (first_homing_complete_ && second_homing_complete_) {
    if (command_data.override_open) {
      next_state = std::make_unique<OverrideOpenClose>(/*is_override_open_commanded=*/true);
      digitalWrite(YELLOW_LED, LOW);
      return true;
    } else if (command_data.override_close) {
      next_state = std::make_unique<OverrideOpenClose>(/*is_override_open_commanded=*/false);
      ;
      digitalWrite(YELLOW_LED, LOW);
      return true;
    }
  } else if (command_data.override_close) {
    // If override close commanded before calibration is complete, go to error state
    next_state = std::make_unique<Error>();
    digitalWrite(YELLOW_LED, LOW);
    return true;
  }
  return false;
}