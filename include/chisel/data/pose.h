#pragma once

#include <atomic>
#include <tuple>

namespace chisel {

struct Pose {
    double x = 0;
    double y = 0;
    double h = 0;

    Pose(const double x, const double y, const double h): x(x), y(y), h(h) {}

    Pose() = default;

    Pose operator+(const Pose &other) const {
        return {x + other.x, y + other.y, h + other.h};
    }

    Pose operator-(const Pose &other) const {
        return {x - other.x, y - other.y, h - other.h};
    }

    Pose& operator=(const Pose &other) = default;
};

} // namespace chisel