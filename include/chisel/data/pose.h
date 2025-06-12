#pragma once

#include <atomic>
#include <cfloat>
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

    static Pose sub(const Pose &a, const Pose &b) {
        return {a.x - b.x, a.y - b.y, a.h - b.h};
    }

    static Pose add(const Pose &a, const Pose &b) {
        return {a.x + b.x, a.y + b.y, a.h + b.h};
    }

    static double dot(const Pose &a, const Pose &b) {
        return a.x * b.x + a.y * b.y;
    }
};

} // namespace chisel