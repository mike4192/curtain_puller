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
