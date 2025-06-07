#include "../../../include/chisel/chassis/drivetrain.h"

namespace chisel {
    DriveTrain::DriveTrain(pros::MotorGroup *left_motor_group, pros::MotorGroup *right_motor_group,
                           const float wheel_size, const float track_width, const float gear_ratio)
        : left_motor_group(left_motor_group), right_motor_group(right_motor_group),
          wheel_size(wheel_size), track_width(track_width), gear_ratio(gear_ratio) {
    }
}
