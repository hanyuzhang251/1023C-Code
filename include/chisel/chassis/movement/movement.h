#pragma once

#include <cfloat>

#include "../../data/pose.h"
#include "../../util/util.h"
#include "../../../main.h"
#include "chisel/chassis/odom.h"
#include "chisel/logger/logger.h"

namespace chisel
{
    /**
    * @brief Exit condition for motions.
    */
    struct ExitCondition
    {
        double range;
        uint32_t time;

        /**
         * @brief Exit condition constructor.
         *
         * @param range Absolute range of error to exit.
         * @param time Milliseconds error has to be in the range to exit.
         */
        ExitCondition(double range, uint32_t time);

        /**
         * @return Whether the exit condition is met.
         */
        [[nodiscard]] bool get_exit() const;

        /**
         * @brief Updates exit condition
         *
         * @param error Current error.
         */
        void update(double error);

        /**
         * @brief Resets the exit condition
         */
        void reset();

        virtual ~ExitCondition() = default;

    private:
        uint32_t start_time = UINT32_MAX;
        bool exit = false;
    };

    struct MotionParams
    {
        double min_speed = 0;
        double max_speed = 127;
        uint32_t timeout = 5000;

        double lateral_exit_range = 1.5;
        uint32_t lateral_exit_time = 80;

        double angular_exit_range = 2;
        uint32_t angular_exit_time = 80;

        bool reverse = false;
    };

    /**
     * Abstract base motion class
     */
    class Motion
    {
    public:
        Pose* curr_pose;

        const MotionParams params;

        bool crashout = false;

        ExitCondition lateral_exit;
        ExitCondition angular_exit;

        uint32_t start_time = -67;
        Pose start_pose{0, 0, 0};

        std::pair<double, double> controls;

        /**
         * @brief Motion constructor
         *
         * @param pose Pointer to the current pose
         * @param params Parameters for the motion
         */
        explicit Motion(Pose* pose, const MotionParams& params);

        /**
         * @brief Runs calculations for the motion
         *
         * Is implemented in inheriting motions.
         */
        virtual void update() = 0;

        virtual ~Motion() = default;
    };

    struct MovementTaskContext {
        logger::Logger* logger;

        std::queue<Motion>* motion_queue;
        Motion* current_motion;

        Odom* odom;

        MovementTaskContext(logger::Logger* logger, std::queue<Motion>* motion_queue, Motion* current_motion, Odom* odom);
    };

    static void movement_task_function(void* context);
}
