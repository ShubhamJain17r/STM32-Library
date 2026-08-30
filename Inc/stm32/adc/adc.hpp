/**
 * @file adc.hpp
 * @brief Master umbrella header for STM32 ADC Subsystem
 */

#pragma once

#include "stm32/adc/core/adc_types.hpp"
#include "stm32/adc/core/adc_traits.hpp"
#include "stm32/adc/core/adc_pin_traits.hpp"
#include "stm32/adc/core/adc_config.hpp"
#include "stm32/adc/core/adc_helper.hpp"
#include "stm32/adc/interrupt/adc_interrupt.hpp"
#include "stm32/adc/handler/adc_handler.hpp"

namespace adc
{

// ---------------------------------------------------------------------------
// Convenient Type Aliases
// ---------------------------------------------------------------------------
using Adc1 = AdcHandler<Instance::adc1>;
using Adc2 = AdcHandler<Instance::adc2>;
using Adc3 = AdcHandler<Instance::adc3>;

using Adc1Config = AdcConfig<Instance::adc1>;
using Adc2Config = AdcConfig<Instance::adc2>;
using Adc3Config = AdcConfig<Instance::adc3>;

} // namespace adc

