#pragma once

#include "command_data.h"
#include "state.h"

class OverrideOpenClose : public State {
 public:
  OverrideOpenClose(MotionCmd open_close_cmd_) : open_close_cmd_(open_close_cmd_) {}
  void init() override;
  bool tick(const CommandData& command_data) override;

 private:
  MotionCmd open_close_cmd_;
  MotionState motion_state{MotionState::INIT};
};