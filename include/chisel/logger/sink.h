#pragma once

#include "log_entry.h"

namespace chisel::logger {
    // Forward declaration to logger, as logger already includes this header.
    class Logger;

    /**
     * @class Sink
     * @brief Abstract base class representing a sink for logging.
     *
     * The Sink class defines the interface for handling log entries. Derived classes
     * must implement the send_log method to process and store log messages as needed.
     * Implementations can forward logs to different outputs such as files, consoles,
     * or remote locations.
     */
    class Sink {
    public:
        virtual void set_parent(Logger* logger) = 0;
        virtual void send_log(const LogEntry& log) = 0;
        virtual ~Sink() = default;
    };

    /**
     * @class FunctionSink
     * @brief Final concrete class implementing a logging sink with a user-defined callback function.
     *
     * The FunctionSink class allows users to specify a custom callback function to handle log entries.
     * When a log entry is sent to this sink, the callback function is invoked with the log entry as its parameter.
     * This provides flexibility in handling log messages, enabling users to route logs to various targets
     * or perform custom processing.
     */
    class FunctionSink final : public Sink {
        using LogFunction = void (*)(const LogEntry&);
        LogFunction callback;

        Logger* parent = nullptr;

    public:
        /**
         * FunctionSink constructor
         *
         * Example usage:
         * \code
         * void my_callback(const LogEntry &log) {
         *     // Do something with the log entry
         * }
         *
         * FunctionSink sink(my_callback);
         * \endcode
         *
         * @param fn Function pointer to the callback function.
         */
        explicit FunctionSink(const LogFunction fn) : callback(fn) {
        }

        void set_parent(Logger* logger) override {
            parent = logger;
        }

        void send_log(const LogEntry& log) override {
            if (callback) callback(log);
        }
    };
}
