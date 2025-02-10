#include "auto_state.h"

#include "constants.h"
#include "override_open_close_state.h"

void Automatic::init() {
  Serial.println("In automatic init");
  digitalWrite(BLUE_LED, HIGH);
}

bool Automatic::tick(const CommandData& command_data) {
  if (command_data.ir_sensor_short_wire) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  if (command_data.override_open || command_data.override_close) {
    if (command_data.override_open) {
      next_state = std::make_unique<OverrideOpenClose>(true);
    } else if (command_data.override_close) {
      next_state = std::make_unique<OverrideOpenClose>(false);
    }
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    return true;
  }
  return false;
}