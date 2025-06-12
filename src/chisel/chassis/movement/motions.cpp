#include "../../../../include/chisel/chassis/movement/motions.h"

namespace chisel {

    TurnToHeading::TurnToHeading(Pose *pose, const double target_heading, const TurnToHeadingParams &params):
        Motion(pose, params), target_heading(target_heading) {
    }

    void TurnToHeading::update() {
        const double error = deg_err(target_heading, curr_pose->h) * (params.revdir ? -1 : 1);

        angular_exit.update(error);

        controls.first = DBL_MAX;
        controls.second = error;
    }

    TurnToPoint::TurnToPoint(Pose *pose, const Pose &target_point, const TurnToPointParams &params):
        Motion(pose, params), target_point(target_point) {
    }

    void TurnToPoint::update() {
        const Pose rel_target = Pose::sub(target_point, *curr_pose);

        const double error = deg_err(deg_to_point(rel_target), curr_pose->h) * (params.revdir ? -1 : 1);

        angular_exit.update(error);

        controls.first = DBL_MAX;
        controls.second = error;
    }

    QuickMoveToPoint::QuickMoveToPoint(Pose *pose, const Pose &target_point, const QuickMoveToPointParams &params):
        Motion(pose, params), target_point(target_point) {
    }

    void QuickMoveToPoint::update() {
        const Pose rel_target = Pose::sub(target_point, *curr_pose);

        if (const double side = Pose::dot(Pose::sub(target_point, start_pose), Pose::sub(target_point, *curr_pose));
            side <= 0)
            crashout = true;

        double lat_error = dist_to_point(rel_target);
        const double ang_error = deg_err(deg_to_point(rel_target), curr_pose->h) * (params.revdir ? -1 : 1);

        lateral_exit.update(lat_error);
        angular_exit.update(ang_error);

        const double lat_mod = 1 - abs(ang_error) / 90;
        lat_error *= lat_mod;

        controls.first = lat_error * (params.revdir ? -1 : 1);
        controls.second = ang_error;
    }
}
