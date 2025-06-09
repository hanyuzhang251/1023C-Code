#pragma once

#include <queue>

#include "api.h"
#include "main.h"

#include "chisel/logger/logger.h"
#include "task.h"

namespace chisel {

    class Scheduler {
        logger::Logger* logger;

        const uint16_t MAX_TASKS;
        std::priority_queue<Task, std::vector<Task>, std::less<>> tasks{};
        uint16_t task_count = 0;

    public:
        explicit Scheduler(logger::Logger* logger, uint16_t max_tasks);

        bool add_task(const Task &task);

        void update();
    };

}