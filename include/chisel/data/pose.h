#pragma once

#include <atomic>
#include <tuple>

namespace chisel {

struct Pose {
    float x = 0;
    float y = 0;
    float h = 0;

    Pose(const float x, const float y, const float h): x(x), y(y), h(h) {}

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