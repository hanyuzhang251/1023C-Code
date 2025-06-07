#pragma once

#include "main.h"

#include "odom.h"

#include "chisel/logger/logger.h"
#include "scheduler/scheduler.h"
#include "chisel/data/device.h"
#include "drivetrain.h"

#include "monitor/monitor.h"

namespace chisel {
    class Chassis {
    public:
        logger::Logger logger = logger::Logger(64);
        Scheduler scheduler;

        DriveTrain drivetrain;

        std::vector<DeviceMetadata> devices{};

        void register_device(DeviceMetadata &&device);

        Chassis(DriveTrain &&drivetrain);
    };
}
