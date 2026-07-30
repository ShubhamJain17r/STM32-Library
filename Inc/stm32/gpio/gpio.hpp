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
	inline void setModeOutput() const noexcept
	{
		pin_.port->MODER &= ~(3U << (pin_.number * 2));
		pin_.port->MODER |=  (static_cast<std::uint8_t>(Mode::OUTPUT) << (pin_.number * 2));
	}

	inline void configureOutputType(OutputType outputType) const noexcept
	{
		pin_.port->OTYPER &= pin_.mask();
		pin_.port->OTYPER |=  (static_cast<std::uint8_t>(outputType) << pin_.number);
	}

	inline void configureOutputSpeed(OutputSpeed outputSpeed) const noexcept
	{
		pin_.port->OSPEEDR &= ~(3U << (pin_.number * 2));
		pin_.port->OSPEEDR |=  (static_cast<std::uint8_t>(outputSpeed) << (pin_.number * 2));
	}

	inline void configurePull(Pull pull) const noexcept
	{
		pin_.port->PUPDR &= ~(3U << (pin_.number * 2));
		pin_.port->PUPDR |=  (static_cast<std::uint8_t>(pull) << (pin_.number * 2));
	}

public:
	inline void write(PinState state) const noexcept
	{
		state == PinState::HIGH ? high() : low();
	}

	inline void high() const noexcept
	{
		pin_.port->BSRR = pin_.mask();
	}

	inline void low() const noexcept
	{
		pin_.port->BSRR = (pin_.mask() << 16);
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
