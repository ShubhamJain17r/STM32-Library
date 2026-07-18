#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace timer
{

extern volatile uint32_t tick_count;

uint32_t getTickCount();

void initSysTick();

void delay_ms(std::uint32_t);

}
