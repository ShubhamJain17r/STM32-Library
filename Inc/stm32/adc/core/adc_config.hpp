/**
 * @file adc_config.hpp
 * @brief Configuration structures and default settings for ADC peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/adc/core/adc_types.hpp"
#include "stm32/adc/core/adc_traits.hpp"

namespace adc
{

template<Instance I>
struct AdcConfig
{
    Resolution   resolution          = Resolution::Bits12;
    SamplingTime defaultSamplingTime = SamplingTime::Cycles84;
    Alignment    alignment           = Alignment::Right;
    Prescaler    prescaler           = Prescaler::Div4; // 22.5 MHz at 90 MHz APB2
    bool         continuous          = false;
    bool         scanMode            = false;
    Trigger      trigger             = Trigger::Software;

    constexpr AdcConfig() = default;

    constexpr explicit AdcConfig(Resolution res, SamplingTime smp = SamplingTime::Cycles84)
        : resolution(res), defaultSamplingTime(smp)
    {}
};

} // namespace adc

