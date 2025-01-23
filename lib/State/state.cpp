#include <Arduino.h>
#include "state.h"
#include "constants.h"

bool Init::tick(const CommandData& command_data) {
    if (millis() - led_toggle_time > 250) {
        led_toggle_time = millis();
        digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
        digitalWrite(RED_LED, !digitalRead(RED_LED));
        digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
        digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
    }

    if (command_data.automatic) {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        next_state = std::make_unique<SelfCalibrate>(stepper_);
        return true;
    }
    return false;
}

void SelfCalibrate::init() {
    Serial.println("In calibrate state init");
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
}

bool SelfCalibrate::tick(const CommandData& command_data) {
    if (command_data.override_open) {
        next_state = std::make_unique<OverrideOpen>(stepper_);
        digitalWrite(YELLOW_LED, LOW);
        return true;
    } else if (command_data.override_close) {
        next_state = std::make_unique<OverrideClose>(stepper_);
        digitalWrite(YELLOW_LED, LOW);
        return true;
    }
    return false;
}

void OverrideOpen::init() {
    Serial.println("In override open init");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH); // Yellow LED to indicate movement 
    // Set target open position.
    stepper_.setTargetPositionInSteps(open_overshoot_pos_);
    stepper_.setSpeedInStepsPerSecond(FAST_SPEED);
    // Set sleep pin to HIGH to enable driver and wait at least 1 ms
    digitalWrite(SLEEP_PIN, HIGH);
    delay(5); // 5 ms
}

bool OverrideOpen::tick(const CommandData& command_data) {

    if (!stepper_.motionComplete()) {
        // When stepper passes threshold, slow it down
        if (stepper_.getCurrentPositionInSteps() > open_slowdown_pos_) {
            stepper_.setSpeedInStepsPerSecond(SLOW_SPEED);
        }

        // If stepper hits limit switch, stop motor
        // (still subject to acceleration)
        if (command_data.open_limit_switch) {
            stepper_.setTargetPositionToStop();
        }

        stepper_.processMovement();
    } else {
        // Set driver board to sleep
        digitalWrite(SLEEP_PIN, LOW);
        digitalWrite(YELLOW_LED, LOW);
    }

    if (command_data.override_close || command_data.automatic) {
      if (command_data.override_close) {
        next_state = std::make_unique<OverrideClose>(stepper_);
      } else if (command_data.automatic) {
        next_state = std::make_unique<Automatic>(stepper_);
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
    digitalWrite(YELLOW_LED, HIGH); // Yellow LED to indicate movement 
    // Set target open position.
    stepper_.setTargetPositionInSteps(close_overshoot_pos_);
    stepper_.setSpeedInStepsPerSecond(FAST_SPEED);
    // Set sleep pin to HIGH to enable driver and wait at least 1 ms
    digitalWrite(SLEEP_PIN, HIGH);
    delay(5); // 5 ms
}

bool OverrideClose::tick(const CommandData& command_data) {
if (!stepper_.motionComplete()) {
        // When stepper passes threshold, slow it down
        if (stepper_.getCurrentPositionInSteps() < close_slowdown_pos_) {
            stepper_.setSpeedInStepsPerSecond(SLOW_SPEED);
        }

        // If stepper hits limit switch, stop motor
        // (still subject to acceleration)
        if (command_data.on_box_limit_switch) {
            stepper_.setTargetPositionToStop();
        }

        stepper_.processMovement();
    } else {
        // Set driver board to sleep
        digitalWrite(SLEEP_PIN, LOW);
        digitalWrite(YELLOW_LED, LOW);
    }

  if (command_data.override_open || command_data.automatic) {
    if (command_data.override_open) {
      next_state = std::make_unique<OverrideOpen>(stepper_);
    } else if (command_data.automatic) {
      next_state = std::make_unique<Automatic>(stepper_);
    }
    digitalWrite(RED_LED, LOW);
    return true;
  }
  return false;
}

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
    if (command_data.override_open || command_data.override_close) {
        if (command_data.override_open) {next_state = std::make_unique<OverrideOpen>(stepper_);}
        else if (command_data.override_close) {next_state = std::make_unique<OverrideClose>(stepper_);}
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, LOW);
        return true;
    }
    return false;   
}