#include "chassis.h"

namespace chisel {
    void Chassis::device_monitor_task_function(void *context) {
        static bool first_call = true;
        const bool is_first_call = first_call;
        first_call = false;

        auto context_pair = static_cast<std::pair<logger::Logger *, std::vector<DeviceMetadata> *> *>(context);
        auto logger = static_cast<logger::Logger *>(context_pair->first);
        auto devices = static_cast<std::vector<DeviceMetadata> *>(context_pair->second);

        for (auto &device: *devices) {
            const char *name = device.name();
            const uint8_t port = device.device->get_port();

            switch (pros::DeviceType device_type = device.device->get_plugged_type()) {
                case pros::DeviceType::none: {
                    device.status = DeviceMetadata::STATUS_NONE;
                    break;
                }
                case pros::DeviceType::motor: {
                    const auto motor_device = static_cast<pros::Motor *>(device.device);

                    double motor_temp = motor_device->get_temperature();

                    if (motor_device->get_position() == PROS_ERR_F) {
                        device.status = DeviceMetadata::STATUS_ERROR;
                        logger->log({
                            logger::LogLevel::Critical,
                            std::format("Motor %s at port %d is malfunctioning", name, port)
                        });
                    } else if (motor_temp >= 59) {
                        device.status = DeviceMetadata::STATUS_BAKED;
                        logger->log({
                            logger::LogLevel::Critical,
                            std::format("Motor %s at port %d is BAKED (%lf C)", name, port, motor_temp)
                        });
                    } else if (motor_temp >= 54) {
                        device.status = DeviceMetadata::STATUS_BURNT;
                        logger->log({
                            logger::LogLevel::Warn,
                            std::format("Motor %s at port %d is BURNT (%lf C)", name, port, motor_temp)
                        });
                    } else if (motor_temp >= 44) {
                        device.status = DeviceMetadata::STATUS_HOT;
                        logger->log({
                            logger::LogLevel::Info,
                            std::format("Motor %s at port %d is HOT (%d C)", name, port, motor_temp)
                        });
                    } else {
                        device.status = DeviceMetadata::STATUS_OK;
                    }

                    break;
                }
                case pros::DeviceType::rotation: {
                    const auto rotation_device = static_cast<pros::Rotation *>(device.device);

                    if (rotation_device->get_position() == PROS_ERR) {
                        device.status = DeviceMetadata::STATUS_ERROR;
                        logger->log({
                            logger::LogLevel::Critical,
                            std::format("Rotation sensor %s at port %d is malfunctioning", name, port)
                        });
                    } else {
                        device.status = DeviceMetadata::STATUS_OK;
                    }

                    break;
                }
                case pros::DeviceType::imu: {
                    const auto imu_device = static_cast<pros::Imu *>(device.device);

                    
                }
                default: {
                    device.status = DeviceMetadata::STATUS_NA;
                    logger->log({
                        (is_first_call ? logger::LogLevel::Warn : logger::LogLevel::Debug),
                        std::format("Device %s at port %d is of unknown type", device.name, device.device->get_port())
                    });
                    break;
                };
            }
        }

        first_call = false;
    }

    void Chassis::register_device(DeviceMetadata &&device) {
        devices.push_back(std::move(device));
    }

    Chassis::Chassis(): scheduler(512) {
        scheduler.add_task({
            pros::millis() + 3000, Task::PRIORITY_HIGH - 1,
            device_monitor_task_function, std::pair{&logger, &devices}, 1000
        });
    }
}
