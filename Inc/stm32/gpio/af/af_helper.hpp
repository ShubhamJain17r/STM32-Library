#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/gpio/af/af_traits.hpp"

namespace gpio::af
{

inline void setAlternateFunction(const Pin& pin, AlternateFunction af) noexcept
{
	const std::uint8_t index = pin.number / 8u;
	const std::uint8_t pos = (pin.number % 8u) * 4u;

	reg::modifyBits(pin.port->AFR[index], 0xFU << pos, static_cast<std::uint32_t>(af) << pos);
}

template<Signal signal>
constexpr bool supports(Pin pin) noexcept
{
    for(const auto& p : Traits<signal>::pins)
    {
        if(p == pin)
            return true;
    }

    return false;
}

template<Signal signal>
constexpr AlternateFunction alternateFunction(Pin pin) noexcept
{
    return supports<signal>(pin) ? Traits<signal>::af : AlternateFunction::INVALID;
}

} // namespace gpio::af
