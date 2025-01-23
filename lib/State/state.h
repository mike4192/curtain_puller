#pragma once

#include <Arduino.h>

#include <memory>

#include "FlexyStepper.h"
#include "command_data.h"

class State {
 public:
  State(FlexyStepper& stepper) : stepper_(stepper) {}
  virtual void init() {};

  // If tick returns true, it is a signal that the state should be changed and
  // next_state is ready to be used as the next state
  // Pure virtual function to be overridden
  virtual bool tick(const CommandData& command_data) = 0;

  std::unique_ptr<State> next_state = nullptr;

 protected:
  FlexyStepper& stepper_;

  // Measured open position
  long open_pos_ = 2100L; 
  long open_slowdown_pos_ = 1700L;
  // Position to command when opening, intentionally
  // overshooting known open position
  long open_overshoot_pos_ = 2200L;
  long close_pos_ = 0L;
  long close_overshoot_pos_ = -100L;
  long close_slowdown_pos_ = 300L;
};

class Init : public State {
 public:
  using State::State;
  bool tick(const CommandData& command_data) override;

 private:
  unsigned long led_toggle_time = 0UL;
};

class SelfCalibrate : public State {
 public:
  using State::State;
  void init() override;
  bool tick(const CommandData& command_data) override;
};

class OverrideOpen : public State {
 public:
  using State::State;
  void init() override;
  bool tick(const CommandData& command_data) override;
};

class OverrideClose : public State {
 public:
  using State::State;
  void init() override;
  bool tick(const CommandData& command_data) override;
};

class Automatic : public State {
 public:
  using State::State;
  void init() override;
  bool tick(const CommandData& command_data) override;
};
