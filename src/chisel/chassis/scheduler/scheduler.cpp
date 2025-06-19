#include "chisel/chassis/scheduler/scheduler.h"

#include "chisel/config.h"

namespace chisel {
    Scheduler::Scheduler(logger::Logger *logger, const uint16_t max_tasks) : logger(logger), MAX_TASKS(max_tasks) {
        std::vector<Task> container;
        container.reserve(MAX_TASKS);

        tasks = std::priority_queue(std::less<>(), std::move(container));

        logger->log({logger::LogLevel::Debug, std::format("New scheduler constructed. max_tasks=%d", MAX_TASKS)});
    }

    bool Scheduler::add_task(const Task &task) {
        if (task_count >= MAX_TASKS) {
            logger->log({logger::LogLevel::Error, std::format("Scheduler full (%d/%d tasks)!", task_count, MAX_TASKS)});
            return false;
        }

        tasks.push(task);
        ++task_count;

        return true;
    }

    void Scheduler::update() {
        uint32_t update_start_time = pros::millis();

        if (tasks.empty()) {
            next_update = update_start_time + PROCESS_DELAY;
            return;
        }

        std::vector<Task> tasks_to_run;
        tasks_to_run.reserve(8);

        uint16_t n_tasks = 0;
        while (tasks.top().execute_time <= update_start_time) {
            tasks_to_run.push_back(tasks.top());
            tasks.pop();
            --task_count;
            ++n_tasks;
        }

        if (n_tasks > 8) {
            logger->log({logger::LogLevel::Warn, "Scheduler is running %d tasks this update.", n_tasks});
        }

        std::ranges::sort(tasks_to_run, [update_start_time](const Task &a, const Task &b) {
            const auto scale_priority = [update_start_time](const Task &task) {
                const uint32_t priority_band = task.priority << (task.priority >= Task::PRIORITY_HIGH ? 2 : 1);
                const uint32_t overdue_bonus = (update_start_time - task.execute_time) / PROCESS_DELAY;

                return priority_band + overdue_bonus;
            };

            return scale_priority(a) > scale_priority(b);
        });

        for (auto &current: tasks_to_run) {
            current.function(current.context);

            if (current.recurring) {
                current.execute_time += current.interval;

                add_task(current);
            }
        }

        const Task* next_task = &tasks.top();
        next_update = std::min(next_update, next_task->execute_time);
    }
}
