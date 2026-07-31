#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/gpio.hpp"

namespace gpio
{

DigitalOutput::DigitalOutput(Pin pin, DigitalOutputConfig config) : pin_(pin)
{
	pin_.enableClock();

	setModeOutput();
	configureOutputType(config.outputType);
	configureOutputSpeed(config.outputSpeed);
	configurePull(config.pull);

	write(config.initialState);

}

DigitalInput::DigitalInput(Pin pin, Pull pull) : pin_(pin)
{
	pin_.enableClock();

	setModeInput();
	configurePull(pull);
}

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

		ExtiManager::setFallingCallback(pin_, config.fallingEdgeCallback);
	}
	if(config.risingEdgeCallback)
	{
		EXTI->RTSR |= pin_.mask();

		ExtiManager::setRisingCallback(pin_, config.risingEdgeCallback);
	}

	EXTI->IMR |= pin_.mask();

	NVIC_EnableIRQ(exti::extiIRQ(pin_.number));
}

void ExtiManager::setRisingCallback(const Pin& pin, Callback cb)
{
    auto& entry = table[pin.number];

    entry.port = pin.port;
    entry.rising = cb;
}

void ExtiManager::setFallingCallback(const Pin& pin, Callback cb)
{
    auto& entry = table[pin.number];

    entry.port = pin.port;
    entry.falling = cb;
}

void ExtiManager::handleInterrupt(std::uint8_t line)
{
    EXTI->PR = (1u << line);

    auto& entry = table[line];

    bool state = entry.port->IDR & (1u << line);

    if(state)
    {
        if(entry.rising)
            entry.rising();
    }
    else
    {
        if(entry.falling)
            entry.falling();
    }
}

} // namespace gpio

extern "C"
{

void EXTI0_IRQHandler()
{
    gpio::ExtiManager::handleInterrupt(0);
}

void EXTI1_IRQHandler()
{
	gpio::ExtiManager::handleInterrupt(1);
}

void EXTI2_IRQHandler()
{
	gpio::ExtiManager::handleInterrupt(2);
}

void EXTI3_IRQHandler()
{
	gpio::ExtiManager::handleInterrupt(3);
}

void EXTI4_IRQHandler()
{
	gpio::ExtiManager::handleInterrupt(4);
}

void EXTI9_5_IRQHandler()
{
    for(int line = 5; line <= 9; ++line)
    {
        if(EXTI->PR & (1u << line))
        		gpio::ExtiManager::handleInterrupt(line);
    }
}

void EXTI15_10_IRQHandler()
{
    for(int line = 10; line <= 15; ++line)
    {
        if(EXTI->PR & (1u << line))
        		gpio::ExtiManager::handleInterrupt(line);
    }
}



} // extern "C"
