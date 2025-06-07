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
constexpr double gear_ratio = 3 / 4;

// do NOT use this drivetrain object directly, use the chassis object instead
inline chisel::DriveTrain drivetrain(
        &dt_left_motors, &dt_right_motors,
        wheel_size, track_width, gear_ratio);

// do NOT use this odom object directly, use the chassis object instead
inline chisel::Odom odom(
        &logger, &drivetrain, &imu,
        &linear_odom, nullptr, nullptr,
        linear_odom_offset, 0, 0,
        {0, 0, 0});

// Note: The chassis object moves the drivetrain and odom objects into its constructor.
//      Because of this, the drivetrain and odom objects should only be used in the chassis object.
inline chisel::Chassis chassis(&logger, std::move(drivetrain), std::move(odom));