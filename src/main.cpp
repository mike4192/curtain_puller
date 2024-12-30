#include <Arduino.h>
#include "FlexyStepper.h"

#define STEPPER_PIN 0 // GP0
#define DIR_PIN 1 // GP1
#define SPEED 100 // steps/second
#define ACCEL 100 // steps/second/second


FlexyStepper stepper;


// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // TODO: Setup three button input pins for override open,
  // override close, and disable

  // TODO: Setup output LED pins for status
  // Override closed
  // Override open
  // Disabled
  // Motor moving

  // TODO: Setup input pin for IR sensor


  // Set up interrupt for button states
  

  stepper.connectToPins(STEPPER_PIN, DIR_PIN);
  stepper.setSpeedInStepsPerSecond(SPEED);
  stepper.setAccelerationInStepsPerSecondPerSecond(ACCEL);
}

// the loop routine runs over and over again forever:
void loop() {
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);

  delay(1000);               // wait for a second

  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
