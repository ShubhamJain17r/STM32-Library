#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include "common/registers.hpp"

namespace rcc {

constexpr std::uint32_t HSI_FREQUENCY  = 16'000'000U;
constexpr std::uint32_t SYSCLK         = HSI_FREQUENCY;
constexpr std::uint32_t HCLK           = SYSCLK;
constexpr std::uint32_t PCLK1          = HCLK;
constexpr std::uint32_t PCLK2          = HCLK;

constexpr std::uint8_t HSI_MHz         = 16U;
constexpr std::uint8_t SYSCLK_MHz      = 16U;
constexpr std::uint8_t HCLK_MHz        = 16U;
constexpr std::uint8_t PCLK1_MHz       = 16U;
constexpr std::uint8_t PCLK2_MHz       = 16U;

void enableGpioClock(const GPIO_TypeDef* port);
void enableDmaClock(const DMA_TypeDef* dmaBase);
void enableI2cClock(const I2C_TypeDef* i2cBase);

inline void enableSyscfgClock() {
    reg::setBit(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN_Pos);
}

} // namespace rcc
