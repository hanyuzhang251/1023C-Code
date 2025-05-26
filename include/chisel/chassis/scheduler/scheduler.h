#pragma once

#include <queue>

#include "api.h"
#include "main.h"

#include "task.h"

namespace chisel {

    class Scheduler {
        const uint16_t MAX_TASKS;
        std::priority_queue<Task> tasks{};
        uint16_t task_count = 0;

    public:
        explicit Scheduler(uint16_t max_tasks);

        bool add_task(Task task);

        void update();
    };

}