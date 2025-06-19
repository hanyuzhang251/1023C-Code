#pragma once

#include "main.h"
#include "chisel/data/pose.h"
#include <iomanip>
#include <sstream>
#include "chisel/config.h"
#include <cstdint>
#include <queue>

#include "chisel/logger/logger.h"

#define digi_button controller_digital_e_t
#define anlg_button controller_analog_e_t

#define CTRL_ANLG_LX E_CONTROLLER_ANALOG_LEFT_X
#define CTRL_ANLG_LY E_CONTROLLER_ANALOG_LEFT_Y

#define CTRL_ANLG_RX E_CONTROLLER_ANALOG_RIGHT_X
#define CTRL_ANLG_RY E_CONTROLLER_ANALOG_RIGHT_Y

#define CTRL_DIGI_L1 E_CONTROLLER_DIGITAL_L1
#define CTRL_DIGI_L2 E_CONTROLLER_DIGITAL_L2
#define CTRL_DIGI_R1 E_CONTROLLER_DIGITAL_R1
#define CTRL_DIGI_R2 E_CONTROLLER_DIGITAL_R2

#define CTRL_DIGI_A E_CONTROLLER_DIGITAL_A
#define CTRL_DIGI_B E_CONTROLLER_DIGITAL_B
#define CTRL_DIGI_X E_CONTROLLER_DIGITAL_X
#define CTRL_DIGI_Y E_CONTROLLER_DIGITAL_Y

#define CTRL_DIGI_UP E_CONTROLLER_DIGITAL_UP
#define CTRL_DIGI_DOWN E_CONTROLLER_DIGITAL_DOWN
#define CTRL_DIGI_LEFT E_CONTROLLER_DIGITAL_LEFT
#define CTRL_DIGI_RIGHT E_CONTROLLER_DIGITAL_RIGHT

/**
 * @brief Analogous to pros::delay()
 *
 * @param delta Time to wait in milliseconds
 */
inline void wait(const uint32_t delta) {
    pros::delay(delta);
}

namespace chisel {

    enum class State {
        Init,
        Crashout,
        Auton,
        Drive,
        End
    };

    /**
     * @brief Finds the sign of the given value.
     *
     * @tparam T Type of the given value.
     * @param val given value to find the sign of.
     * @return An int in the range [-1, 1] describing the sign of the value.
     *
     * @note Will reset in a compile time error if the type of the value does not support required operators.
     */
    template<typename T>
    int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    /**
     * @brief Clamps a given value to a given range.
     *
     * @tparam T Type of the given value.
     * @param value Value to clamp.
     * @param min Min value.
     * @param max Max value.
     * @return The clamped value.
     */
    template<typename T>
    T clamp(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    /**
     * @brief Clamps a given value to a given range.
     *
     * @tparam T Type of the given value.
     * @param value Value to clamp.
     * @param min Min value.
     * @param max Max value.
     * @return The clamped value.
     */
    template<typename T>
    T clamp_(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    /**
     * @brief Clamps the given value to a given absolute range.
     *
     * If the range was [+20, +50], the value would be clamped to the ranges [-50, -20] and [+20, +50], based on the sign.
     * Because of this, both min and max values are expected to be positive.
     *
     * @tparam T Type of the given value.
     * @param value Value to clamp.
     * @param min Absolute min value.
     * @param max Absolute max value.
     * @return The clamped value.
     */
    template<typename T>
    T abs_clamp(T value, T min, T max) {
        const T abs_val = clamp(std::abs(value), min, max);

        return abs_val * sgn(value);
    }

    /**
     * @brief Normalizes a given degree to the range [-180.0f, +180.0f]
     *
     * @param degree Degree to normalize.
     * @return The normalized degree.
     */
    double deg_norm(double degree);

    /**
     * @brief Finds the error between a current and target degree.
     *        The error is signed, so the order of parameters must be followed.
     *
     * If the current degree was +45,0f and the target degree was +90,0f, the error would be +45.0f
     * On th eother hand if the target degree of 0.0f, the error would be -45.0f.
     *
     * @param current Current degree.
     * @param target Target degree.
     * @return Degree error, normalized to the range [-180.0f, +180.0f]
     */
    double deg_err(double current, double target);

    /**
     * @brief Finds the degree to a given point with the atan2 function.
     *
     * @param point Reference to the point. Stored in a Pose object though its heading is ignored.
     * @return The degree to the point.
     */
    double deg_to_point(const Pose &point);

    /**
     * @brief Finds the distance to a give point using the Pythagorean theorem.
     *
     * The point is assumed to be relative, as in the function will not account for the current robot position.
     *
     * @param point
     * @return
     */
    double dist_to_point(const Pose &point);

    /**
     * @brief Formates a provided millisecond value to mm:ss:SSS
     *
     * @param milliseconds Milliseconds to format.
     * @return Formated string as a std::string.
     */
    std::string format_millis(uint32_t milliseconds);

    /**
     * @brief Timestamp prefix for logs.
     *
     * @return A std::string of length 15, containing the current timestamp in the format "[mm:ss:SSS]:   "
     */
    std::string prefix(uint32_t timestamp = -67);

    static std::queue<std::pair<std::pair<uint8_t, uint8_t>, std::string>> ctrl_screen_queue;

    static void ctrl_screen_update_task_function(void* context) {
        if (ctrl_screen_queue.empty()) return;

        const auto [row_col, content] = ctrl_screen_queue.front();
        ctrl_screen_queue.pop();

        master.set_text(row_col.first, row_col.second, content.c_str());

        if (ctrl_screen_queue.size() >= 8) {
            const auto logger = static_cast<logger::Logger*>(context);
            logger->log({
                logger::LogLevel::Warn,
                std::format("Ctrl screen queue overflow ({} entries)", ctrl_screen_queue.size())
            });
        }
    }

} // namespace chisel
