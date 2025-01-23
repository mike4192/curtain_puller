#pragma once

// #include "constants.h"

struct CommandData {
    volatile bool override_open = false;
    volatile bool override_close = false;
    volatile bool automatic = false;
    volatile bool ir_sensor_1 = false;
    volatile bool ir_sensor_2 = false;
    volatile bool open_limit_switch = false;
    volatile bool close_limit_switch = false;

    void setOverrideOpen() {
        override_open = true;
        override_close = false;
        automatic = false;
    }

    void setOverrideClose() {
        override_close = true;
        override_open = false;
        automatic = false;
    }

    void setAutomaticOrDisable() {
        // Automatic and disable button serves double duty.
        automatic = true;
        override_open = false;
        override_close = false;
    }

    void clearButtonStates() {
        override_open = false;
        override_close = false;
        automatic = false;
    }
};