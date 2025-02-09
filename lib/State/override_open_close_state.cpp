#include "override_open_close_state.h"

#include "FlexyStepper.h"
#include "auto_state.h"
#include "constants.h"
#include "override_open_close_state.h"

void OverrideOpen::init() {
  Serial.println("In override open init");
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);  // Yellow LED to indicate movement
  // Set target open position.
  stepper_->setTargetPositionInSteps(open_overshoot_pos_);
  stepper_->setSpeedInStepsPerSecond(FAST_SPEED);
  // Set sleep pin to HIGH to enable driver and wait at least 1 ms
  digitalWrite(SLEEP_PIN, HIGH);
  delay(5);  // 5 ms
}

bool OverrideOpen::tick(const CommandData& command_data) {
  if (!stepper_->motionComplete()) {
    // When stepper passes threshold, slow it down
    if (stepper_->getCurrentPositionInSteps() > open_slowdown_pos_) {
      stepper_->setSpeedInStepsPerSecond(SLOW_SPEED);
    }

    // If stepper hits limit switch, stop motor
    // (still subject to acceleration)
    if (command_data.open_limit_switch) {
      stepper_->setTargetPositionToStop();
    }

    stepper_->processMovement();
  } else {
    // Set driver board to sleep
    digitalWrite(SLEEP_PIN, LOW);
    digitalWrite(YELLOW_LED, LOW);
  }

  if (command_data.override_close || command_data.automatic) {
    if (command_data.override_close) {
      next_state = std::make_unique<OverrideClose>();
    } else if (command_data.automatic) {
      next_state = std::make_unique<Automatic>();
    }
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    return true;
  }
  return false;
}

void OverrideClose::init() {
  Serial.println("In override close init");
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);  // Yellow LED to indicate movement
  // Set target open position.
  stepper_->setTargetPositionInSteps(close_overshoot_pos_);
  stepper_->setSpeedInStepsPerSecond(FAST_SPEED);
  // Set sleep pin to HIGH to enable driver and wait at least 1 ms
  digitalWrite(SLEEP_PIN, HIGH);
  delay(5);  // 5 ms
}

bool OverrideClose::tick(const CommandData& command_data) {
  if (!stepper_->motionComplete()) {
    // When stepper passes threshold, slow it down
    if (stepper_->getCurrentPositionInSteps() < close_slowdown_pos_) {
      stepper_->setSpeedInStepsPerSecond(SLOW_SPEED);
    }

    // If stepper hits limit switch, stop motor
    // (still subject to acceleration)
    if (command_data.on_box_limit_switch) {
      stepper_->setTargetPositionToStop();
    }

    stepper_->processMovement();
  } else {
    // Set driver board to sleep
    digitalWrite(SLEEP_PIN, LOW);
    digitalWrite(YELLOW_LED, LOW);
  }

  if (command_data.override_open || command_data.automatic) {
    if (command_data.override_open) {
      next_state = std::make_unique<OverrideOpen>();
    } else if (command_data.automatic) {
      next_state = std::make_unique<Automatic>();
    }
    digitalWrite(RED_LED, LOW);
    return true;
  }
  return false;
}