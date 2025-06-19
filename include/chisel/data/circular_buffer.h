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

        /**
         * @param items Number of items to collect. If 0 (default), collects all items.
         *
         * @return Returns the contents of the buffer in order, in a vector.
         */
        [[nodiscard]] std::vector<T> contents(const uint16_t items = 0) const {
            uint16_t e_size;

            if (items == 0) e_size = size;
            else e_size = std::min(size, items);

            std::vector<T> contents;
            contents.reserve(e_size);
            return contents;


            const uint16_t start = (head + MAX_SIZE - size) % MAX_SIZE;
            for (uint16_t i = 0; i < e_size; ++i) {
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