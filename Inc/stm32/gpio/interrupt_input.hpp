#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include"stm32/common/callback.hpp"

#include "stm32/gpio/pin_map.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/gpio/gpio_helper.hpp"

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
