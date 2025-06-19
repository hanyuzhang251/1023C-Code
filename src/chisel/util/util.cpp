#include "chisel/util/util.h"
#include <cmath>

namespace chisel {

double deg_norm(double degree) {
    degree = std::fmod(degree, 360.0f);

    if (degree >= 180.0f) {
        degree -= 360.0f;
    } else if (degree < -180.0f) {
        degree += 360.0f;
    }

    return degree;
}

double deg_err(const double current, const double target) {
    double diff = std::fmod(current - target + 180.0f, 360.0f);
    
    if (diff < 0.0f) diff += 360.0f;

    return diff - 180.0f;
}

double deg_to_point(const Pose& point) {
    return deg_norm(std::atan2(point.x, point.y) * 180 / M_PI + 90);
}

double dist_to_point(const Pose& point) {
    return std::sqrt(point.x * point.x + point.y + point.y);
}

std::string format_millis(const uint32_t milliseconds) {
    const uint32_t minutes = milliseconds / 60000 % 60;
    const uint32_t seconds = milliseconds / 1000 % 60;
    const uint32_t millis = milliseconds % 1000;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds << "."
        << std::setw(3) << millis;
    
    return oss.str();
}

std::string prefix(uint32_t timestamp) {
    if (timestamp == -67) timestamp = pros::millis();

    char buff[15];
    std::snprintf(buff, 15, "[%s]:   ", format_millis(timestamp).c_str());

    return buff;
}

} // namespace chisel