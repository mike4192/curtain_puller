#pragma once

#include "command_data.h"
#include "state.h"

// class OverrideOpen : public State {
//  public:
//   void init() override;
//   bool tick(const CommandData& command_data) override;
// };

// class OverrideClose : public State {
//  public:
//   void init() override;
//   bool tick(const CommandData& command_data) override;
// };

class OverrideOpenClose : public State {
 public:
  OverrideOpenClose(bool is_override_open_commanded) : is_override_open_commanded_(is_override_open_commanded) {}
  void init() override;
  bool tick(const CommandData& command_data) override;
 private:
  bool is_override_open_commanded_;
};