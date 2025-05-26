#pragma once

#include <cstdint>
#include <algorithm>

namespace chisel {

    struct Task {
        static constexpr uint16_t MAX_INTERVAL = 0x7FFF;

        uint32_t execute_time;
        uint8_t priority;
        void(*function)(void* context);
        void* context;

        uint16_t interval : 15;
        uint16_t recurring : 1;

        Task(uint32_t execute_time, uint8_t priority,
             void (*func)(void*), void* ctx = nullptr, uint16_t itv = 0)
             : execute_time(execute_time), priority(priority), function(func), context(ctx) {
            recurring = itv > 0;

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