#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include"stm32/common/callback.hpp"

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio
{

struct InterruptInputConfig
{
	Pull pull = Pull::NONE;
	Callback risingEdgeCallback = nullptr;
	Callback fallingEdgeCallback = nullptr;
};

class InterruptInput
{
private:
	Pin pin_;

public:
	InterruptInput() = delete;
	~InterruptInput() = default;

	InterruptInput(const InterruptInput&) = delete;
	InterruptInput& operator=(const InterruptInput&) = delete;

	InterruptInput(InterruptInput&&) = delete;
	InterruptInput& operator=(InterruptInput&&) = delete;

	explicit InterruptInput(Pin pin, InterruptInputConfig config = {});

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

	inline bool isHigh() const noexcept
	{
		return (read() == PinState::HIGH);
	}

	inline bool isLow() const noexcept
	{
		return !isHigh();
	}
};

} // namespace gpio
