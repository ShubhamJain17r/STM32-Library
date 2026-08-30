/**
 * @file i2c_af_traits.hpp
 * @brief Alternate function pin capability mappings for I2C peripherals on STM32F446
 */

#pragma once

#include <cstdint>
#include <array>

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/pin_map.hpp"
#include "stm32/gpio/af/af_types.hpp"
#include "stm32/i2c/core/i2c_types.hpp"

namespace i2c::af
{

using gpio::Pin;
using gpio::af::AlternateFunction;

enum class Signal : std::uint8_t
{
    SCL,
    SDA,
    SMBA
};

template<Instance I, Signal S>
struct SignalPins;

// ===========================================================================
// I2C1 (AF4)
// ===========================================================================
template<> struct SignalPins<Instance::i2c1, Signal::SCL> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 2> pins = {gpio::PB6, gpio::PB8};
};
template<> struct SignalPins<Instance::i2c1, Signal::SDA> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 2> pins = {gpio::PB7, gpio::PB9};
};
template<> struct SignalPins<Instance::i2c1, Signal::SMBA> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 1> pins = {gpio::PB5};
};

// ===========================================================================
// I2C2 (AF4)
// ===========================================================================
template<> struct SignalPins<Instance::i2c2, Signal::SCL> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 1> pins = {gpio::PB10};
};
template<> struct SignalPins<Instance::i2c2, Signal::SDA> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 2> pins = {gpio::PB11, gpio::PC12};
};
template<> struct SignalPins<Instance::i2c2, Signal::SMBA> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 1> pins = {gpio::PB12};
};

// ===========================================================================
// I2C3 (AF4)
// ===========================================================================
template<> struct SignalPins<Instance::i2c3, Signal::SCL> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 1> pins = {gpio::PA8};
};
template<> struct SignalPins<Instance::i2c3, Signal::SDA> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 2> pins = {gpio::PB4, gpio::PC9};
};
template<> struct SignalPins<Instance::i2c3, Signal::SMBA> {
    static constexpr AlternateFunction af = AlternateFunction::AF4;
    static constexpr std::array<Pin, 2> pins = {gpio::PA9, gpio::PB2};
};

template<Instance I, Signal S>
constexpr bool supports(Pin pin) noexcept
{
    for(const auto& p : SignalPins<I, S>::pins)
    {
        if(p == pin) return true;
    }
    return false;
}

template<Instance I, Signal S>
constexpr AlternateFunction alternateFunction(Pin pin) noexcept
{
    return supports<I, S>(pin) ? SignalPins<I, S>::af : AlternateFunction::INVALID;
}

} // namespace i2c::af

