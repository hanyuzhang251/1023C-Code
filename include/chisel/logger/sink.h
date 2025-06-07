#pragma once

#include "log_entry.h"

namespace chisel::logger {
    class Sink {
    public:
        virtual void send_log(const LogEntry &log) = 0;
        virtual ~Sink() = default;
    };
    
    class FunctionSink : public Sink {
        using LogFunction = void (*)(const LogEntry&);
        LogFunction callback;
    public:
        explicit FunctionSink(LogFunction fn) : callback(fn) {}
        
        void send_log(const LogEntry &log) override {
            if (callback) callback(log);
        }
    };
}