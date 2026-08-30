/**
 * @file adc_interrupt.hpp
 * @brief Interrupt event and watchdog dispatch layer for ADC peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/callback/callback.hpp"
#include "stm32/adc/core/adc_types.hpp"
#include "stm32/adc/core/adc_traits.hpp"

namespace adc::interrupt
{

enum class Event : std::uint8_t
{
    ConversionComplete,
    Watchdog,
    Overrun
};

struct AdcCallbacks
{
    stm32::Callback conversionComplete = nullptr;
    stm32::Callback watchdog           = nullptr;
    stm32::Callback overrun            = nullptr;
};

class AdcEvent
{
public:
    static void setDeveloperCallback(Instance I, Event ev, stm32::Callback cb) noexcept;
    static void setUserCallback(Instance I, Event ev, stm32::Callback cb) noexcept;

    static void handleInterrupt(Instance I) noexcept;

private:
    inline static AdcCallbacks developerCallbacks_[3];
    inline static AdcCallbacks userCallbacks_[3];
};

inline void enableIRQ(IRQn_Type irq = ADC_IRQn) noexcept
{
    NVIC_EnableIRQ(irq);
}

inline void disableIRQ(IRQn_Type irq = ADC_IRQn) noexcept
{
    NVIC_DisableIRQ(irq);
}

} // namespace adc::interrupt

