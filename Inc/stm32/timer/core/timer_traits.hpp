/**
 * @file timer_traits.hpp
 * @brief Hardware traits and IRQ mappings for all 14 timers on STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc.hpp"
#include "stm32/timer/core/timer_types.hpp"

namespace timer
{

template<Instance I>
struct Traits;

template<>
struct Traits<Instance::tim1>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM1; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = true;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 4;
    static constexpr IRQn_Type irq = TIM1_UP_TIM10_IRQn;
    static constexpr IRQn_Type ccIrq = TIM1_CC_IRQn;
};

template<>
struct Traits<Instance::tim2>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM2; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = true;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 4;
    static constexpr IRQn_Type irq = TIM2_IRQn;
    static constexpr IRQn_Type ccIrq = TIM2_IRQn;
};

template<>
struct Traits<Instance::tim3>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM3; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 4;
    static constexpr IRQn_Type irq = TIM3_IRQn;
    static constexpr IRQn_Type ccIrq = TIM3_IRQn;
};

template<>
struct Traits<Instance::tim4>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM4; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 4;
    static constexpr IRQn_Type irq = TIM4_IRQn;
    static constexpr IRQn_Type ccIrq = TIM4_IRQn;
};

template<>
struct Traits<Instance::tim5>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM5; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = true;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 4;
    static constexpr IRQn_Type irq = TIM5_IRQn;
    static constexpr IRQn_Type ccIrq = TIM5_IRQn;
};

template<>
struct Traits<Instance::tim6>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM6; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = true;
    static constexpr std::uint8_t channelCount = 0;
    static constexpr IRQn_Type irq = TIM6_DAC_IRQn;
    static constexpr IRQn_Type ccIrq = TIM6_DAC_IRQn;
};

template<>
struct Traits<Instance::tim7>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM7; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = true;
    static constexpr std::uint8_t channelCount = 0;
    static constexpr IRQn_Type irq = TIM7_IRQn;
    static constexpr IRQn_Type ccIrq = TIM7_IRQn;
};

template<>
struct Traits<Instance::tim8>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM8; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = true;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 4;
    static constexpr IRQn_Type irq = TIM8_UP_TIM13_IRQn;
    static constexpr IRQn_Type ccIrq = TIM8_CC_IRQn;
};

template<>
struct Traits<Instance::tim9>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM9; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 2;
    static constexpr IRQn_Type irq = TIM1_BRK_TIM9_IRQn;
    static constexpr IRQn_Type ccIrq = TIM1_BRK_TIM9_IRQn;
};

template<>
struct Traits<Instance::tim10>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM10; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 1;
    static constexpr IRQn_Type irq = TIM1_UP_TIM10_IRQn;
    static constexpr IRQn_Type ccIrq = TIM1_UP_TIM10_IRQn;
};

template<>
struct Traits<Instance::tim11>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM11; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 1;
    static constexpr IRQn_Type irq = TIM1_TRG_COM_TIM11_IRQn;
    static constexpr IRQn_Type ccIrq = TIM1_TRG_COM_TIM11_IRQn;
};

template<>
struct Traits<Instance::tim12>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM12; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 2;
    static constexpr IRQn_Type irq = TIM8_BRK_TIM12_IRQn;
    static constexpr IRQn_Type ccIrq = TIM8_BRK_TIM12_IRQn;
};

template<>
struct Traits<Instance::tim13>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM13; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 1;
    static constexpr IRQn_Type irq = TIM8_UP_TIM13_IRQn;
    static constexpr IRQn_Type ccIrq = TIM8_UP_TIM13_IRQn;
};

template<>
struct Traits<Instance::tim14>
{
    static constexpr TIM_TypeDef* peripheral() noexcept { return TIM14; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr bool is32Bit = false;
    static constexpr bool isAdvanced = false;
    static constexpr bool isBasic = false;
    static constexpr std::uint8_t channelCount = 1;
    static constexpr IRQn_Type irq = TIM8_TRG_COM_TIM14_IRQn;
    static constexpr IRQn_Type ccIrq = TIM8_TRG_COM_TIM14_IRQn;
};

} // namespace timer

