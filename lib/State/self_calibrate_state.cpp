#include "self_calibrate_state.h"

#include "FlexyStepper.h"
#include "constants.h"
#include "error_state.h"
#include "override_open_close_state.h"

void SelfCalibrate::init() {
  Serial.println("In calibrate state init");
  digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
}

bool SelfCalibrate::tick(const CommandData& command_data) {
  // If both limit switches are depressed, this is indicative of an
  // error and Error state should be entered.
  if (command_data.on_box_limit_switch && command_data.off_box_limit_switch) {
    next_state = std::make_unique<Error>();
    return true;
  }

  if (!first_homing_complete_) {
    if (!first_homing_commanded_) {
      first_homing_commanded_ = true;
      stepper_->setSpeedInStepsPerSecond(HOMING_SPEED);
      stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR);
    }
    if (command_data.on_box_limit_switch) {
      stepper_->setTargetPositionToStop();
      first_homing_complete_ = true;
    }
  } else if (!second_homing_complete_) {
    if (!second_homing_commanded_) {
      second_homing_commanded_ = true;
      stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR * -1);
    }
    if (command_data.off_box_limit_switch) {
      stepper_->setTargetPositionToStop();
      first_homing_complete_ = true;
    }
  }

  stepper_->processMovement();

  // Home to the other limit switch



  // Handle transitions
  // TODO: Don't allow transitions until calibration is complete
  if (command_data.override_open) {
    next_state = std::make_unique<OverrideOpen>();
    digitalWrite(YELLOW_LED, LOW);
    return true;
  } else if (command_data.override_close) {
    next_state = std::make_unique<OverrideClose>();
    digitalWrite(YELLOW_LED, LOW);
    return true;
  }
  return false;
}