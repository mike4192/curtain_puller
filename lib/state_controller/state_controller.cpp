#include "state_controller.h"

void StateController::tick(const CommandData& command_data) {
    if (state_->tick(command_data)) {
        if (state_->next_state == nullptr) {
            Serial.println("Error: next_state is nullptr, not changing state.");
        } else {
            changeState(std::move(state_->next_state));
        }
    };
}

void StateController::changeState(std::unique_ptr<State> newState) {
    state_ = std::move(newState);
    state_->init();
}