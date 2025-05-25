#pragma once

#include "log_entry.h"

namespace chisel::logger {
    class Sink {
    public:
        virtual void send_log(const LogEntry &log) = 0;
    };

}