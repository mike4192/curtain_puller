#pragma once

#include "command_data.h"
#include "state.h"

class SelfCalibrate : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;

 private:
  bool first_homing_complete_ = false;
  bool first_homing_commanded_ = false;
  bool second_homing_complete_ = false;
  bool second_homing_commanded_ = false;
};