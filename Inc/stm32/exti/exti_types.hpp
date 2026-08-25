#pragma once

#include <cstdint>

namespace exti
{

enum class Trigger : std::uint8_t
{
    None    = 0,
    Rising  = 1,
    Falling = 2,
    Both    = 3
};

enum class Mode : std::uint8_t
{
    Interrupt,
    Event,
    InterruptAndEvent
};

enum class Edge : std::uint8_t
{
    Rising,
    Falling
};

using EdgeCallback = void(*)(Edge);

inline constexpr Trigger operator|(Trigger lhs, Trigger rhs) noexcept
{
    return static_cast<Trigger>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

inline constexpr Trigger operator&(Trigger lhs, Trigger rhs) noexcept
{
    return static_cast<Trigger>(static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
}

} // namespace exti

