#include "pid.h"

namespace chisel {
    PIDSetting::PIDSetting(
        const double kp, const double ki, const double kd,
        const double tolerance,
        const double wind, const double clamp, const double slew,
        const double small_error, const double large_error
    ): kp(kp), ki(ki), kd(kd), tolerance(tolerance),
       wind(wind), clamp(clamp), slew(slew),
       small_error(small_error), large_error(large_error) {
    }

    PIDController::PIDController(
        const PIDSetting &pid,
        const std::function<double(double, double)> &normalize_err): pid(pid), normalize_err(normalize_err) {
    }

    void PIDController::update() {


    }
} // namespace chisel
