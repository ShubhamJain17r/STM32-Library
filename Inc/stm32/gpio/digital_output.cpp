#include "stm32/gpio/digital_output.hpp"

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

} // namespace gpio
