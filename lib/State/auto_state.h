#pragma once

#include "command_data.h"
#include "constants.h"
#include "state.h"
#include "utils.h"

class Automatic : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;

 private:
  MotionCmd prev_motion_cmd_{MotionCmd::CLOSE};
  bool prev_ir_sense_short{false};
  bool prev_ir_sense_long{false};
  unsigned long motor_run_start_time_ms_{0UL};
  bool motor_was_running_{false};
};