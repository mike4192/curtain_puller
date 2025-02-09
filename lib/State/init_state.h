#pragma once

#include "command_data.h"
#include "state.h"

class Init : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;

 private:
  unsigned long led_toggle_time = 0UL;
};