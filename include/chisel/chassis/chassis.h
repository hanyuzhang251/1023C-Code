#pragma once

#include "../../main.h"

#include "chisel/logger/logger.h"
#include "scheduler/scheduler.h"
#include "chisel/data/device.h"

namespace chisel {

    class Chassis {
        static void device_monitor_task_function(void* context);

    public:
        auto logger = logger::Logger(64);
        Scheduler scheduler;

        std::vector<DeviceMetadata> devices{};

        void register_device(DeviceMetadata &&device);

        Chassis();
    };

}