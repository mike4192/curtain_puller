#pragma once

#include "command_data.h"
#include "state.h"

enum class HomingState { INIT, HOMING_1, STOP_1, HOMING_2, STOP_2, COMPLETE };

class SelfCalibrate : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;

 private:
 bool handle_transitions(const CommandData& command_data);
//  bool first_homing_commanded_ = false;
//   bool first_homing_complete_ = false;
//   bool second_homing_commanded_ = false;
//   bool second_homing_complete_ = false;
//   bool recalculate_complete_ = false;

  HomingState homing_state_{HomingState::INIT};
};