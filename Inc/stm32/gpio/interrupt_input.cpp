#include "stm32/gpio/interrupt_input.hpp"

#include "stm32/gpio/exti_manager.hpp"

namespace gpio
{

InterruptInput::InterruptInput(Pin pin, InterruptInputConfig config) : pin_(pin)
{
	pin_.enableClock();

	setModeInput();
	configurePull(config.pull);

	rcc::enableSyscfgClock();

	std::uint8_t index = pin_.number / 4;
	std::uint16_t pos = (index % 4) * 4;

	SYSCFG->EXTICR[index] &= ~(0xF << pos);
	SYSCFG->EXTICR[index] |=  (pin_.portIndex() << pos);

	if(config.fallingEdgeCallback)
	{
		EXTI->FTSR |= pin_.mask();

		exti::ExtiManager::setFallingCallback(pin_, config.fallingEdgeCallback);
	}
	if(config.risingEdgeCallback)
	{
		EXTI->RTSR |= pin_.mask();

		exti::ExtiManager::setRisingCallback(pin_, config.risingEdgeCallback);
	}

	EXTI->IMR |= pin_.mask();

	NVIC_EnableIRQ(exti::extiIRQ(pin_.number));
}

} // namespace gpio
