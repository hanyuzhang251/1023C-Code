#pragma once
#include "movement.h"
#include "../../config.h"

namespace chisel
{
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
