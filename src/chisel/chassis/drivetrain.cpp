#include "../../../include/chisel/chassis/drivetrain.h"

namespace chisel {
    DriveTrain::DriveTrain(pros::MotorGroup *left_motor_group, pros::MotorGroup *right_motor_group,
                           const double wheel_size, const double track_width, const double gear_ratio,
                           PIDController *lateral_pid_controller, PIDController *angular_pid_controller)
        : left_motor_group(left_motor_group), right_motor_group(right_motor_group),
          wheel_size(wheel_size), track_width(track_width), gear_ratio(gear_ratio),
          lateral_pid_controller(lateral_pid_controller), angular_pid_controller(angular_pid_controller) {
    }
}
