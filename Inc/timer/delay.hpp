#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace timer
{

void initSysTick(void);

void delay_ms(std::uint32_t);

}
