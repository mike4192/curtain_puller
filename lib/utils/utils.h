#pragma once

#include <Arduino.h>

#include <string>

#include "FlexyStepper.h"
#include "constants.h"

void logIfEnabled(const char* message);

void disable_driver();

void enable_driver();

bool is_driver_enabled();