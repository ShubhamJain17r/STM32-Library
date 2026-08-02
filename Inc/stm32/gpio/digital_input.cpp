#include "stm32/gpio/digital_input.hpp"
#include "stm32/gpio/gpio_helper.hpp"

namespace gpio
{

DigitalInput::DigitalInput(Pin pin, Pull pull) : pin_(pin)
{
	pin_.enableClock();

	helper::setMode(pin_, Mode::INPUT);
	helper::setPull(pin_, pull);
}

} // namespace gpio
