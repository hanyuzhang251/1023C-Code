#pragma once

#include <cstdint>
#include <string>

namespace chisel::logger {
        enum class LogLevel {
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    struct LogEntry {
        LogLevel log_level;
        std::string log;
        uint32_t timestamp;

        LogEntry(LogLevel log_level, std::string log, uint32_t timestamp);

        LogEntry() = default;
    };

}