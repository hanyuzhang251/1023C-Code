#pragma once

#include "movement.h"

namespace chisel {
    class TurnToHeading final : public Motion {
    public:
        double target_heading;

        struct TurnToHeadingParams : MotionParams {
            double lateral_exit_range = -67;
        };

        TurnToHeading(Pose *pose, double target_heading, const TurnToHeadingParams &params);

        void update() override;

    private:
        float angular_pid_control = 0;
    };

    class TurnToPoint final : public Motion {
        Pose target_point;

        struct TurnToPointParams : MotionParams {
            double lateral_exit_range = -67;
        };

        TurnToPoint(Pose *pose, const Pose &target_point, const TurnToPointParams &params);

        void update() override;
    };

    class QuickMoveToPoint final : public Motion {
        Pose target_point;

        struct QuickMoveToPointParams : MotionParams {};

        QuickMoveToPoint(Pose *pose, const Pose &target_point, const QuickMoveToPointParams &params);

        void update() override;
    };
}
