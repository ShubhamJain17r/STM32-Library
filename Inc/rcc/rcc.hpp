#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace rcc
{

// temporarily using sysclk variable as 16MHz. Later using function to determine sysclk and bus clocks
const std::uint32_t SYSCLK = 16000000;
const std::uint32_t SYSCLK_MHz = 16;

void enableClock_GPIO(GPIO_TypeDef*);

}
