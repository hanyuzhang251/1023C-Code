#pragma once

#include <cstdint>
#include <algorithm>

namespace chisel {

    struct Task {
        static constexpr uint32_t MAX_INTERVAL = 0x7FFFFFF;  // 2^27 - 1
        static constexpr uint32_t MAX_PRIORITY = 15;         // 2^4 - 1

        uint32_t execute_time;
        void(*function)(void* context);
        void* context; 

        uint32_t priority : 4;
        uint32_t interval : 27;
        uint32_t recurring : 1;

        Task(const uint32_t execute_time, const uint32_t priority,
             void (*func)(void*), void* ctx = nullptr, const uint32_t itv = 0)
             : execute_time(execute_time), function(func), context(ctx) {
            recurring = itv > 0;

            this->priority = std::min(MAX_PRIORITY, priority);
            interval = std::min(MAX_INTERVAL, itv);
        }

        bool operator<(const Task& other) const {
            if (execute_time != other.execute_time) {
                return execute_time > other.execute_time;
            }
            return priority > other.priority;
        }
    };

}