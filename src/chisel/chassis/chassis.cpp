#include "../include/chisel/chassis/chassis.h"

#include "chisel/config.h"

namespace chisel {

    void Chassis::register_device(DeviceMetadata &&device) {
        devices.push_back(device);
    }

    Chassis::Chassis(logger::Logger* logger, DriveTrain &&drivetrain, Odom &&odom):
        logger(logger), scheduler(logger, 512), drivetrain(drivetrain), odom(odom) {

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
            while (state != State::SHUTDOWN) {
                scheduler.update();

                pros::delay(scheduler.next_update - pros::millis());
            }
        });
    }
}
