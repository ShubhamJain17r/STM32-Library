/**
 * @file interrupt_input.cpp
 * @brief Implementation of InterruptInput pin configuration and RAII cleanup
 */

#include "stm32/gpio/interrupt_input/interrupt_input.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/exti/exti_manager.hpp"

namespace gpio
{

InterruptInput::InterruptInput(Pin pin, InterruptInputConfig config) : pin_(pin)
{
	pin_.enableClock();

	helper::setMode(pin_, Mode::INPUT);
	helper::setPull(pin_, config.pull);

	rcc::enablePeripheralClock(SYSCFG);

	exti::configureExticr(pin_);
	exti::setTrigger(pin_, config.trigger);

	exti::ExtiManager::registerPin(pin_, config.trigger, config.callback, config.edgeCallback);

	if(config.risingCallback)
	{
		exti::ExtiManager::setRisingCallback(pin_, config.risingCallback);
	}
	if(config.fallingCallback)
	{
		exti::ExtiManager::setFallingCallback(pin_, config.fallingCallback);
	}

	exti::clearPending(pin_.number);
	exti::enableInterrupt(pin_);
	exti::enableIRQ(pin_);
}

InterruptInput::InterruptInput(Pin pin, exti::Trigger trigger, stm32::Callback cb, Pull pull)
    : InterruptInput(pin, InterruptInputConfig{pull, trigger, cb, nullptr, nullptr, nullptr})
{
}

InterruptInput::~InterruptInput()
{
	exti::disableInterrupt(pin_);
	exti::setTrigger(pin_, exti::Trigger::None);
	exti::clearPending(pin_.number);
	exti::ExtiManager::unregisterPin(pin_);
}

void InterruptInput::enable() const noexcept
{
	exti::enableInterrupt(pin_);
}

void InterruptInput::disable() const noexcept
{
	exti::disableInterrupt(pin_);
}

void InterruptInput::softwareTrigger() const noexcept
{
	exti::softwareTrigger(pin_.number);
}

} // namespace gpio

