/* 
 * Basic State Controller to manage ticking and changing states for a finite state machine.
 * A state transition table is not defined, instead, state transitions are delegated to
 * individual states to define. The risk of bad state transitions is aknowledged in this design
 * but accepted as a tradeoff for simplicity and flexibility.
 */

#pragma once

#include <memory>

#include "command_data.h"
#include "state.h"
// #include "FlexyStepper.h"

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