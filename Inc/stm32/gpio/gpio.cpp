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

} // namespace gpio
