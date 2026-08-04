#pragma once

#include <cstdint>

namespace buffer
{

template<typename T, std::size_t Capacity>
class RingBuffer
{
public:

    bool push(const T&) noexcept;

    bool pop(T&) noexcept;

    bool empty() const noexcept;

    bool full() const noexcept;

    std::size_t size() const noexcept;

    constexpr std::size_t capacity() const noexcept;

    std::size_t freeSpace() const noexcept;

    void clear() noexcept;

private:

    T buffer_[Capacity];

    std::size_t head_;

    std::size_t tail_;

    std::size_t count_;
};

} // namesapce buffer
