#pragma once

#include <array>

// A ring buffer with constant length and a way to access past data.

// Example:
// RingBuffer<float, 40> buf{0.f};

template <typename T, size_t N>
class RingBuffer {
public:
    RingBuffer(const T& init) {
        vals.fill(init);
    }

    void reset(const T& val) {
        vals.fill(val);
    }

    // Add an element (and removing element at index N)
    void push(const T& val) {
        pos++;
        pos %= N;
        vals[pos] = val;
    }

    // Get nth last pushed element. 0 <= n < N.
    const T& at(size_t n) const {
        if (pos < n) {
            return vals[pos+N-n];
        }
        return vals[pos-n];
    }

    const T& oldest() const {
        return at(N-1);
    }

private:
    std::array<T, N> vals;
    size_t pos = N-1;
};
