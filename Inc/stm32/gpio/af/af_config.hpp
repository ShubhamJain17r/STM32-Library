#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/gpio_helper.hpp"

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
inline void configure(const Pin& pin, const AlternatePinConfig& config) noexcept
{
	pin.enableClock();

	setAlternateFunction(pin, Traits<signal>::af);
	helper::setOutputType(pin, config.outputType);
	helper::setOutputSpeed(pin, config.outputSpeed);
	helper::setPull(pin, config.pull);
	helper::setMode(pin, Mode::ALTERNATE);
}

} // namespace gpio::af
