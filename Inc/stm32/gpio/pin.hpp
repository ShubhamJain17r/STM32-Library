#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace gpio
{

struct Pin
{
	GPIO_TypeDef* port;
	std::uint8_t pin;
};

constexpr Pin PA0{GPIOA, 0};

} // namespace gpio
