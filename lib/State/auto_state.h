#pragma once

#include "command_data.h"
#include "state.h"

class Automatic : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;

 private:
  MotionCmd prev_motion_cmd_{MotionCmd::CLOSE};
  bool prev_ir_sense_short{false};
  bool prev_ir_sense_long{false};
};