#pragma once

#include <memory>
#include <vector>
#include <cstdint>

namespace chisel {

    template<typename T>
    class CircularBuffer {
        const uint16_t MAX_SIZE;
        std::unique_ptr<T[]> buffer{};
        uint16_t size = 0;
        uint16_t head = 0;

    public:
        explicit CircularBuffer(uint16_t buffer_size)
                : MAX_SIZE(buffer_size),
                  buffer(std::make_unique<T[]>(buffer_size)) {}

        void push(const T &item) {
            buffer[head] = item;
            advance();
        }

        void push(T &&item) {
            buffer[head] = std::move(item);
            advance();
        }

        [[nodiscard]] std::vector<T> contents() const {
            std::vector<T> contents;
            contents.reserve(size);

            const uint16_t start = (head + MAX_SIZE - size) % MAX_SIZE;
            for (uint16_t i = 0; i < size; ++i) {
                contents.push_back(buffer[(start + i) % MAX_SIZE]);
            }

            return contents;
        }

        void clear() {
            size = 0;
            head = 0;
        }

        CircularBuffer(const CircularBuffer &) = delete;

        CircularBuffer &operator=(const CircularBuffer &) = delete;

        CircularBuffer(CircularBuffer &&) noexcept = default;

        CircularBuffer &operator=(CircularBuffer &&) noexcept = default;

    private:
        void advance() {
            head = (head + 1) % MAX_SIZE;
            if (size < MAX_SIZE) ++size;
        }
    };
}