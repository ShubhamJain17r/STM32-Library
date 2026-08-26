/**
 * @file gpio_helper.hpp
 * @brief Low-level register manipulation helper functions for GPIO
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"

namespace gpio::helper
{

inline void setMode(const Pin& pin, Mode mode) noexcept
{
	const std::uint8_t shift = pin.number * 2u;
	reg::modifyBits(pin.port->MODER, 3U << shift, static_cast<std::uint32_t>(mode) << shift);
}

inline void setOutputType(const Pin& pin, OutputType type) noexcept
{
	reg::modifyBits(pin.port->OTYPER, pin.mask(), static_cast<std::uint32_t>(type) << pin.number);
}

inline void setOutputSpeed(const Pin& pin, OutputSpeed speed) noexcept
{
	const std::uint8_t shift = pin.number * 2u;
	reg::modifyBits(pin.port->OSPEEDR, 3U << shift, static_cast<std::uint32_t>(speed) << shift);
}

inline void setPull(const Pin& pin, Pull pull) noexcept
{
	const std::uint8_t shift = pin.number * 2u;
	reg::modifyBits(pin.port->PUPDR, 3U << shift, static_cast<std::uint32_t>(pull) << shift);
}

} // namespace gpio::helper

