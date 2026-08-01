#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio::detail
{

inline void setMode(const Pin& pin, Mode mode) noexcept
{
	pin.port->MODER &= ~(3U << (pin.number * 2));
	pin.port->MODER |=
		(static_cast<std::uint32_t>(mode) << (pin.number * 2));
}

inline void setOutputType(const Pin& pin,
						  OutputType type) noexcept
{
	pin.port->OTYPER &= ~pin.mask();

	pin.port->OTYPER |=
		(static_cast<std::uint32_t>(type) << pin.number);
}

inline void setOutputSpeed(const Pin& pin,
						   OutputSpeed speed) noexcept
{
	pin.port->OSPEEDR &= ~(3U << (pin.number * 2));

	pin.port->OSPEEDR |=
		(static_cast<std::uint32_t>(speed) << (pin.number * 2));
}

inline void setPull(const Pin& pin,
					Pull pull) noexcept
{
	pin.port->PUPDR &= ~(3U << (pin.number * 2));

	pin.port->PUPDR |=
		(static_cast<std::uint32_t>(pull) << (pin.number * 2));
}

inline void write(const Pin& pin,
				  PinState state) noexcept
{
	if(state == PinState::HIGH)
	{
		pin.port->BSRR = pin.mask();
	}
	else
	{
		pin.port->BSRR = pin.mask() << 16;
	}
}

inline void high(const Pin& pin) noexcept
{
	pin.port->BSRR = pin.mask();
}

inline void low(const Pin& pin) noexcept
{
	pin.port->BSRR = pin.mask() << 16;
}

inline void toggle(const Pin& pin) noexcept
{
	pin.port->ODR ^= pin.mask();
}

inline PinState readInput(const Pin& pin) noexcept
{
	return (pin.port->IDR & pin.mask())
		? PinState::HIGH
		: PinState::LOW;
}

inline PinState readOutput(const Pin& pin) noexcept
{
	return (pin.port->ODR & pin.mask())
		? PinState::HIGH
		: PinState::LOW;
}

} // namespace gpio::detail
