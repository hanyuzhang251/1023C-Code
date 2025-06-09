#include "../../../include/chisel/chassis/odom.h"

namespace chisel {

    Odom::Odom(logger::Logger *logger, DriveTrain *drivetrain, pros::Imu *imu, 
               pros::Rotation *ltw, pros::Rotation *rtw, pros::Rotation *stw,
               const double sl, const double sr, const double ss, Pose &&pose_offset)
        : logger(logger), drivetrain(drivetrain), imu(imu),
          ltw(ltw), rtw(rtw), stw(stw),
          sl(sl), sr(sr), ss(ss),
          pose_offset(pose_offset) {
    }

    void Odom::reset() {
        // reset the internal pose
        i_pose = Pose{0, 0, 0};
        pi_pose = Pose{0, 0, 0};

        // resets the tracking wheel positions
        if (ltw) ltw_reset = ltw->get_position();
        if (rtw) rtw_reset = rtw->get_position();
        if (stw) stw_reset = stw->get_position();

        // resets the drivetrain motor encoder positions.
        dtl_reset = drivetrain->left_motor_group->get_position();
        dtr_reset = drivetrain->right_motor_group->get_position();
    }

    void Odom::setPose(const Pose &pose) {
        // sets the pose offset as if the robot was at the provided pose.
        pose_offset = pose - i_pose;
    }

    double Odom::obtain_heading() {
        double heading_rad = 0;

        if (ltw && rtw) {
            // if the left and right tracking wheels are provided, use them to calculate the heading.
            heading_rad = ((ltw->get_position() - ltw_reset) - (rtw->get_position() - rtw_reset)) / (sl + sr);
        } else if (imu) {
            // if the IMU is provided, use it to calculate the heading.
            heading_rad = imu->get_heading();
        } else if (drivetrain) {
            // if neither the vertical tracking wheels nor the IMU is provided, use the drivetrain motor encoders to calculate the heading instead.
            heading_rad = ((drivetrain->left_motor_group->get_position() - dtl_reset)
                          - (drivetrain->right_motor_group->get_position() - dtr_reset))
                         / (drivetrain->track_width);
        } else {
            // if nothing is provided, log a critical error and return 0.
            // if this happens, set crashout to abort the auton.
            crashout = true;
            logger->log({logger::LogLevel::Critical, "Odom cannot find the heading"});
        }

        return heading_rad;
    }

    std::pair<double, double> Odom::obtain_local_xy(const double delta_theta) {
        double local_x = 0, local_y = 0;

        if (ltw || rtw) {
            // If either the left or right tracking wheels are provided, use them to calculate the local coordinates.

            // adapt values for tracking wheel selected
            const double delta_tw = ltw ? ltw->get_position() - ltw_pp : rtw->get_position() - rtw_pp;
            const double s_tw = ltw ? sl : sr;

            // If the back tracking wheel is provided, use it to calculate local x. If not, local x will be 0;
            const double delta_S = stw ? stw->get_position() - stw_pp : 0;

            if (fabs(delta_theta) < 1e-6) {
                // If the change in heading is insignificant, calculate as if movement is straight.
                local_x = delta_S;
                local_y = delta_tw;
            } else {
                // Otherwise, calculate the local coordinates using the chord formula.
                const double chord_factor = 2 * sin(delta_theta / 2);
                local_x = chord_factor * (delta_S / delta_theta + ss);
                local_y = chord_factor * (delta_tw / delta_theta + s_tw);
            }
        } else if (drivetrain) {
            // If neither the left or right tracking wheels are provided, use the drivetrain motor encoders to calculate the local coordinates instead.

            const double delta_left = drivetrain->left_motor_group->get_position() - dtl_reset;
            const double delta_right = drivetrain->right_motor_group->get_position() - dtr_reset;

            const double left_distance = (delta_left * M_PI * drivetrain->wheel_size) / 
                                      (drivetrain->gear_ratio * 360);
            const double right_distance = (delta_right * M_PI * drivetrain->wheel_size) / 
                                       (drivetrain->gear_ratio * 360);

            if (fabs(delta_theta) < 1e-6) {
                // If moving straight
                local_x = (left_distance + right_distance) / 2;
                local_y = 0;
            } else {
                // If not

                const double radius = (drivetrain->track_width / 2) *
                                    (left_distance + right_distance) / (left_distance - right_distance);

                const double chord_factor = 2 * sin(delta_theta / 2);
                local_x = radius * chord_factor;
                local_y = 0;
            }
        } else {
            // if nothing is provided, log a critical error and return 0.
            // if this happens, set crashout to abort the auton.
            crashout = true;
            logger->log({logger::LogLevel::Critical, "Odom cannot track lateral movement"});
        }

        return {local_x, local_y};
    }

    void Odom::track() {
        const double new_heading_rad = obtain_heading();
        const double delta_theta = new_heading_rad - (pi_pose.h * M_PI / 180);

        const auto [local_x, local_y] = obtain_local_xy(new_heading_rad);

        const double avg_heading_rad = (pi_pose.h * M_PI / 180) + (delta_theta / 2);

        const double global_x = local_x * cos(avg_heading_rad) - local_y * sin(avg_heading_rad);
        const double global_y = local_x * sin(avg_heading_rad) + local_y * cos(avg_heading_rad);

        i_pose.x += global_x;
        i_pose.y += global_y;
        i_pose.h = new_heading_rad * (180 / M_PI);

        pi_pose = i_pose;
        if (ltw) ltw_pp = ltw->get_position();
        if (rtw) rtw_pp = rtw->get_position();
        if (stw) stw_pp = stw->get_position();
    }
}