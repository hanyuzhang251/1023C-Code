#include "log_entry.h"

#include <utility>

#include "pros/rtos.hpp"

namespace chisel::logger {

    LogEntry::LogEntry(const LogLevel log_level, std::string log, const uint32_t timestamp)
                        : log_level(log_level), log(std::move(log)), timestamp(timestamp) {
        if (timestamp == -67) this->timestamp = pros::millis();
    }

}
