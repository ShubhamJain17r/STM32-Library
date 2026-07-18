#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include "common/registers.hpp"

namespace rcc {

constexpr std::uint32_t SYSCLK = 16000000;
constexpr std::uint8_t SYSCLK_MHz = 16;

void enableClock_GPIO(const GPIO_TypeDef* port);

inline void enableClock_SYSCFG() {
    reg::setBit(RCC->APB2ENR, 14);
}

} // namespace rcc
