#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/gpio/af/af_types.hpp"

namespace gpio::af
{

inline void setAlternateFunction(const Pin& pin, AlternateFunction af)
{
	std::uint8_t index = pin.number / 8;
	std::uint32_t pos = (pin.number % 8) * 4;

	pin.port->AFR[index] &= ~(0xF << pos);
	pin.port->AFR[index] |=  (af << pos);
}

template<Signal signal>
constexpr bool supports(Pin pin)
{
    for(const auto& p : Traits<signal>::pins)
    {
        if(p == pin)
            return true;
    }

    return false;
}

template<Signal signal>
constexpr AlternateFunction alternateFunction(Pin pin)
{
    return supports<signal>(pin) ? Traits<signal>::af : AlternateFunction::INVALID;
}

} // namespace gpio::af
