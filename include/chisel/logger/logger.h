#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "chisel/data/circular_buffer.h"

#include "log_entry.h"
#include "sink.h"

#include "main.h"

namespace chisel::logger {

    /**
     * @class Logger
     * @brief A class to handle logging across multiple sinks with backtrace capabilities.
     *
     * The Logger class provides mechanisms for logging messages, managing multiple
     * logging sinks, and maintaining a backtrace of log entries for later inspection.
     */
    class Logger {
        static constexpr uint8_t MAX_SINKS = 16;
        CircularBuffer<LogEntry> backtrace;
        Sink *targets[MAX_SINKS]{};
        uint8_t sink_count = 0;

    public:
        /**
         * Logger constructor
         *
         * @param backtrace_size Number of log entries to store in the backtrace. Recommended to be less than 300.
         */
        explicit Logger(uint16_t backtrace_size);

        bool add_sink(Sink *sink);

        void log(LogEntry &&log_entry);

        void dump_backtrace() const;
    };
}
