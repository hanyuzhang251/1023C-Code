#include "../include/chisel/logger/logger.h"

namespace chisel::logger {

    Logger::Logger(const uint16_t backtrace_size) :
        backtrace(backtrace_size) {
        for (auto &t: targets)
            t = nullptr;
    }

    bool Logger::add_sink(Sink *sink) {
        if (sink_count >= MAX_SINKS)
            return false;
        targets[sink_count++] = sink;
        sink->set_parent(this);
        return true;
    }

    void Logger::log(LogEntry &&log_entry) {
        for (uint8_t i = 0; i < sink_count; ++i) {
            if (targets[i])
                targets[i]->send_log(log_entry);
        }
        backtrace.push(std::move(log_entry));
    }

    void Logger::dump_backtrace() const {
        for (const auto backtrace_contents = this->backtrace.contents();
             const auto &log_entry: backtrace_contents) {
            for (auto *target: this->targets) {
                target->send_log(log_entry);
            }
        }
    }

}
