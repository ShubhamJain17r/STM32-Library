/**
 * @file adc_pin_traits.hpp
 * @brief Pin-to-Channel capability mappings for ADC peripherals on STM32F446
 */

#pragma once

#include <cstdint>
#include <optional>

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/pin_map.hpp"
#include "stm32/adc/core/adc_types.hpp"

namespace adc::pin_traits
{

using gpio::Pin;

template<Instance I>
constexpr std::optional<Channel> getChannel(Pin pin) noexcept
{
    if(pin == gpio::PA0) return Channel::IN0;
    if(pin == gpio::PA1) return Channel::IN1;
    if(pin == gpio::PA2) return Channel::IN2;
    if(pin == gpio::PA3) return Channel::IN3;

    if(pin == gpio::PC0) return Channel::IN10;
    if(pin == gpio::PC1) return Channel::IN11;
    if(pin == gpio::PC2) return Channel::IN12;
    if(pin == gpio::PC3) return Channel::IN13;

    if constexpr(I == Instance::adc1 || I == Instance::adc2)
    {
        if(pin == gpio::PA4) return Channel::IN4;
        if(pin == gpio::PA5) return Channel::IN5;
        if(pin == gpio::PA6) return Channel::IN6;
        if(pin == gpio::PA7) return Channel::IN7;
        if(pin == gpio::PB0) return Channel::IN8;
        if(pin == gpio::PB1) return Channel::IN9;
        if(pin == gpio::PC4) return Channel::IN14;
        if(pin == gpio::PC5) return Channel::IN15;
    }

    return std::nullopt;
}

template<Instance I>
constexpr std::optional<Pin> getPin(Channel ch) noexcept
{
    switch(ch)
    {
        case Channel::IN0:  return gpio::PA0;
        case Channel::IN1:  return gpio::PA1;
        case Channel::IN2:  return gpio::PA2;
        case Channel::IN3:  return gpio::PA3;
        case Channel::IN10: return gpio::PC0;
        case Channel::IN11: return gpio::PC1;
        case Channel::IN12: return gpio::PC2;
        case Channel::IN13: return gpio::PC3;

        case Channel::IN4:  return (I != Instance::adc3) ? std::optional<Pin>(gpio::PA4) : std::nullopt;
        case Channel::IN5:  return (I != Instance::adc3) ? std::optional<Pin>(gpio::PA5) : std::nullopt;
        case Channel::IN6:  return (I != Instance::adc3) ? std::optional<Pin>(gpio::PA6) : std::nullopt;
        case Channel::IN7:  return (I != Instance::adc3) ? std::optional<Pin>(gpio::PA7) : std::nullopt;
        case Channel::IN8:  return (I != Instance::adc3) ? std::optional<Pin>(gpio::PB0) : std::nullopt;
        case Channel::IN9:  return (I != Instance::adc3) ? std::optional<Pin>(gpio::PB1) : std::nullopt;
        case Channel::IN14: return (I != Instance::adc3) ? std::optional<Pin>(gpio::PC4) : std::nullopt;
        case Channel::IN15: return (I != Instance::adc3) ? std::optional<Pin>(gpio::PC5) : std::nullopt;

        default:
            return std::nullopt; // Internal channels (Vref, Temp, Vbat)
    }
}

template<Instance I>
constexpr bool supports(Pin pin) noexcept
{
    return getChannel<I>(pin).has_value();
}

} // namespace adc::pin_traits

