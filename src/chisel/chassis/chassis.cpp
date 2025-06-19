#include "../include/chisel/chassis/chassis.h"

#include "chisel/config.h"

namespace chisel {
    void Chassis::register_device(DeviceMetadata&& device) {
        devices.push_back(device);
    }

    static void print_pose(void* context) {
        const auto odom_ = static_cast<Odom*>(context);
        const auto& pose = odom_->pose;
        ctrl_screen_queue.push({{0, 0}, std::format("x={}", pose.x)});
        ctrl_screen_queue.push({{1, 0}, std::format("y={}", pose.y)});
        ctrl_screen_queue.push({{2, 0}, std::format("h={}", pose.h)});
    }

    Chassis::Chassis(logger::Logger* logger, DriveTrain&& drivetrain, Odom&& odom):
        logger(logger), scheduler(logger, 512), drivetrain(drivetrain), odom(odom) {

        scheduler.add_task(Task{
            pros::millis(), Task::PRIORITY_HIGH - 1,
            ctrl_screen_update_task_function, &logger, 50
        });

        // ensure the scheduler is working
        scheduler.add_task(Task{
            pros::millis(), Task::PRIORITY_CRITICAL,
            print_pose,
            nullptr, 150
        });

        scheduler.add_task(Task{
            pros::millis() + 500, Task::PRIORITY_CRITICAL,
            [](void* context) {
                const auto odom_ = static_cast<Odom*>(context);
                odom_->track();
            },
            nullptr, PROCESS_DELAY
        });

        // register device monitor task.
        static auto logger_device_context_pair = std::make_pair(&logger, &devices);
        scheduler.add_task(Task{
            pros::millis() + 3000, Task::PRIORITY_HIGH - 1,
            monitor::device_monitor_task_function, &logger_device_context_pair, 1000
        });

        static auto movement_task_context_struct = MovementTaskContext{logger, &motion_queue, current_motion, &odom};
        scheduler.add_task(Task{
            pros::millis() + 1000, Task::PRIORITY_HIGH,
            movement_task_function, &movement_task_context_struct, PROCESS_DELAY
        });

        pros::Task([&] {
            while (state != State::End) {
                scheduler.update();

                pros::delay(scheduler.next_update - pros::millis());
            }
        });
    }
}
