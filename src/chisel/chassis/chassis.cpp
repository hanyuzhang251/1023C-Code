#include "chassis.h"

namespace chisel {

    void Chassis::register_device(DeviceMetadata &&device) {
        devices.push_back(std::move(device));
    }

    Chassis::Chassis(DriveTrain&& drivetrain): scheduler(512), drivetrain(drivetrain) {

        // auto reg = [&](auto& motors, const char* prefix) {
        //     for (int i = 0; i < motors.size(); ++i)
        //         register_device({std::format("{}_{}", prefix, i + 1).c_str(), motors[i]});
        // };
        // reg(drivetrain.left_motor_group, "dt_left");
        // reg(drivetrain.right_motor_group, "dt_right");

        static auto logger_device_context_pair = std::make_pair(&logger, &devices);

        scheduler.add_task(Task{
            pros::millis() + 3000, Task::PRIORITY_HIGH - 1,
            monitor::device_monitor_task_function, &logger_device_context_pair, 1000
        });
    }
}
