#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin_map.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio
{

class DigitalInput
{
private:
	Pin pin_;

public:
	DigitalInput() = delete;
	~DigitalInput() = default;

	DigitalInput(const DigitalInput&) = delete;
	DigitalInput& operator=(const DigitalInput&) = delete;

	DigitalInput(DigitalInput&&) = delete;
	DigitalInput& operator=(DigitalInput&&) = delete;

	explicit DigitalInput(Pin pin, Pull pull = Pull::NONE);
private:
	inline void setModeInput() const noexcept
	{
		pin_.port->MODER &= ~(3U << (pin_.number * 2));
		pin_.port->MODER |=  (static_cast<std::uint8_t>(Mode::INPUT) << (pin_.number * 2));
	}

	inline void configurePull(Pull pull) const noexcept
	{
		pin_.port->PUPDR &= ~(3U << (pin_.number * 2));
		pin_.port->PUPDR |=  (static_cast<std::uint8_t>(pull) << (pin_.number * 2));
	}

public:
	inline PinState read() const noexcept
	{
		if(pin_.port->IDR & pin_.mask())
		{
			return PinState::HIGH;
		}
		return PinState::LOW;
	}
};

} // namespace gpio
