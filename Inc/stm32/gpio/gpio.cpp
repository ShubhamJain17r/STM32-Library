#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/gpio.hpp"

namespace gpio
{

DigitalOutput::DigitalOutput(Pin pin, DigitalOutputConfig config) : pin_(pin)
{

}

DigitalInput::DigitalInput(Pin pin, DigitalInputConfig config) : pin_(pin)
{

}

} // namespace gpio
