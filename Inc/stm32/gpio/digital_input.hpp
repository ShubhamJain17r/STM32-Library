#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin_map.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/gpio/gpio_helper.hpp"

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
