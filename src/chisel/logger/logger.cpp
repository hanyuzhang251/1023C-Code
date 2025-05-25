#include "logger.h"

namespace chisel::logger {

    Logger::Logger(const uint16_t size, std::initializer_list<Sink *> sinks)
            : backtrace(size), targets(sinks) {
        for (auto* sink : sinks) {
            if (!sink) throw std::invalid_argument("Null sink passed to Logger");
        }
    }

    void Logger::log(LogEntry log_entry) {
        for (auto target : targets) {
            target->send_log(log_entry);
        }
        backtrace.push(std::move(log_entry));
    }

    void Logger::dump_backtrace() const {
        const auto backtrace_contents = this->backtrace.contents();
        for (const auto& log_entry : backtrace_contents) {
            for (auto* target : this->targets) {
                target->send_log(log_entry);
            }
        }
    }

}