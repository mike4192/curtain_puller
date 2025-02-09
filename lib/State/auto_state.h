#pragma once

#include "command_data.h"
#include "state.h"

class Automatic : public State {
 public:
  void init() override;
  bool tick(const CommandData& command_data) override;
};