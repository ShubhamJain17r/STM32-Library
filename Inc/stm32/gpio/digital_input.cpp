#include "stm32/gpio/digital_input.hpp"

namespace gpio
{

DigitalInput::DigitalInput(Pin pin, Pull pull) : pin_(pin)
{
	pin_.enableClock();

	setModeInput();
	configurePull(pull);
}

} // namespace gpio
