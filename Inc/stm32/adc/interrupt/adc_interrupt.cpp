/**
 * @file adc_interrupt.cpp
 * @brief Implementation of ADC interrupt event and watchdog dispatcher
 */

#include "stm32/common/registers/registers.hpp"
#include "stm32/adc/core/adc_helper.hpp"
#include "stm32/adc/interrupt/adc_interrupt.hpp"

namespace adc::interrupt
{

static constexpr std::size_t index(Instance I) noexcept
{
    return static_cast<std::size_t>(I);
}

static ADC_TypeDef* getPeripheral(Instance I) noexcept
{
    switch(I)
    {
        case Instance::adc1: return ADC1;
        case Instance::adc2: return ADC2;
        case Instance::adc3: return ADC3;
    }
    return nullptr;
}

void AdcEvent::setDeveloperCallback(Instance I, Event ev, stm32::Callback cb) noexcept
{
    switch(ev)
    {
        case Event::ConversionComplete:
            developerCallbacks_[index(I)].conversionComplete = cb;
            break;
        case Event::Watchdog:
            developerCallbacks_[index(I)].watchdog = cb;
            break;
        case Event::Overrun:
            developerCallbacks_[index(I)].overrun = cb;
            break;
    }
}

void AdcEvent::setUserCallback(Instance I, Event ev, stm32::Callback cb) noexcept
{
    switch(ev)
    {
        case Event::ConversionComplete:
            userCallbacks_[index(I)].conversionComplete = cb;
            break;
        case Event::Watchdog:
            userCallbacks_[index(I)].watchdog = cb;
            break;
        case Event::Overrun:
            userCallbacks_[index(I)].overrun = cb;
            break;
    }
}

void AdcEvent::handleInterrupt(Instance I) noexcept
{
    auto* adc = getPeripheral(I);
    if(!adc) return;

    const std::uint32_t sr  = reg::read(adc->SR);
    const std::uint32_t cr1 = reg::read(adc->CR1);

    const auto& dev  = developerCallbacks_[index(I)];
    const auto& user = userCallbacks_[index(I)];

    // 1. EOC (End of Conversion)
    if((sr & ADC_SR_EOC) && (cr1 & ADC_CR1_EOCIE))
    {
        helper::clearConversionFlag(adc);
        if(dev.conversionComplete)  dev.conversionComplete();
        if(user.conversionComplete) user.conversionComplete();
    }

    // 2. AWD (Analog Watchdog)
    if((sr & ADC_SR_AWD) && (cr1 & ADC_CR1_AWDIE))
    {
        helper::clearWatchdogFlag(adc);
        if(dev.watchdog)  dev.watchdog();
        if(user.watchdog) user.watchdog();
    }

    // 3. OVR (Overrun)
    if((sr & ADC_SR_OVR) && (cr1 & ADC_CR1_OVRIE))
    {
        helper::clearOverrunFlag(adc);
        if(dev.overrun)  dev.overrun();
        if(user.overrun) user.overrun();
    }
}

} // namespace adc::interrupt

