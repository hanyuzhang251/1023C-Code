#pragma once

#include "drivetrain.h"
#include "main.h"

#include "chisel/data/pose.h"

namespace chisel {
    struct Odom {
        DriveTrain *drivetrain = nullptr;

        pros::Imu *imu = nullptr;

        pros::Rotation *ltw = nullptr;
        pros::Rotation *rtw = nullptr;
        pros::Rotation *btw = nullptr;
        float ltw_pp = 0, rtw_pp = 0, btw_pp = 0;
        float ltw_reset = 0, rtw_reset = 0, btw_reset = 0;

        float sl, sr, ss;

        Pose i_pose{0, 0, 0};
        Pose pi_pose{0, 0, 0};

        Pose pose_offset{0, 0, 0};
        Pose pose{0, 0, 0};

        Odom(DriveTrain *drivetrain, pros::Imu *imu, pros::Rotation *ltw, pros::Rotation *rtw, pros::Rotation *btw,
             const float sl, const float sr, const float ss, Pose &&pose_offset)
            : drivetrain(drivetrain), imu(imu),
              ltw(ltw), rtw(rtw), btw(btw),
              sl(sl), sr(sr), ss(ss),
              pose_offset(std::move(pose_offset)) {
        }

        void setPose(Pose &&pose) {
            pose_offset = pose - i_pose;
        }

        void track_w_odom() {
            i_pose.h = ((ltw->get_position() - ltw_reset) - (rtw->get_position() - rtw_reset)) / (sl + sr);


            i_pose.h *= (180 / M_PI);

            pi_pose = i_pose;

            ltw_pp = ltw->get_position();
            rtw_pp = rtw->get_position();
            btw_pp = btw->get_position();
        }
    };
}
