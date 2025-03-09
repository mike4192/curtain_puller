#include "state_controller.h"

#include "utils.h"

void StateController::initialize(std::unique_ptr<State> initial_state) {
  state_ = std::move(initial_state);
  state_->init();
}

void StateController::tick(const CommandData& command_data) {
  if (state_->tick(command_data)) {
    if (state_->next_state == nullptr) {
      logIfEnabled("Error: next_state is nullptr, not changing state.");
    } else {
      changeState(std::move(state_->next_state));
    }
  };
}

void StateController::changeState(std::unique_ptr<State> newState) {
  state_ = std::move(newState);
  state_->init();
}