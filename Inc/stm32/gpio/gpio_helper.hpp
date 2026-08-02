#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio::helper
{

inline void setMode(const Pin& pin, Mode mode) noexcept
{
	pin.port->MODER &= ~(3U << (pin.number * 2));
	pin.port->MODER |=  (static_cast<std::uint32_t>(mode) << (pin.number * 2));
}

inline void setOutputType(const Pin& pin, OutputType type) noexcept
{
	pin.port->OTYPER &= ~pin.mask();

	pin.port->OTYPER |= (static_cast<std::uint32_t>(type) << pin.number);
}

inline void setOutputSpeed(const Pin& pin, OutputSpeed speed) noexcept
{
	pin.port->OSPEEDR &= ~(3U << (pin.number * 2));

	pin.port->OSPEEDR |=  (static_cast<std::uint32_t>(speed) << (pin.number * 2));
}

inline void setPull(const Pin& pin, Pull pull) noexcept
{
	pin.port->PUPDR &= ~(3U << (pin.number * 2));

	pin.port->PUPDR |=  (static_cast<std::uint32_t>(pull) << (pin.number * 2));
}

} // namespace gpio::helper
