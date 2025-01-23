#pragma once

// #include "constants.h"

struct CommandData {
    bool override_open = false;
    bool override_close = false;
    bool automatic = false;
    bool ir_sensor_short_wire = false;
    bool ir_sensor_long_wire = false;
    bool open_limit_switch = false;
    bool on_box_limit_switch = false;
    bool off_box_limit_switch = false;


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