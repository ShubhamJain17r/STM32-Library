#pragma once

#include <cstdint>
#include <array>

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/pin_map.hpp"
#include "stm32/gpio/af/af_types.hpp"
#include "stm32/timer/timer_types.hpp"

namespace timer::af
{

using gpio::Pin;
using gpio::af::AlternateFunction;

template<Instance I, Channel C>
struct ChannelPins;

// ---------------------------------------------------------------------------
// TIM1 (AF1)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim1, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA8, gpio::PE9};
};
template<> struct ChannelPins<Instance::tim1, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA9, gpio::PE11};
};
template<> struct ChannelPins<Instance::tim1, Channel::CH3> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA10, gpio::PE13};
};
template<> struct ChannelPins<Instance::tim1, Channel::CH4> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA11, gpio::PE14};
};

// ---------------------------------------------------------------------------
// TIM2 (AF1)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim2, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 3> pins = {gpio::PA0, gpio::PA5, gpio::PA15};
};
template<> struct ChannelPins<Instance::tim2, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA1, gpio::PB3};
};
template<> struct ChannelPins<Instance::tim2, Channel::CH3> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA2, gpio::PB10};
};
template<> struct ChannelPins<Instance::tim2, Channel::CH4> {
    static constexpr AlternateFunction af = AlternateFunction::AF1;
    static constexpr std::array<Pin, 2> pins = {gpio::PA3, gpio::PB11};
};

// ---------------------------------------------------------------------------
// TIM3 (AF2)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim3, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 3> pins = {gpio::PA6, gpio::PB4, gpio::PC6};
};
template<> struct ChannelPins<Instance::tim3, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 3> pins = {gpio::PA7, gpio::PB5, gpio::PC7};
};
template<> struct ChannelPins<Instance::tim3, Channel::CH3> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PB0, gpio::PC8};
};
template<> struct ChannelPins<Instance::tim3, Channel::CH4> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PB1, gpio::PC9};
};

// ---------------------------------------------------------------------------
// TIM4 (AF2)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim4, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PB6, gpio::PD12};
};
template<> struct ChannelPins<Instance::tim4, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PB7, gpio::PD13};
};
template<> struct ChannelPins<Instance::tim4, Channel::CH3> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PB8, gpio::PD14};
};
template<> struct ChannelPins<Instance::tim4, Channel::CH4> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PB9, gpio::PD15};
};

// ---------------------------------------------------------------------------
// TIM5 (AF2)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim5, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PA0, gpio::PH10};
};
template<> struct ChannelPins<Instance::tim5, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PA1, gpio::PH11};
};
template<> struct ChannelPins<Instance::tim5, Channel::CH3> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 2> pins = {gpio::PA2, gpio::PH12};
};
template<> struct ChannelPins<Instance::tim5, Channel::CH4> {
    static constexpr AlternateFunction af = AlternateFunction::AF2;
    static constexpr std::array<Pin, 1> pins = {gpio::PA3};
};

// ---------------------------------------------------------------------------
// TIM8 (AF3)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim8, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 1> pins = {gpio::PC6};
};
template<> struct ChannelPins<Instance::tim8, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 1> pins = {gpio::PC7};
};
template<> struct ChannelPins<Instance::tim8, Channel::CH3> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 1> pins = {gpio::PC8};
};
template<> struct ChannelPins<Instance::tim8, Channel::CH4> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 1> pins = {gpio::PC9};
};

// ---------------------------------------------------------------------------
// TIM9 (AF3)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim9, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 2> pins = {gpio::PA2, gpio::PE5};
};
template<> struct ChannelPins<Instance::tim9, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 2> pins = {gpio::PA3, gpio::PE6};
};

// ---------------------------------------------------------------------------
// TIM10 (AF3)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim10, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 2> pins = {gpio::PB8, gpio::PF6};
};

// ---------------------------------------------------------------------------
// TIM11 (AF3)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim11, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF3;
    static constexpr std::array<Pin, 2> pins = {gpio::PB9, gpio::PF7};
};

// ---------------------------------------------------------------------------
// TIM12 (AF9)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim12, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF9;
    static constexpr std::array<Pin, 1> pins = {gpio::PB14};
};
template<> struct ChannelPins<Instance::tim12, Channel::CH2> {
    static constexpr AlternateFunction af = AlternateFunction::AF9;
    static constexpr std::array<Pin, 1> pins = {gpio::PB15};
};

// ---------------------------------------------------------------------------
// TIM13 (AF9)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim13, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF9;
    static constexpr std::array<Pin, 2> pins = {gpio::PA6, gpio::PF8};
};

// ---------------------------------------------------------------------------
// TIM14 (AF9)
// ---------------------------------------------------------------------------
template<> struct ChannelPins<Instance::tim14, Channel::CH1> {
    static constexpr AlternateFunction af = AlternateFunction::AF9;
    static constexpr std::array<Pin, 2> pins = {gpio::PA7, gpio::PF9};
};

// ---------------------------------------------------------------------------
// Helper utilities
// ---------------------------------------------------------------------------
template<Instance I, Channel C>
constexpr bool supports(Pin pin) noexcept
{
    for(const auto& p : ChannelPins<I, C>::pins)
    {
        if(p == pin) return true;
    }
    return false;
}

template<Instance I, Channel C>
constexpr AlternateFunction alternateFunction(Pin pin) noexcept
{
    return supports<I, C>(pin) ? ChannelPins<I, C>::af : AlternateFunction::INVALID;
}

} // namespace timer::af
