#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/common/callback.hpp"
#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/exti/exti_types.hpp"

namespace gpio
{

struct InterruptInputConfig
{
	Pull               pull            = Pull::NONE;
	exti::Trigger      trigger         = exti::Trigger::Falling;
	stm32::Callback    callback        = nullptr;
	exti::EdgeCallback edgeCallback    = nullptr;
	stm32::Callback    risingCallback  = nullptr;
	stm32::Callback    fallingCallback = nullptr;
};

class InterruptInput
{
private:
	Pin pin_;

public:
	InterruptInput() = delete;
	~InterruptInput();

	InterruptInput(const InterruptInput&) = delete;
	InterruptInput& operator=(const InterruptInput&) = delete;

	InterruptInput(InterruptInput&&) = delete;
	InterruptInput& operator=(InterruptInput&&) = delete;

	explicit InterruptInput(Pin pin, InterruptInputConfig config = {});
	InterruptInput(Pin pin, exti::Trigger trigger, stm32::Callback cb, Pull pull = Pull::NONE);

	inline PinState read() const noexcept
	{
		return reg::isAnyBitSet(pin_.port->IDR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
	}

	inline bool isHigh() const noexcept
	{
		return (read() == PinState::HIGH);
	}

	inline bool isLow() const noexcept
	{
		return !isHigh();
	}

	void enable() const noexcept;
	void disable() const noexcept;
	void softwareTrigger() const noexcept;
};

} // namespace gpio
