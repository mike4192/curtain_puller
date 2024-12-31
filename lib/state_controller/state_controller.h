#pragma once

#include <memory>

#include "command_data.h"
#include "state.h"
#include "FlexyStepper.h"

class StateController {
 public:
  StateController(std::unique_ptr<State> initial_state)
      : state_(std::move(initial_state)) {
    state_->init();
  }
  void tick(const CommandData& command_data);

 private:
  // Pointer to state object
  std::unique_ptr<State> state_;

  void changeState(std::unique_ptr<State> newState);
};