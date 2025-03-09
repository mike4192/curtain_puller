#pragma once

#include "command_data.h"
#include "state.h"

class Disabled : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;
 
 private:
  unsigned long led_toggle_time_ms{0U};
};