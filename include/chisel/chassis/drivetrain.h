#pragma once

#include "main.h"

namespace Chassis {
    struct DriveTrain {
        pros::MotorGroup *left_motors;
        pros::MotorGroup *right_motors;

        float wheel_size;
        float track_width;
        float gear_ratio;

        DriveTrain(
            pros::MotorGroup *left_motors, pros::MotorGroup *right_motors,
            float wheel_size, float track_width, float gear_ratio
        );
    };
}
