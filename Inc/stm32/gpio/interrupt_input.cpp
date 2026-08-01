#include "stm32/gpio/interrupt_input.hpp"

#include "stm32/gpio/exti_manager.hpp"

namespace gpio
{

InterruptInput::InterruptInput(Pin pin, InterruptInputConfig config) : pin_(pin)
{
	pin_.enableClock();

	detail::setMode(pin_, Mode::INPUT);
	detail::setPull(pin_, config.pull);

	rcc::enableSyscfgClock();

	exti::configureExticr(pin_);

	if(config.fallingEdgeCallback)
	{
		exti::enableFallingTrigger(pin_);
		exti::ExtiManager::setFallingCallback(pin_, config.fallingEdgeCallback);
	}

	if(config.risingEdgeCallback)
	{
		exti::enableRisingTrigger(pin_);
		exti::ExtiManager::setRisingCallback(pin_, config.risingEdgeCallback);
	}

	exti::enableInterrupt(pin_);
	exti::enableIRQ(pin_);
}

} // namespace gpio
