#include "self_calibrate_state.h"

#include "FlexyStepper.h"
#include "auto_state.h"
#include "constants.h"
#include "error_state.h"
#include "override_open_close_state.h"
#include "utils.h"

void SelfCalibrate::init() {
  Serial.println("In calibrate state's init()");

  // Yelow LED is indicative of self calibration
  digitalWrite(YELLOW_LED, HIGH);
  stepper_->setSpeedInStepsPerSecond(HOMING_SPEED);

  // Set sleep pin to HIGH to enable motor driver and wait at least 1 ms
  digitalWrite(SLEEP_PIN, HIGH);
  delay(5);  // 5 ms
}

bool SelfCalibrate::tick(const CommandData& command_data) {
  switch (homing_state_) {
    case HomingState::INIT:
      logIfEnabled("SCS: Commanding first homing");
      stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR);
      homing_state_ = HomingState::HOMING_1;
      break;

    case HomingState::HOMING_1:
      if (command_data.on_box_limit_switch) {
        // Get current pos
        on_box_limit_switch_pos_ = stepper_->getCurrentPositionInSteps();
        logIfEnabled(("SCS: First homing completed. On box limit switch position: " +
                      String(on_box_limit_switch_pos_))
                         .c_str());

        // Set accel super high to stop very quickly, and command a stop
        stepper_->setAccelerationInStepsPerSecondPerSecond(ESTOP_ACCEL);
        stepper_->setTargetPositionToStop();
        homing_state_ = HomingState::STOP_1;
      }
      break;

    case HomingState::STOP_1:
      // Wait for motion to stop, then reset accel
      if (stepper_->motionComplete()) {
        stepper_->setAccelerationInStepsPerSecondPerSecond(ACCEL);
        stepper_->setTargetPositionInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR * -1);
        logIfEnabled("SCS: First homing stop complete. Second homing commanded.");
        homing_state_ = HomingState::HOMING_2;
      }
      break;

    case HomingState::HOMING_2:
      if (command_data.off_box_limit_switch) {
        off_box_limit_switch_pos_ = stepper_->getCurrentPositionInSteps();
        logIfEnabled(
            ("SCS: Off box limit switch hit with position: " + String(off_box_limit_switch_pos_))
                .c_str());

        stepper_->setAccelerationInStepsPerSecondPerSecond(ESTOP_ACCEL);
        stepper_->setTargetPositionToStop();

        homing_state_ = HomingState::STOP_2;
      }
      break;
    case HomingState::STOP_2:
      // Wait for motion to stop, then reset accel
      if (stepper_->motionComplete()) {
        stepper_->setAccelerationInStepsPerSecondPerSecond(ACCEL);
        stepper_->setTargetPositionInSteps(off_box_limit_switch_pos_);
        logIfEnabled("SCS: Second homing complete.");

        // Set sleep pin to low to disable driver
        digitalWrite(SLEEP_PIN, LOW);

        // Recalculate on box limit switch position and then set current motor position to 0
        on_box_limit_switch_pos_ = on_box_limit_switch_pos_ - off_box_limit_switch_pos_;
        off_box_limit_switch_pos_ = 0L;
        open_pos_ = on_box_limit_switch_pos_;
        close_pos_ = off_box_limit_switch_pos_;

        // Set current stepper position to zero, and the target position to zero
        // so the stepper doesn't keep moving
        stepper_->setCurrentPositionInSteps(0L);
        stepper_->setTargetPositionInSteps(0L);

        logIfEnabled(("Second homing completed. On box and off box limit switch positions: (" +
                      String(on_box_limit_switch_pos_) + ", " + String(off_box_limit_switch_pos_) +
                      "). Current stepper pos: " + String(stepper_->getCurrentPositionInSteps()))
                         .c_str());

        homing_state_ = HomingState::COMPLETE;
      }
      break;
    case HomingState::COMPLETE:
      // do nothing
      break;
  }

  // if (!first_homing_complete_) {
  //   if (!first_homing_commanded_) {
  //     logIfEnabled("First homing commanded");
  //     first_homing_commanded_ = true;
  //     stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR);
  //   }
  //   if (command_data.on_box_limit_switch) {
  //     stepper_->setTargetPositionToStop();
  //     // Record the position
  //     on_box_limit_switch_pos_ = stepper_->getCurrentPositionInSteps();
  //     auto msg = "First homing completed. On box limit switch position: " +
  //     String(on_box_limit_switch_pos_); logIfEnabled(msg.c_str()); first_homing_complete_ = true;
  //   }
  // } else if (!second_homing_complete_) {
  //   if (!second_homing_commanded_) {
  //     logIfEnabled("Second homing commanded");
  //     second_homing_commanded_ = true;
  //     stepper_->setTargetPositionRelativeInSteps(HOMING_MAX_DISTANCE * INITIAL_HOMING_DIR * -1);
  //   }
  //   if (command_data.off_box_limit_switch) {
  //     off_box_limit_switch_pos_ = stepper_->getCurrentPositionInSteps();
  //     auto msg2 = "Off box limit switch hit with position: " + String(off_box_limit_switch_pos_);
  //     logIfEnabled(msg2.c_str());

  //     // Command stepper to the limit switch position
  //     stepper_->setTargetPositionInSteps(off_box_limit_switch_pos_);
  //     second_homing_complete_ = true;
  //     logIfEnabled("Second homing completed");
  //   }
  // }

  stepper_->processMovement();

  // if (first_homing_complete_ && second_homing_complete_ && stepper_->motionComplete() &&
  // !recalculate_complete_) {
  //   recalculate_complete_ = true;

  //   // Set sleep pin to low to disable driver
  //   digitalWrite(SLEEP_PIN, LOW);

  //   // Recalculate on box limit switch position and then set current motor position to 0
  //   on_box_limit_switch_pos_ = on_box_limit_switch_pos_ - off_box_limit_switch_pos_;
  //   off_box_limit_switch_pos_ = 0L;
  //   open_pos_ = on_box_limit_switch_pos_;
  //   close_pos_ = off_box_limit_switch_pos_;

  //   // Set current stepper position to zero, and the target position to zero
  //   // so the stepper doesn't keep moving
  //   stepper_->setCurrentPositionInSteps(0L);
  //   stepper_->setTargetPositionInSteps(0L);

  //   auto msg3 = "Second homing completed. On box and off box limit switch positions: (" +
  //   String(on_box_limit_switch_pos_) + ", " + String(off_box_limit_switch_pos_) + "). Current
  //   stepper pos: " + String(stepper_->getCurrentPositionInSteps()); logIfEnabled(msg3.c_str());
  // }

  return handle_transitions(command_data);
}

