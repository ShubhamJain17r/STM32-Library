#include "stm32/gpio/digital_output.hpp"
#include "stm32/gpio/gpio_helper.hpp"

namespace gpio
{

DigitalOutput::DigitalOutput(Pin pin, DigitalOutputConfig config) : pin_(pin)
{
	pin_.enableClock();

	write(config.initialState);
	helper::setOutputType(pin_, config.outputType);
	helper::setOutputSpeed(pin_, config.outputSpeed);
	helper::setPull(pin_, config.pull);
	helper::setMode(pin_, Mode::OUTPUT);
}

} // namespace gpio
