#pragma once

#include <memory>

#include "FlexyStepper.h"
#include "command_data.h"

enum class MotionState { INIT, ACCELERATE, FAST_MOTION, CREEP_MOTION, COMPLETE, ESTOP };

enum class MotionCmd { OPEN, CLOSE };

// Base State class definition. Constructor receives a pointer to a flexy stepper object
class State {
 public:
  static void setStepper(FlexyStepper* stepper) { stepper_ = stepper; }

  virtual void init() {};

  // If tick returns true, it is a signal that the state should be changed and
  // next_state is ready to be used as the next state
  // Pure virtual function to be overridden
  virtual bool tick(const CommandData& command_data) = 0;

  std::unique_ptr<State> next_state = nullptr;

 protected:
  MotionState motion_state_{MotionState::INIT};

  void process_motion_profile(MotionCmd cmd, MotionCmd prev_cmd, const CommandData cmd_data);
  MotionState check_limit_switches(const MotionState motion_state, const CommandData cmd_data, MotionCmd cmd);
  static FlexyStepper* stepper_;

  // Measured open position
  static long on_box_limit_switch_pos_;
  static long off_box_limit_switch_pos_;
  
  static long open_pos_;
  static long close_pos_;
};
