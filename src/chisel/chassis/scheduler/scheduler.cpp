#include "scheduler.h"

namespace chisel {

    Scheduler::Scheduler(logger::Logger* logger, const uint16_t max_tasks) : logger(logger), MAX_TASKS(max_tasks) {
        std::vector<Task> container;
        container.reserve(MAX_TASKS);

        tasks = std::priority_queue(std::less<Task>(), std::move(container));
    }

    bool Scheduler::add_task(Task task) {
        if (task_count >= MAX_TASKS) {
            logger->log({logger::LogLevel::Error, std::format("Scheduler full (%d/%d tasks)!", task_count, MAX_TASKS)});
            return false;
        }

        tasks.push(std::move(task));
        ++task_count;

        return true;
    }

    void Scheduler::update() {
        if (tasks.empty()) return;

        while (tasks.top().execute_time <= pros::millis()) {
            Task current = tasks.top();
            tasks.pop();
            --task_count;

            current.function(current.context);

            if (current.recurring) {
                current.execute_time += current.interval;
                add_task(std::move(current));
            }
        }
    }
}
