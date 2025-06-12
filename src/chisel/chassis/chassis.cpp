#include "chassis.h"

#include "chisel/config.h"

namespace chisel {

    void Chassis::register_device(DeviceMetadata &&device) {
        devices.push_back(device);
    }

    Chassis::Chassis(logger::Logger* logger, DriveTrain &&drivetrain, Odom &&odom):
        logger(logger), scheduler(logger, 512), drivetrain(drivetrain), odom(odom) {


        // auto reg = [&](auto& motors, const char* prefix) {
        //     for (int i = 0; i < motors.size(); ++i)
        //         register_device({std::format("{}_{}", prefix, i + 1).c_str(), motors[i]});
        // };
        // reg(drivetrain.left_motor_group, "dt_left");
        // reg(drivetrain.right_motor_group, "dt_right");

        // register device monitor task.
        static auto logger_device_context_pair = std::make_pair(&logger, &devices);
        scheduler.add_task(Task{
                pros::millis() + 3000, Task::PRIORITY_HIGH - 1,
                monitor::device_monitor_task_function, &logger_device_context_pair, 1000
        });

        // static auto movement_task_context_struct = /*custom context struct here*/;
        // scheduler.add_task(Task{
        //     pros::millis() + 1000, Task::PRIORITY_HIGH,
        //     movement_task_function, &movement_task_context_struct, PROCESS_DELAY
        // });

        pros::Task([&] {
            while (true) {
                scheduler.update();

                pros::delay(PROCESS_DELAY);
            }
        });
    }
}
