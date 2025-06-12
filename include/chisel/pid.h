#pragma once

#include "util/util.h"

#include <functional>
#include <cfloat>

namespace chisel {
    struct PIDController {
        float kp;
        float ki;
        float kd;
        float tolerance;
        float wind;
        float clamp;
        float slew;
        float small_error;
        float large_error;

        double error = 0;
        double output = 0;

        float prev_output = 0;
        float prev_error = 0;
        float integral = 0;
        float derivative = 0;

        PIDController(float kp, float ki, float kd, float tolerance,
                      float wind, float clamp, float slew,
                      float small_error, float large_error);

        void update();
    };
} // namespace chisel
