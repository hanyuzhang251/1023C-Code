#pragma once

#include "drivetrain.h"
#include "../../main.h"

#include "../../chisel/data/pose.h"
#include "../../chisel/logger/logger.h"

namespace chisel {
    /**
     * @brief Represents the Odometry system for tracking the robot's position and orientation.
     */
    struct Odom {
        logger::Logger *logger;

        // if this flag is set to true, the auto will abort.
        bool crashout = false;

        DriveTrain *drivetrain = nullptr;
        double dtl_reset = 0, dtr_reset = 0;

        pros::Imu *imu = nullptr;

        pros::Rotation *ltw = nullptr;
        pros::Rotation *rtw = nullptr;
        pros::Rotation *stw = nullptr;
        double ltw_pp = 0, rtw_pp = 0, stw_pp = 0;
        double ltw_reset = 0, rtw_reset = 0, stw_reset = 0;

        double sl, sr, ss;

        Pose i_pose{0, 0, 0};
        Pose pi_pose{0, 0, 0};

        Pose pose_offset{0, 0, 0};
        Pose pose{0, 0, 0};

        /**
         * Odom constructor
         *
         * @param logger pointer to the logger
         * @param drivetrain pointer to the drivetrain
         * @param imu pointer to the imu
         * @param ltw pointer to the left tracking wheel
         * @param rtw pointer to the right tracking wheel
         * @param stw pointer to the back tracking wheel
         * @param sl horizontal offset of left tracking wheel to tracking center
         * @param sr horizontal offset of right tracking wheel to tracking center
         * @param ss vertical offset of back tracking wheel to tracking center
         * @param pose_offset offset applied to the internal pose
         */
        Odom(logger::Logger *logger, DriveTrain *drivetrain, pros::Imu *imu, pros::Rotation *ltw, pros::Rotation *rtw,
             pros::Rotation *stw,
             double sl, double sr, double ss, Pose &&pose_offset);

        void reset();

        void setPose(const Pose &pose);

        /**
         * @breif Gets the heading of the robot in radians, adapting approach depending on which sensors are provided.
         *
         * First attempts to calculate heading from the left and right vertical tracking wheels.
         * If the tracking wheels aren't provided, uses the IMU instead.
         * If the IMU isn't provided, uses motor encoders from the drive train instead.
         * If the drive train isn't provided, logs a critical error, aborts the auton, and returns 0.
         *
         * @return The heading of the robot in radians.
         */
        [[nodiscard]] double obtain_heading();

        /**
         * @brief Gets the local x and y coordinates of the robot, given the change of heading in radians.
         *
         * Uses either one of the left and right tracking wheels and the back tracking wheel to calculate the local x and y coordinates.
         * If the back tracking wheel isn't provided, local x will not be calculated.
         * If neither the left and right tracking wheels are provided, uses motor encoders from the drive train instead.
         *
         * @param delta_theta change of heading in radians.
         * @return a pair of the local x and y change.
         */
        [[nodiscard]] std::pair<double, double> obtain_local_xy(double delta_theta);

        void track();
    };
}
