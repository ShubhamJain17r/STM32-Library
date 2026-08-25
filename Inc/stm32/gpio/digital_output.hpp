#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio
{

struct DigitalOutputConfig
{
	OutputType outputType = OutputType::PUSH_PULL;
	OutputSpeed outputSpeed = OutputSpeed::LOW;
	Pull pull = Pull::NONE;
	PinState initialState = PinState::LOW;
};

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
		state == PinState::HIGH ? high() : low();
	}

	inline void high() const noexcept
	{
		reg::write(pin_.port->BSRR, pin_.mask());
	}

	inline void low() const noexcept
	{
		reg::write(pin_.port->BSRR, pin_.mask() << 16u);
	}

	inline void toggle() const noexcept
	{
		pin_.port->ODR ^= pin_.mask();
	}

	inline PinState read() const noexcept
	{
		return reg::isAnyBitSet(pin_.port->ODR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
	}
};

} // namespace gpio
