#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin_map.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio
{

class DigitalOutput
{
private:
	Pin pin_;

public:
	DigitalOutput() = delete;
	~DigitalOutput() = default;

	DigitalOutput(const DigitalOutput&) = delete;
	DigitalOutput& operator=(const DigitalOutput&) = delete;

	DigitalOutput(DigitalOutput&&) = delete;
	DigitalOutput& operator=(DigitalOutput&&) = delete;

	explicit DigitalOutput(Pin pin, DigitalOutputConfig config = {});

private:

public:
	inline void write(PinState state) const noexcept
	{
		if(state == PinState::HIGH)
		{
			high();
		}
		else
		{
			low();
		}
	}

	inline void high() const noexcept
	{
		pin_.port->BSRR = pin_.mask();
	}

	inline void low() const noexcept
	{
		pin_.port->BSRR = pin_.mask();
	}

	inline void toggle() const noexcept
	{
		pin_.port->ODR ^= pin_.mask();
	}

	inline PinState read() const noexcept
	{
		if(pin_.port->ODR & pin_.mask())
		{
			return PinState::HIGH;
		}
		return PinState::LOW;
	}
};

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

	explicit DigitalInput(Pin pin, DigitalInputConfig config = {});
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
