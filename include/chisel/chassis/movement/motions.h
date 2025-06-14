#pragma once

#include "movement.h"

namespace chisel {

    /**
     * Motion for turning to a heading
     */
    class TurnToHeading final : public Motion {
    public:
        double target_heading;

        /**
         * Parameters for TurnToHeading motion.
         * Lateral exit range is set to -67, as lateral exit isn't used for this motion.
         */
        struct TurnToHeadingParams : MotionParams {
            double lateral_exit_range = -67;
        };

        TurnToHeading(Pose *pose, double target_heading, const TurnToHeadingParams &params);

        void update() override;
    };

    /**
     * Motion for turning to face a point
     */
    class TurnToPoint final : public Motion {
        Pose target_point;

        /**
         * Parameters for TurnToPoint motion.
         * Lateral exit range is set to -67, as lateral exit isn't used for this motion.
         */
        struct TurnToPointParams : MotionParams {
            double lateral_exit_range = -67;
        };

        TurnToPoint(Pose *pose, const Pose &target_point, const TurnToPointParams &params);

        void update() override;
    };

    /**
     * Motion for quickly moving to a point.
     * Quick as in it just b-lines for the point, no fancy curves.
     */
    class QuickMoveToPoint final : public Motion {
        Pose target_point;

        /**
         * Parameters for QuickMoveToPoint motion.
         * Same as default motion parameters.
         */
        struct QuickMoveToPointParams : MotionParams {};

        QuickMoveToPoint(Pose *pose, const Pose &target_point, const QuickMoveToPointParams &params);

        void update() override;
    };
}
