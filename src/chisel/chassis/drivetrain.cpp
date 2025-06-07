#include "../../../include/chisel/chassis/drivetrain.h"

namespace chisel {
    DriveTrain::DriveTrain(std::vector<pros::Motor *>&& left_motors, std::vector<pros::Motor *>&& right_motors,
                           const float wheel_size, const float track_width, const float gear_ratio)
        : left_motors(std::move(left_motors)), right_motors(std::move(right_motors)),
          wheel_size(wheel_size), track_width(track_width), gear_ratio(gear_ratio) {
    }
}
