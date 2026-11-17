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
#include "stm32/dma/core/dma_types.hpp"

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

template<Instance I>
struct DmaTraits;

template<>
struct DmaTraits<Instance::adc1>
{
    static constexpr dma::Controller controller = dma::Controller::Dma2;
    static constexpr dma::StreamId   stream     = dma::StreamId::Stream0;
    static constexpr dma::ChannelId  channel    = dma::ChannelId::Channel0;
};

template<>
struct DmaTraits<Instance::adc2>
{
    static constexpr dma::Controller controller = dma::Controller::Dma2;
    static constexpr dma::StreamId   stream     = dma::StreamId::Stream2;
    static constexpr dma::ChannelId  channel    = dma::ChannelId::Channel1;
};

template<>
struct DmaTraits<Instance::adc3>
{
    static constexpr dma::Controller controller = dma::Controller::Dma2;
    static constexpr dma::StreamId   stream     = dma::StreamId::Stream1;
    static constexpr dma::ChannelId  channel    = dma::ChannelId::Channel2;
};

} // namespace adc

