#pragma once

#include "../../main.h"

#include <cstdint>
#include <cstring>

namespace chisel {

    struct DeviceMetadata {
        static constexpr uint8_t MAX_NAME_LENGTH = 12;

        static constexpr uint8_t STATUS_NA = 0;
        static constexpr uint8_t STATUS_NONE = 1;
        static constexpr uint8_t STATUS_OK = 2;
        static constexpr uint8_t STATUS_ERROR = 3;

        static constexpr uint8_t STATUS_HOT = 4;
        static constexpr uint8_t STATUS_BURNT = 5;
        static constexpr uint8_t STATUS_BAKED = 6;

        static constexpr const char* STATUS_TEXT[] = {
            "n/a",
            "none",
            "ok",
            "ERROR",
            "HOT",
            "BURNT",
            "BAKED"
        };

        pros::v5::Device *device;
        char name[MAX_NAME_LENGTH];

        uint8_t status = 0;

        DeviceMetadata(const char *name, pros::v5::Device* device) {
            std::strncpy(this->name, name, MAX_NAME_LENGTH - 1);
            this->name[MAX_NAME_LENGTH - 1] = '\0';

            if (device == nullptr) {
                throw std::invalid_argument("Device pointer cannot be null");
            }
            this->device = device;
        }
    };

}