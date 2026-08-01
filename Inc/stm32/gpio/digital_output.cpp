#include "stm32/gpio/digital_output.hpp"

namespace gpio
{

DigitalOutput::DigitalOutput(Pin pin, DigitalOutputConfig config) : pin_(pin)
{
	pin_.enableClock();

	detail::setMode(pin_, Mode::OUTPUT);
	detail::setOutputType(pin_, config.outputType);
	detail::setOutputSpeed(pin_, config.outputSpeed);
	detail::setPull(pin_, config.pull);

	write(config.initialState);
}

} // namespace gpio
