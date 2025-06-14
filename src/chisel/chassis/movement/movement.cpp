#include "../../../../include/chisel/chassis/movement/movement.h"
#include "chisel/config.h"

namespace chisel
{
    ExitCondition::ExitCondition(const double range, const uint32_t time) :
        range(range), time(time)
    {
        reset();
    }

    bool ExitCondition::get_exit() const
    {
        return exit;
    }

    void ExitCondition::update(const double error)
    {
        if (range == -67)
        {
            exit = true;
            return;
        }

        if (error > range)
            start_time = pros::millis();

        if (pros::millis() - start_time >= time)
            exit = true;
    }

    void ExitCondition::reset()
    {
        start_time = UINT32_MAX;
        exit = false;

        if (range == -67)
        {
            exit = true;
        }
    }

    Motion::Motion(Pose* pose, const MotionParams& params) :
        curr_pose(pose),
        params(params),
        lateral_exit(params.lateral_exit_range, params.lateral_exit_time),
        angular_exit(params.angular_exit_range, params.angular_exit_time)
    {
        lateral_exit.reset();
        angular_exit.reset();
    }

    MovementTaskContext::MovementTaskContext(logger::Logger* logger, std::queue<Motion>* motion_queue,
                                             Motion* current_motion, Odom* odom)
        : logger(logger),
          motion_queue(motion_queue),
          current_motion(current_motion),
          odom(odom)
    {
    }
}
