#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace timer {

extern volatile uint32_t tick_count;

void initSysTick();

inline uint32_t getTickCount();

void delay_ms(std::uint32_t);

}
