#pragma once

#include <memory>

#include "FlexyStepper.h"
#include "command_data.h"

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
  static FlexyStepper* stepper_;

  // Measured open position
  long on_box_limit_switch_pos_ = 0L; // TODO: Change name to open or close
  long off_box_limit_switch_pos_ = 0L; // TODO: Change name to open or close
  
  long open_slowdown_pos_ = 1700L;

  long open_overshoot_pos_ = 2200L;
  long close_pos_ = 0L;
  long close_overshoot_pos_ = -100L;
  long close_slowdown_pos_ = 300L;
};
