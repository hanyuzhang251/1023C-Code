#pragma once

#include "../include/main.h"

#include "../include/chisel/chassis/chassis.h"

/*****************************************************************************/
/*                                Device Ports                               */
/*****************************************************************************/

constexpr int8_t dt_fl_motor_port = 1;
constexpr int8_t dt_ml_motor_port = 2;
constexpr int8_t dt_bl_motor_port = 3;

constexpr int8_t dt_fr_motor_port = -4;
constexpr int8_t dt_mr_motor_port = -5;
constexpr int8_t dt_br_motor_port = -6;

constexpr int8_t linear_odom_port = 7;

constexpr int8_t imu_port = 8;

/*****************************************************************************/
/*                                  Devices                                  */
/*****************************************************************************/

inline pros::MotorGroup dt_left_motors = {dt_fl_motor_port, dt_ml_motor_port, dt_bl_motor_port};
inline pros::MotorGroup dt_right_motors = {dt_fr_motor_port, dt_mr_motor_port, dt_br_motor_port};

inline pros::Rotation linear_odom(linear_odom_port);
constexpr float linear_odom_offset = 0;

inline pros::IMU imu(imu_port);

/*****************************************************************************/
/*                                  Systems                                  */
/*****************************************************************************/

inline auto logger = chisel::logger::Logger(64);

constexpr double wheel_size = 3.25;
constexpr double track_width = 15.5;
constexpr double gear_ratio = 3.0 / 4;

inline chisel::PIDController lateral_pid = {
    1, // kp
    2, // ki
    3, // kp
    4, // tolerance
    5, // wind
    6, // clamp
    7, // decay
    8, // small error
    9 // large error
};

inline chisel::PIDController angular_pid = {
    1, // kp
    2, // ki
    3, // kp
    4, // tolerance
    5, // wind
    6, // clamp
    7, // decay
    8, // small error
    9 // large error
};

// do NOT use this drivetrain object directly, use the chassis object instead
inline chisel::DriveTrain drivetrain(
    &dt_left_motors, &dt_right_motors,
    wheel_size, track_width, gear_ratio,
    &lateral_pid, &angular_pid);

// do NOT use this odom object directly, use the chassis object instead
inline chisel::Odom odom(
    &logger, &drivetrain, &imu,
    &linear_odom, nullptr, nullptr,
    linear_odom_offset, 0, 0,
    {0, 0, 0});

/**
 * @brief Global Chassis object. Represents the robot chassis.
 *
 * Contains the scheduler, drivetrain, and odometry. Logger is provided as a
 * pointer. As drivetrain and odometry are moved into the chassis, the global
 * instance of these objects should no longer be used.
 *
 * @note Clang-Tidy may suggest that \code std::move()\endcode is not needed when passing
 *       the drivetrain and odom to the chassis, but this is incorrect.
 */
inline chisel::Chassis chassis(&logger, std::move(drivetrain), std::move(odom));

/*****************************************************************************/
/*                                    Init                                   */
/*****************************************************************************/

void device_init()
{

}