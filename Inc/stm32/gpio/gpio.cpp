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

DigitalInput::DigitalInput(Pin pin, DigitalInputConfig config) : pin_(pin)
{
	pin_.enableClock();
}

} // namespace gpio
