#pragma once

#include "../../main.h"
#include "chisel/pid.h"

namespace chisel {

    /**
     * This struct encapsulates the properties and configuration of a drivetrain
     * including motor groups, wheel size, track width, and gear ratio.
     */
    struct DriveTrain {
        pros::MotorGroup *left_motor_group;
        pros::MotorGroup *right_motor_group;

        PIDController *lateral_pid_controller;
        PIDController *angular_pid_controller;

        double wheel_size;
        double track_width;
        double gear_ratio;

        /**
         * DriveTrain constructor
         *
         * @param left_motor_group pointer to MotorGroup of left motors
         * @param right_motor_group pointer to MotorGroup of right motors
         * @param wheel_size size of wheels
         * @param track_width distance between the centers of the wheels
         * @param gear_ratio ratio of the gearbox to the wheels
         * @param lateral_pid_controller pointer to lateral pid controller
         * @param angular_pid_controller pointer to angular pid controller
         */
        DriveTrain(
            pros::MotorGroup *left_motor_group, pros::MotorGroup *right_motor_group,
            double wheel_size, double track_width, double gear_ratio,
            PIDController *lateral_pid_controller, PIDController *angular_pid_controller);
    };
}
