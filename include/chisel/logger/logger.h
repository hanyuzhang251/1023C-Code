#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "chisel/data/circular_buffer.h"

#include "log_entry.h"
#include "sink.h"

#include "main.h"

namespace chisel::logger {
    class Logger {
        CircularBuffer<LogEntry> backtrace;
        std::vector<Sink *> targets{};

    public:
        Logger(uint16_t size, std::initializer_list<Sink*> sinks);

        void log(LogEntry log_entry);

        void dump_backtrace() const;
    };
}
