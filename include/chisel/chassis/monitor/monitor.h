#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <format>

#include "../../../main.h"
#include "../../logger/logger.h"
#include "../../data/device.h"

namespace chisel::monitor {

    static void device_monitor_task_function(void* context);

}