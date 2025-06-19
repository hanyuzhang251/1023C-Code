#pragma once

#include "../include/main.h"

#include "../include/chisel/chassis/chassis.h"

/*****************************************************************************/
/*                                Device Ports                               */
/*****************************************************************************/

constexpr int8_t dt_fl_motor_port = 0;
constexpr int8_t dt_ml_motor_port = 0;
constexpr int8_t dt_bl_motor_port = 0;

constexpr int8_t dt_fr_motor_port = 0;
constexpr int8_t dt_mr_motor_port = 0;
constexpr int8_t dt_br_motor_port = 0;

constexpr int8_t linear_odom_port = 0;

constexpr int8_t imu_port = 0;

constexpr int8_t intake_left_roller_port = 0;
constexpr int8_t intake_right_roller_port = 0;
constexpr int8_t intake_stage1_port = 18;
constexpr int8_t intake_stage2_port = 19;

constexpr int8_t intake_indexer_port = -17;
constexpr int8_t intake_hood_port = 0;

/*****************************************************************************/
/*                                  Controls                                 */
/*****************************************************************************/

constexpr pros::digi_button intake_fwd_button = pros::CTRL_DIGI_R1;
constexpr pros::digi_button intake_rev_button = pros::CTRL_DIGI_R2;

// 0 -> store
// 1 -> score high
// 2 -> score mid
inline uint8_t intake_target = 0;
constexpr int32_t intake_stage1_mt[] = {1, 1, 1};
constexpr int32_t intake_stage2_mt[] = {1, -1, -1};
constexpr int32_t intake_indexer_mt[] = {1, 1, -1};

constexpr pros::digi_button intake_store_button = pros::CTRL_DIGI_LEFT;
constexpr pros::digi_button intake_high_button = pros::CTRL_DIGI_UP;
constexpr pros::digi_button intake_mid_button = pros::CTRL_DIGI_RIGHT;

constexpr int32_t INTAKE_SPEED = 127;

/*****************************************************************************/
/*                                  Devices                                  */
/*****************************************************************************/

// controller is defined in config.h

inline pros::MotorGroup dt_left_motors = {dt_fl_motor_port, dt_ml_motor_port, dt_bl_motor_port};
inline pros::MotorGroup dt_right_motors = {dt_fr_motor_port, dt_mr_motor_port, dt_br_motor_port};

inline pros::Rotation linear_odom(linear_odom_port);
constexpr float linear_odom_offset = 0;

inline pros::IMU imu(imu_port);

inline pros::MotorGroup intake_stage1 {
    intake_left_roller_port, intake_right_roller_port, intake_stage1_port
};
inline pros::Motor intake_stage2 (intake_stage2_port);
inline pros::Motor intake_indexer(intake_indexer_port);
inline pros::adi::AnalogIn intake_hood(intake_hood_port);

/*****************************************************************************/
/*                                  Systems                                  */
/*****************************************************************************/

inline auto logger = chisel::logger::Logger(64);

class BrainScreenSink final : public chisel::logger::Sink {
    const uint8_t REFRESH_RATE = 17;
    const uint8_t MAX_BACKLOG = 6;

    const uint16_t ROW_COUNT = 12;
    const float ROW_HEIGHT = 239 / ROW_COUNT;

    chisel::logger::Logger* parent = nullptr;

    uint8_t backlog = 0;
    uint32_t last_flush = 0;

    void flush() const {
        pros::screen::set_pen(pros::Color::black);
        pros::screen::erase_rect(0, 0, 480, 240);

        const auto logs = parent->backtrace.contents(ROW_COUNT);

        for (int i = 0; i < logs.size(); ++i) {
            const auto& log = logs[i];

            pros::screen::set_pen(pros::Color::white);

            if (log.log_level == chisel::logger::LogLevel::Critical) {
                pros::screen::set_pen(pros::Color::red);

                const uint16_t y = 239 - (i + 1) * ROW_HEIGHT;
                pros::screen::fill_rect(0, y, 480, y + ROW_HEIGHT);

                pros::screen::set_pen(pros::Color::white);
            }
            else pros::screen::set_pen(chisel::logger::log_level_to_color(log.log_level));

            pros::screen::print(TEXT_MEDIUM, ROW_COUNT - i,
                                std::format("{}{}", chisel::prefix(log.timestamp), log.log).c_str());
        }
    }

public:
    void set_parent(chisel::logger::Logger* logger) override {
        parent = logger;
    }

    void send_log(const chisel::logger::LogEntry& log) override {
        ++backlog;

        flush();

        // if (backlog >= MAX_BACKLOG || pros::millis() - last_flush >= REFRESH_RATE) flush();
    }
};

inline BrainScreenSink brain_sink;

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

inline void device_init() {
    // logger.add_sink(&brain_sink);

    chisel::logger::FunctionSink console_sink ([](const chisel::logger::LogEntry& log) {
        std::printf("%s %s", chisel::prefix(log.timestamp).c_str(), log.log.c_str());
    });

    (void)intake_stage1.set_brake_mode_all(pros::MotorBrake::coast);
    (void)intake_stage2.set_brake_mode_all(pros::MotorBrake::coast);
    (void)intake_indexer.set_brake_mode_all(pros::MotorBrake::coast);
}
