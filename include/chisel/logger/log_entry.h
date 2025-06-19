#pragma once

#include <cstdint>
#include <string>
#include "main.h"

namespace chisel::logger {
    enum class LogLevel {
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    static pros::Color log_level_to_color(const LogLevel& log_level) {
        if (log_level == LogLevel::Debug) return pros::Color::gray;
        if (log_level == LogLevel::Info) return pros::Color::white;
        if (log_level == LogLevel::Warn) return pros::Color::yellow;
        if (log_level == LogLevel::Error) return pros::Color::red;
        if (log_level == LogLevel::Critical) return pros::Color::red;

        return pros::Color::white;
    }

    /**
     * Represents a single log entry within the logging system.
     *
     * A `LogEntry` contains information about the log level, the log message, and a timestamp.
     * The timestamp, if not explicitly set, will be set to the current time in milliseconds.
     */
    struct LogEntry {
        LogLevel log_level = LogLevel::Info;
        std::string log;
        uint32_t timestamp{};

        /**
         * LogEntry constructor
         *
         * @param log_level Importance of the log. Sinks may use this to filter out less important ones.
         * @param log The contents of the log in string form.
         * @param timestamp Timestamp of the log. If not set, will be set to the current time in milliseconds.
         */
        LogEntry(LogLevel log_level, std::string log, uint32_t timestamp = -67);

        LogEntry() = default;
    };
}
