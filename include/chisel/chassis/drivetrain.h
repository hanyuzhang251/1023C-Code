#pragma once

#include "../../main.h"

namespace chisel {

    /**
     * This struct encapsulates the properties and configuration of a drivetrain
     * including motor groups, wheel size, track width, and gear ratio.
     */
    struct DriveTrain {
        pros::MotorGroup *left_motor_group;
        pros::MotorGroup *right_motor_group;

        double wheel_size;
        double track_width;
        double gear_ratio;

        /**
         * DriveTrain constructor
         *
         * @param left_motor_group pointer to MotorGroup of left motors
         * @param right_motor_group pointer to MotorGroup of right motors
         * @param wheel_size
         * @param track_width
         * @param gear_ratio
         */
        DriveTrain(
            pros::MotorGroup *left_motor_group, pros::MotorGroup *right_motor_group,
            double wheel_size, double track_width, double gear_ratio);
    };
}
