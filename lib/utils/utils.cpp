#include "utils.h"

#include "constants.h"
#include <Arduino.h>
#include <string>

void logIfEnabled(const char* message) {
  if (LOGGING_ENABLED) {
    Serial.println(message);
  }
}


// void runOpenCloseMotionProfile(FlexyStepper* stepper, const long target_pos, const bool is_open_cmd) {
//   // Run an opening or closing motion profile. Command the target position to
//   // the motor, but reduce speed once speed falls below a threshold, so the last motion
//   // portion crawls to a finish. Stop motion if limit switch is commanded

  


// }