bool SelfCalibrate::handle_transitions(const CommandData& command_data) {
  bool ret_val = false;

  // If both limit switches are depressed, this is indicative of an
  // error and Error state should be entered.
  if (command_data.on_box_limit_switch && command_data.off_box_limit_switch) {
    logIfEnabled("Both limit switches depressed. Transitioning to error state.");
    next_state = std::make_unique<Error>();
    ret_val = true;
  }

  // Only do nominal transitions if homing is complete
  if (homing_state_ == HomingState::COMPLETE) {
    if (command_data.override_open) {
      logIfEnabled("Transitioning to override open from self calibrate");
      next_state = std::make_unique<OverrideOpenClose>(MotionCmd::OPEN);
      ret_val = true;
    } else if (command_data.override_close) {
      logIfEnabled("Transitioning to override close from self calibrate");
      next_state = std::make_unique<OverrideOpenClose>(MotionCmd::CLOSE);
      ret_val = true;
    } else if (command_data.automatic) {
      logIfEnabled("Transitioning to auto from self calibrate");
      next_state = std::make_unique<Automatic>();
      ret_val = true;
    }
  } else if (command_data.override_close || command_data.override_open || command_data.automatic) {
    // If override close or open or auto commanded before calibration is complete, go to error state
    logIfEnabled(
        "Transitioning to error from self calibrate as transition was commanded before homing was "
        "complete");
    next_state = std::make_unique<Error>();
    ret_val = true;
  }

  if (ret_val) {
    // Turn off LED upon exiting
    digitalWrite(YELLOW_LED, LOW);
  }

  return ret_val;
}