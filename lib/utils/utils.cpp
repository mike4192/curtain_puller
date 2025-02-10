#include "utils.h"

#include "constants.h"
#include <Arduino.h>
#include <string>

void logIfEnabled(const char* message) {
  if (LOGGING_ENABLED) {
    Serial.println(message);
  }
}