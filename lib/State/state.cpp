#include "state.h"

// Initialize the static stepper pointer. No protections are put in
// place on this pointer as we assume it is properly assigned with a real object
// at the beginning of the program.
FlexyStepper* State::stepper_ = nullptr;
