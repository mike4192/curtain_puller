#pragma once

#include "command_data.h"
#include "state.h"

// enum class MotionState { INIT, ACCELERATE, FAST_MOTION, CREEP_MOTION, COMPLETE, ESTOP };

class OverrideOpenClose : public State {
 public:
  OverrideOpenClose(MotionCmd cmd) : cmd(cmd) {}
  void init() override;
  bool tick(const CommandData& command_data) override;
 private:

  // MotionState check_limit_switches(const MotionState motion_state, const CommandData cmd_data);
  MotionCmd cmd;

  bool limit_switch_hit_{false};

  MotionState motion_state{MotionState::INIT};
};