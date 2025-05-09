#include "auto_state.h"

#include "constants.h"
#include "disabled_state.h"
#include "override_open_close_state.h"
#include "utils.h"

void Automatic::init() {
  logIfEnabled("In automatic init");
  digitalWrite(BLUE_LED, HIGH);
  motor_run_start_time_ms_ = 0UL;  // Reset motor run timer
  motor_was_running_ = false;      // Reset motor running state
}

bool Automatic::tick(const CommandData& command_data) {
  MotionCmd current_motion_cmd{MotionCmd::CLOSE};

  if (command_data.ir_sensor_short_wire != prev_ir_sense_short) {
    if (command_data.ir_sensor_short_wire) {
      logIfEnabled("Short wire IR state changed true");
    } else {
      logIfEnabled("Short wire IR state changed false");
    }
  }

  if (command_data.ir_sensor_long_wire != prev_ir_sense_long) {
    if (command_data.ir_sensor_long_wire) {
      logIfEnabled("Long wire IR state changed true");
    } else {
      logIfEnabled("Long wire IR state changed false");
    }
  }

  prev_ir_sense_short = command_data.ir_sensor_short_wire;
  prev_ir_sense_long = command_data.ir_sensor_long_wire;

  if (command_data.ir_sensor_short_wire || command_data.ir_sensor_long_wire) {
    current_motion_cmd = MotionCmd::OPEN;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    current_motion_cmd = MotionCmd::CLOSE;
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  process_motion_profile(current_motion_cmd, prev_motion_cmd_, command_data);
  prev_motion_cmd_ = current_motion_cmd;

  // Safety check that limits motor continuous run time
  bool motor_runtime_exceeded = false;
  if (is_driver_enabled()) {
    if (!motor_was_running_) {
      // Motor just started
      motor_run_start_time_ms_ = millis();
      motor_was_running_ = true;
      logIfEnabled("Motor started, tracking run time.");
    } else {
      // Motor has been running, check for timeout
      if (millis() - motor_run_start_time_ms_ > MAX_MOTOR_CONTINUOUS_RUN_TIME_MS) {
        logIfEnabled(
            "Motor run time exceeded. Disabling driver and transitioning to Disabled state.");
        disable_driver();
        motor_runtime_exceeded = true;
      }
    }
  } else {
    if (motor_was_running_) {
      logIfEnabled("Motor stopped, resetting run time tracking.");
    }
    motor_was_running_ = false;  // Motor is not running
  }

  // Handle Transitions
  bool ret_val = false;
  if (command_data.override_open) {
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::OPEN);
    ret_val = true;
  } else if (command_data.override_close) {
    next_state = std::make_unique<OverrideOpenClose>(MotionCmd::CLOSE);
    ret_val = true;
  } else if (command_data.automatic || motor_runtime_exceeded) {
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