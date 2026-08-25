#pragma once

#include <cstdint>

namespace buffer
{

template<typename T, std::size_t N>
class RingBuffer
{
    static_assert(N > 0, "RingBuffer capacity must be greater than zero");

public:
    bool push(const T& value) noexcept;
    bool pop(T& value) noexcept;

    bool empty() const noexcept;
    bool full() const noexcept;

    void clear() noexcept;

    std::size_t size() const noexcept;
    constexpr std::size_t capacity() const noexcept;

    T& front() noexcept;
    const T& front() const noexcept;

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
