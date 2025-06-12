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

    static void mtf_load_current_motion(MovementTaskContext* context)
    {
        context->current_motion = &context->motion_queue->front();

        context->current_motion->start_pose = context->odom->pose;
        context->current_motion->start_time = pros::millis();
    }

    static void mtf_stop_current_motion(const MovementTaskContext* context)
    {
        delete context->current_motion;
        context->motion_queue->pop();
    }

    static bool mtf_check_motion_exit(const MovementTaskContext* context)
    {
        if (context->current_motion->lateral_exit.get_exit() && context->current_motion->angular_exit.get_exit())
        {
            if (verbose)
                context->logger->log({
                    logger::LogLevel::Debug,
                    "Motion exited properly"
                });
            return true;
        }
        if (context->current_motion->crashout)
        {
            context->logger->log({
                logger::LogLevel::Warn,
                "Motion triggered crashout"
            });
            return true;
        }
        if (pros::millis() - context->current_motion->start_time >= context->current_motion->params.timeout)
        {
            context->logger->log({
                logger::LogLevel::Warn,
                "Motion timed out before exiting properly"
            });
            return true;
        }

        return false;
    }

    static void movement_task_function(void* context)
    {
        const auto task_context = static_cast<MovementTaskContext*>(context);
        const auto motion_queue = task_context->motion_queue;
        const auto current_motion = task_context->current_motion;

        if (motion_queue->empty()) return;

        if (!current_motion) mtf_load_current_motion(task_context);

        // Check if current motion is still null after loading. This shouldn't
        // happen, but one can rarely be too cautious.
        if (!current_motion) return;

        if (mtf_check_motion_exit(task_context))
        {
            mtf_stop_current_motion(task_context);
            return;
        }

        task_context->current_motion->update();
    }
}
