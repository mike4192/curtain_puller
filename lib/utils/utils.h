#pragma once

#include "constants.h"
#include <Arduino.h>
#include "FlexyStepper.h"
#include <string>

void logIfEnabled(const char* message);

// void runOpenCloseMotionProfile(FlexyStepper* stepper, const long target_pos, const bool is_open_cmd);