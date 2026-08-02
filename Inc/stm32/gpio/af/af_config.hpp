#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/gpio/gpio_helper.hpp"

#include "stm32/gpio/af/af_types.hpp"
#include "stm32/gpio/af/af_helper.hpp"

namespace gpio::af
{

struct AlternatePinConfig
{
	OutputType outputType = OutputType::PUSH_PULL;
	OutputSpeed outputSpeed = OutputSpeed::LOW;
	Pull pull = Pull::NONE;
};

template<Signal signal>
void configure(const Pin& pin, const AlternatePinConfig& config)
{
	pin.enableClock();

	helper::setMode(pin, Mode::ALTERNATE);
	helper::setOutputType(pin, config.outputType);
	helper::setOutputSpeed(pin, config.outputSpeed);
	helper::setPull(pin, config.pull);

	setAlternateFunction(pin, Traits<signal>::af);
}

} // namespace gpio::af
