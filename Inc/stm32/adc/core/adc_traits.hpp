/**
 * @file adc_traits.hpp
 * @brief Hardware traits and IRQ mappings for all 3 ADC peripherals on STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/adc/core/adc_types.hpp"

namespace adc
{

template<Instance I>
struct Traits;

template<>
struct Traits<Instance::adc1>
{
    static constexpr ADC_TypeDef* peripheral() noexcept { return ADC1; }
    static constexpr ADC_Common_TypeDef* common() noexcept { return ADC; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr IRQn_Type irq = ADC_IRQn;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(ADC1);
    }
};

template<>
struct Traits<Instance::adc2>
{
    static constexpr ADC_TypeDef* peripheral() noexcept { return ADC2; }
    static constexpr ADC_Common_TypeDef* common() noexcept { return ADC; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr IRQn_Type irq = ADC_IRQn;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(ADC2);
    }
};

template<>
struct Traits<Instance::adc3>
{
    static constexpr ADC_TypeDef* peripheral() noexcept { return ADC3; }
    static constexpr ADC_Common_TypeDef* common() noexcept { return ADC; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr IRQn_Type irq = ADC_IRQn;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(ADC3);
    }
};

} // namespace adc

