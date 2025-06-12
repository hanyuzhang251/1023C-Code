#pragma once

#include "main.h"

#include "odom.h"

#include "chisel/logger/logger.h"
#include "scheduler/scheduler.h"
#include "chisel/data/device.h"
#include "drivetrain.h"
#include "movement/movement.h"
#include "../pid.h"
#include "../util/util.h"

#include "monitor/monitor.h"

namespace chisel {

    class Chassis {
    public:
        State gameState = State::INIT;

        logger::Logger* logger;
        Scheduler scheduler;

        DriveTrain drivetrain;
        Odom odom;

        std::vector<DeviceMetadata> devices{};

        std::queue<Motion> motion_queue{};
        Motion *current_motion = nullptr;

        void register_device(DeviceMetadata &&device);

        explicit Chassis(logger::Logger* logger, DriveTrain &&drivetrain, Odom &&odom);
    };
}
