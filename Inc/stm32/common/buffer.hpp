/**
 * @file buffer.hpp
 * @brief Statically allocated, type-safe circular ring buffer template
 * 
 * Used for interrupt-driven communications (UART RX/TX, SPI, I2C buffers).
 * Provides FIFO push/pop semantics with compile-time capacity checking.
 */

#pragma once

#include <cstdint>

namespace buffer
{

template<typename T, std::size_t N>
class RingBuffer
{
    static_assert(N > 0, "RingBuffer capacity must be greater than zero");

public:
    /**
     * @brief Pushes an item into the buffer.
     * @return true if pushed successfully, false if the buffer is full.
     */
    bool push(const T& value) noexcept;

    /**
     * @brief Pops the oldest item from the buffer.
     * @return true if popped successfully, false if the buffer is empty.
     */
    bool pop(T& value) noexcept;

    /**
     * @brief Returns true if the buffer contains 0 elements.
     */
    bool empty() const noexcept;

    /**
     * @brief Returns true if the buffer has reached maximum capacity N.
     */
    bool full() const noexcept;

    /**
     * @brief Clears the buffer indices and count to 0.
     */
    void clear() noexcept;

    /**
     * @brief Returns the current number of elements in the buffer.
     */
    std::size_t size() const noexcept;

    /**
     * @brief Returns the maximum capacity N of the buffer.
     */
    constexpr std::size_t capacity() const noexcept;

    /**
     * @brief Returns a reference to the front (oldest) element without removing it.
     */
    T& front() noexcept;
    const T& front() const noexcept;

    /**
     * @brief Returns a reference to the back (newest) element.
     */
    T& back() noexcept;
    const T& back() const noexcept;

private:
    T buffer_[N];
    std::size_t head_ = 0;
    std::size_t tail_ = 0;
    std::size_t count_ = 0;
};

template<typename T, std::size_t N>
bool RingBuffer<T, N>::push(const T& value) noexcept
{
    if(full())
    {
        return false;
    }

    buffer_[head_] = value;
    head_++;

    if(head_ == N)
    {
        head_ = 0;
    }

    count_++;
    return true;
}

template<typename T, std::size_t N>
bool RingBuffer<T, N>::pop(T& value) noexcept
{
    if(empty())
    {
        return false;
    }

    value = buffer_[tail_];
    tail_++;

    if(tail_ == N)
    {
        tail_ = 0;
    }

    count_--;
    return true;
}

template<typename T, std::size_t N>
bool RingBuffer<T, N>::empty() const noexcept
{
    return count_ == 0;
}

template<typename T, std::size_t N>
bool RingBuffer<T, N>::full() const noexcept
{
    return count_ == N;
}

template<typename T, std::size_t N>
void RingBuffer<T, N>::clear() noexcept
{
    head_ = 0;
    tail_ = 0;
    count_ = 0;
}

template<typename T, std::size_t N>
std::size_t RingBuffer<T, N>::size() const noexcept
{
    return count_;
}

template<typename T, std::size_t N>
constexpr std::size_t RingBuffer<T, N>::capacity() const noexcept
{
    return N;
}

template<typename T, std::size_t N>
T& RingBuffer<T, N>::front() noexcept
{
    return buffer_[tail_];
}

template<typename T, std::size_t N>
const T& RingBuffer<T, N>::front() const noexcept
{
    return buffer_[tail_];
}

template<typename T, std::size_t N>
T& RingBuffer<T, N>::back() noexcept
{
    std::size_t index = (head_ == 0) ? N - 1 : head_ - 1;
    return buffer_[index];
}

template<typename T, std::size_t N>
const T& RingBuffer<T, N>::back() const noexcept
{
    std::size_t index = (head_ == 0) ? N - 1 : head_ - 1;
    return buffer_[index];
}

} // namespace buffer
