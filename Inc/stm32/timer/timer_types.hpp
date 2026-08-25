#pragma once

#include <cstdint>

namespace timer
{

enum class Instance : std::uint8_t
{
    tim1,
    tim2,
    tim3,
    tim4,
    tim5,
    tim6,
    tim7,
    tim8,
    tim9,
    tim10,
    tim11,
    tim12,
    tim13,
    tim14
};

enum class Channel : std::uint8_t
{
    CH1 = 1,
    CH2 = 2,
    CH3 = 3,
    CH4 = 4
};

enum class CounterMode : std::uint8_t
{
    Up             = 0,
    Down           = 1,
    CenterAligned1 = 2, // Output compare interrupt flag set only when counting down
    CenterAligned2 = 3, // Output compare interrupt flag set only when counting up
    CenterAligned3 = 4  // Output compare interrupt flag set both counting up and down
};

enum class PwmMode : std::uint8_t
{
    Mode1 = 6, // Active when CNT < CCR, Inactive when CNT >= CCR
    Mode2 = 7  // Inactive when CNT < CCR, Active when CNT >= CCR
};

enum class OutputCompareMode : std::uint8_t
{
    Frozen          = 0,
    ActiveOnMatch   = 1,
    InactiveOnMatch = 2,
    ToggleOnMatch   = 3,
    ForcedInactive  = 4,
    ForcedActive    = 5,
    Pwm1            = 6,
    Pwm2            = 7
};

enum class CaptureEdge : std::uint8_t
{
    Rising  = 0,
    Falling = 1,
    Both    = 2
};

enum class Polarity : std::uint8_t
{
    ActiveHigh = 0,
    ActiveLow  = 1
};

enum class InputPrescaler : std::uint8_t
{
    Div1 = 0, // Capture on every edge
    Div2 = 1, // Capture once every 2 events
    Div4 = 2, // Capture once every 4 events
    Div8 = 3  // Capture once every 8 events
};

} // namespace timer
