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

        PIDController(const float kp, const float ki, const float kd, const float tolerance,
                      const float wind, const float clamp, const float slew,
                      const float small_error, const float large_error)
            : kp(kp), ki(ki), kd(kd), tolerance(tolerance),
              wind(wind), clamp(clamp), slew(slew),
              small_error(small_error), large_error(large_error) {
        }

        void update() {
            // Convert error to float. Point of this is to only convert in the controller.
            const auto errorF = static_cast<float>(error);

            // Update prev values.
            prev_error = errorF;
            prev_output = static_cast<float>(output);

            // If we crossed the target, we should set the integral to zero.
            if (sgn(prev_error) != sgn(error)) {
                integral = 0;
            }

            // If the integral is in the windup range, update integral.
            if (std::abs(error) <= wind) {
                integral += errorF;
            }
            // Otherwise, set integral to zero.
            else {
                integral = 0;
            }

            // Clamp the integral.
            integral = clamp(integral, -clamp, clamp);

            // Update derivative
            derivative = errorF - prev_error;

            // Copy integral and derivative to avoid messing with originals.
            float real_integral = integral;
            float real_derivative = derivative;

            // Integral *= [error=0.0 -> error=small_error : 0.3 -> 1.0]
            real_integral *= std::min(1.0f, std::max(0.3f, std::abs(errorF) / small_error));
            // Derivative *= [error=0.0 -> error=large_error : 1.0 -> 0.0]
            real_derivative *= 1 - std::min(1.0f, std::abs(errorF) / large_error);

            // Calculate power.
            float calc_power = errorF * kp + real_integral * ki + real_derivative * kd;

            // Clamp power to slew.
            calc_power = clamp(calc_power, prev_output - slew, prev_output + slew);
            // Clamp power to min/max speed

            output = calc_power;
        }
    };
} // namespace chisel
