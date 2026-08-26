/**
 * @file timer_types.hpp
 * @brief Core enums for STM32 Timers, PWM, Output Compare, and Input Capture
 */

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
    CenterAligned1 = 2,
    CenterAligned2 = 3,
    CenterAligned3 = 4
};

enum class PwmMode : std::uint8_t
{
    Mode1 = 6,
    Mode2 = 7
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
    Div1 = 0,
    Div2 = 1,
    Div4 = 2,
    Div8 = 3
};

} // namespace timer

