#include <iostream>
#include <array>
#include <stdexcept>

template <typename T, size_t Capacity>
class CircularQueue {
public:

    CircularQueue() : head(0), tail(0), full(false) {}

    void push(const T& item) {
        buffer[tail] = item;
        tail = (tail + 1) % Capacity;
        if (full) {
            head = (head + 1) % Capacity;
        }
        full = tail == head;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        head = (head + 1) % Capacity;
        full = false;
    }

    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return buffer[head];
    }

    size_t size() const {
        if (full) {
            return Capacity;
        }
        if (tail >= head) {
            return tail - head;
        }
        return Capacity - head + tail;
    }

    bool empty() const {
        return (!full && (head == tail));
    }

    bool is_full() const {
        return full;
    }

    class Iterator {
    public:
        Iterator(T* buffer, size_t capacity, size_t pos, size_t head)
            : buffer(buffer), capacity(capacity), pos(pos), head(head) {
        }

        T& operator*() { return buffer[(head + pos) % capacity]; }

        Iterator& operator++() {
            ++pos;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return pos != other.pos;
        }

    private:
        T* buffer;
        size_t capacity;
        size_t pos;
        size_t head;
    };

    Iterator begin() {
        return Iterator(buffer.data(), Capacity, 0, head);
    }

    Iterator end() {
        return Iterator(buffer.data(), Capacity, size(), head);
    }

private:
    std::array<T, Capacity> buffer = {};
    size_t head;
    size_t tail;
    bool full;
};

int main() {
    CircularQueue<int, 5> queue;

    for (int i = 0; i < 5; i++) {
        queue.push(i);
    }

    for (const auto& item : queue) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    queue.push(5);
    for (const auto& item : queue) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    queue.pop();
    for (const auto& item : queue) {
        std::cout << item << " ";
    }

    std::cout << std::endl;
}