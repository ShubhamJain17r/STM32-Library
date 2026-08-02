#include "stm32/gpio/digital_output.hpp"
#include "stm32/gpio/gpio_helper.hpp"

namespace gpio
{

DigitalOutput::DigitalOutput(Pin pin, DigitalOutputConfig config) : pin_(pin)
{
	pin_.enableClock();

	helper::setMode(pin_, Mode::OUTPUT);
	helper::setOutputType(pin_, config.outputType);
	helper::setOutputSpeed(pin_, config.outputSpeed);
	helper::setPull(pin_, config.pull);

	write(config.initialState);
}

} // namespace gpio
