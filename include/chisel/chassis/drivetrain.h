#pragma once

#include "../../main.h"

namespace chisel {
    struct DriveTrain {
        pros::MotorGroup *left_motor_group;
        pros::MotorGroup *right_motor_group;

        float wheel_size;
        float track_width;
        float gear_ratio;

        DriveTrain(
            pros::MotorGroup *left_motor_group, pros::MotorGroup *right_motor_group,
            float wheel_size, float track_width, float gear_ratio);
    };
}
