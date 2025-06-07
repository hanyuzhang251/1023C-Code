#pragma once

    #include "../../main.h"

namespace chisel {
    struct DriveTrain {
        std::vector<pros::Motor*> left_motors;
        std::vector<pros::Motor*> right_motors;;

        float wheel_size;
        float track_width;
        float gear_ratio;

        DriveTrain(
            std::vector<pros::Motor*>&& left_motors, std::vector<pros::Motor*>&& right_motors,
            float wheel_size, float track_width, float gear_ratio
        );
    };
}
