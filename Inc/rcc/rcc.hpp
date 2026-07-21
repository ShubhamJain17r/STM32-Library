#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include "common/registers.hpp"

namespace rcc {

constexpr std::uint32_t SYSCLK = 16000000;
constexpr std::uint8_t SYSCLK_MHz = 16;

void enableGpioClock(const GPIO_TypeDef* port);
void enableDmaClock(const DMA_TypeDef* dmaBase);
void enableI2cClock(const I2C_TypeDef* i2cBase);

inline void enableSyscfgClock() {
    reg::setBit(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN_Pos);
}

} // namespace rcc
