#include "utils.h"

#include <Arduino.h>

#include <string>

#include "constants.h"

void logIfEnabled(const char* message) {
  if (LOGGING_ENABLED) {
    Serial.println(message);
  }
}

void disable_driver() { digitalWrite(SLEEP_PIN, HIGH); }

void enable_driver() { digitalWrite(SLEEP_PIN, LOW); }

bool is_driver_enabled() { return digitalRead(SLEEP_PIN) == LOW; }