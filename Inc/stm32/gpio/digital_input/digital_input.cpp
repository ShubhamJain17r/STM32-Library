/**
 * @file digital_input.cpp
 * @brief Implementation of DigitalInput initialization
 */

#include "stm32/gpio/digital_input/digital_input.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"

namespace gpio
{

DigitalInput::DigitalInput(Pin pin, Pull pull) : pin_(pin)
{
	pin_.enableClock();

	helper::setPull(pin_, pull);
	helper::setMode(pin_, Mode::INPUT);
}

} // namespace gpio
