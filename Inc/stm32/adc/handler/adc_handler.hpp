/**
 * @file adc_handler.hpp
 * @brief High-level ADC driver with single/scan conversions, internal sensors, and watchdog
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <cstddef>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/common/systick/systick.hpp"

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"

#include "stm32/adc/core/adc_types.hpp"
#include "stm32/adc/core/adc_traits.hpp"
#include "stm32/adc/core/adc_pin_traits.hpp"
#include "stm32/adc/core/adc_config.hpp"
#include "stm32/adc/core/adc_helper.hpp"
#include "stm32/adc/interrupt/adc_interrupt.hpp"

namespace adc
{

template<Instance I>
class AdcHandler
{
public:
    ~AdcHandler();

    AdcHandler(const AdcHandler&) = delete;
    AdcHandler& operator=(const AdcHandler&) = delete;

    AdcHandler(AdcHandler&&) = delete;
    AdcHandler& operator=(AdcHandler&&) = delete;

    explicit AdcHandler(Resolution res = Resolution::Bits12, SamplingTime smp = SamplingTime::Cycles84);
    explicit AdcHandler(const AdcConfig<I>& config);

    // Channel Enable & Configuration
    bool enableChannel(Channel ch, SamplingTime smp = SamplingTime::Cycles84) noexcept;
    bool enableChannel(gpio::Pin pin, SamplingTime smp = SamplingTime::Cycles84) noexcept;

    // Single Channel Polling
    std::uint16_t readRaw(Channel ch) noexcept;
    std::uint16_t readRaw(gpio::Pin pin) noexcept;

    float readVoltage(Channel ch, float vref = 3.3f) noexcept;
    float readVoltage(gpio::Pin pin, float vref = 3.3f) noexcept;

    // Internal Sensor Channels
    float readInternalTemperature(float vref = 3.3f) noexcept;
    float readVrefInternal(float vref = 3.3f) noexcept;
    float readVbat(float vref = 3.3f) noexcept;

    // Multi-Channel Sequence / Scan Mode
    void configureSequence(const Channel* channels, std::size_t count) noexcept;
    void readSequence(std::uint16_t* buffer, std::size_t count) noexcept;

    // Hardware Analog Watchdog
    void enableAnalogWatchdog(Channel ch, std::uint16_t lowThreshold, std::uint16_t highThreshold, stm32::Callback callback = nullptr) noexcept;
    void disableAnalogWatchdog() noexcept;

    // Callbacks & Interrupts
    void attachConversionCompleteCallback(stm32::Callback cb) noexcept;
    void attachWatchdogCallback(stm32::Callback cb) noexcept;

    void enable() noexcept;
    void disable() noexcept;
    void start() noexcept;

    float getMaxCounts() const noexcept;

private:
    AdcConfig<I> config_;

    void configureHardware() noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

template<Instance I>
void AdcHandler<I>::configureHardware() noexcept
{
    // 1. Enable peripheral clock
    Traits<I>::enableClock();

    auto* adc = Traits<I>::peripheral();
    auto* common = Traits<I>::common();

    // 2. Set Common Prescaler
    helper::setPrescaler(common, config_.prescaler);

    // 3. Set Resolution & Data Alignment
    helper::setResolution(adc, config_.resolution);
    helper::setDataAlignment(adc, config_.alignment);

    // 4. Set Scan & Continuous Modes
    helper::setScanMode(adc, config_.scanMode);
    helper::setContinuousMode(adc, config_.continuous);

    // 5. Enable ADC
    helper::enable(adc);
}

template<Instance I>
AdcHandler<I>::AdcHandler(Resolution res, SamplingTime smp)
    : AdcHandler(AdcConfig<I>{res, smp})
{}

template<Instance I>
AdcHandler<I>::AdcHandler(const AdcConfig<I>& config)
    : config_(config)
{
    configureHardware();
}

template<Instance I>
AdcHandler<I>::~AdcHandler()
{
    disable();
}

template<Instance I>
void AdcHandler<I>::enable() noexcept
{
    helper::enable(Traits<I>::peripheral());
}

template<Instance I>
void AdcHandler<I>::disable() noexcept
{
    helper::disable(Traits<I>::peripheral());
}

template<Instance I>
void AdcHandler<I>::start() noexcept
{
    helper::startConversion(Traits<I>::peripheral());
}

template<Instance I>
float AdcHandler<I>::getMaxCounts() const noexcept
{
    switch(config_.resolution)
    {
        case Resolution::Bits12: return 4095.0f;
        case Resolution::Bits10: return 1023.0f;
        case Resolution::Bits8:  return 255.0f;
        case Resolution::Bits6:  return 63.0f;
    }
    return 4095.0f;
}

template<Instance I>
bool AdcHandler<I>::enableChannel(Channel ch, SamplingTime smp) noexcept
{
    auto pinOpt = pin_traits::getPin<I>(ch);
    if(pinOpt.has_value())
    {
        auto pin = *pinOpt;
        pin.enableClock();
        gpio::helper::setMode(pin, gpio::Mode::ANALOG);
        gpio::helper::setPull(pin, gpio::Pull::NONE);
    }
    else if(ch == Channel::Temperature || ch == Channel::VrefInt)
    {
        helper::enableTemperatureAndVref(Traits<I>::common());
    }
    else if(ch == Channel::Vbat)
    {
        helper::enableVbat(Traits<I>::common());
    }

    helper::setChannelSamplingTime(Traits<I>::peripheral(), ch, smp);
    return true;
}

template<Instance I>
bool AdcHandler<I>::enableChannel(gpio::Pin pin, SamplingTime smp) noexcept
{
    auto chOpt = pin_traits::getChannel<I>(pin);
    if(!chOpt.has_value()) return false;

    pin.enableClock();
    gpio::helper::setMode(pin, gpio::Mode::ANALOG);
    gpio::helper::setPull(pin, gpio::Pull::NONE);

    helper::setChannelSamplingTime(Traits<I>::peripheral(), *chOpt, smp);
    return true;
}

template<Instance I>
std::uint16_t AdcHandler<I>::readRaw(Channel ch) noexcept
{
    auto* adc = Traits<I>::peripheral();

    // Configure single conversion on channel
    helper::setSequenceLength(adc, 1);
    helper::setSequenceChannel(adc, 1, ch);

    // Clear any previous EOC
    helper::clearConversionFlag(adc);

    // Start conversion
    helper::startConversion(adc);

    // Wait for EOC with timeout
    systick::Timeout to(20);
    while(!helper::isConversionComplete(adc))
    {
        if(to.expired()) return 0;
    }

    helper::clearConversionFlag(adc);
    return helper::readData(adc);
}

template<Instance I>
std::uint16_t AdcHandler<I>::readRaw(gpio::Pin pin) noexcept
{
    auto chOpt = pin_traits::getChannel<I>(pin);
    if(!chOpt.has_value()) return 0;
    return readRaw(*chOpt);
}

template<Instance I>
float AdcHandler<I>::readVoltage(Channel ch, float vref) noexcept
{
    const std::uint16_t raw = readRaw(ch);
    return (static_cast<float>(raw) * vref) / getMaxCounts();
}

template<Instance I>
float AdcHandler<I>::readVoltage(gpio::Pin pin, float vref) noexcept
{
    const std::uint16_t raw = readRaw(pin);
    return (static_cast<float>(raw) * vref) / getMaxCounts();
}

template<Instance I>
float AdcHandler<I>::readInternalTemperature(float vref) noexcept
{
    if constexpr(I != Instance::adc1)
    {
        return 0.0f; // Internal temperature sensor is only connected to ADC1
    }

    helper::enableTemperatureAndVref(Traits<I>::common());
    helper::setChannelSamplingTime(Traits<I>::peripheral(), Channel::Temperature, SamplingTime::Cycles480);

    // Required sensor warmup delay
    systick::delayUs(20);

    const float vSense = readVoltage(Channel::Temperature, vref);

    // STM32F446 Datasheet Typical Parameters:
    // V25 = 0.76 V, Avg_Slope = 2.5 mV/°C = 0.0025 V/°C
    constexpr float V25 = 0.76f;
    constexpr float AVG_SLOPE = 0.0025f;

    return ((vSense - V25) / AVG_SLOPE) + 25.0f;
}

template<Instance I>
float AdcHandler<I>::readVrefInternal(float vref) noexcept
{
    if constexpr(I != Instance::adc1)
    {
        return 0.0f;
    }

    helper::enableTemperatureAndVref(Traits<I>::common());
    helper::setChannelSamplingTime(Traits<I>::peripheral(), Channel::VrefInt, SamplingTime::Cycles480);
    systick::delayUs(20);

    return readVoltage(Channel::VrefInt, vref);
}

template<Instance I>
float AdcHandler<I>::readVbat(float vref) noexcept
{
    if constexpr(I != Instance::adc1)
    {
        return 0.0f;
    }

    helper::enableVbat(Traits<I>::common());
    helper::setChannelSamplingTime(Traits<I>::peripheral(), Channel::Vbat, SamplingTime::Cycles480);
    systick::delayUs(20);

    // Internal divider divides VBAT by 4
    return readVoltage(Channel::Vbat, vref) * 4.0f;
}

template<Instance I>
void AdcHandler<I>::configureSequence(const Channel* channels, std::size_t count) noexcept
{
    if(!channels || count == 0) return;
    if(count > 16) count = 16;

    auto* adc = Traits<I>::peripheral();

    helper::setScanMode(adc, true);
    helper::setSequenceLength(adc, static_cast<std::uint8_t>(count));

    for(std::size_t i = 0; i < count; ++i)
    {
        enableChannel(channels[i]);
        helper::setSequenceChannel(adc, static_cast<std::uint8_t>(i + 1), channels[i]);
    }
}

template<Instance I>
void AdcHandler<I>::readSequence(std::uint16_t* buffer, std::size_t count) noexcept
{
    if(!buffer || count == 0) return;

    auto* adc = Traits<I>::peripheral();

    for(std::size_t i = 0; i < count; ++i)
    {
        helper::clearConversionFlag(adc);
        helper::startConversion(adc);

        systick::Timeout to(20);
        while(!helper::isConversionComplete(adc))
        {
            if(to.expired())
            {
                buffer[i] = 0;
                break;
            }
        }
        buffer[i] = helper::readData(adc);
    }
}

template<Instance I>
void AdcHandler<I>::enableAnalogWatchdog(Channel ch, std::uint16_t lowThreshold, std::uint16_t highThreshold, stm32::Callback callback) noexcept
{
    auto* adc = Traits<I>::peripheral();

    helper::setWatchdogThresholds(adc, lowThreshold, highThreshold);
    helper::enableWatchdog(adc, ch);

    if(callback)
    {
        attachWatchdogCallback(callback);
        helper::enableWatchdogInterrupt(adc);
        interrupt::enableIRQ(Traits<I>::irq);
    }
}

template<Instance I>
void AdcHandler<I>::disableAnalogWatchdog() noexcept
{
    auto* adc = Traits<I>::peripheral();
    helper::disableWatchdogInterrupt(adc);
    helper::disableWatchdog(adc);
}

template<Instance I>
void AdcHandler<I>::attachConversionCompleteCallback(stm32::Callback cb) noexcept
{
    interrupt::AdcEvent::setUserCallback(I, interrupt::Event::ConversionComplete, cb);
    helper::enableEocInterrupt(Traits<I>::peripheral());
    interrupt::enableIRQ(Traits<I>::irq);
}

template<Instance I>
void AdcHandler<I>::attachWatchdogCallback(stm32::Callback cb) noexcept
{
    interrupt::AdcEvent::setUserCallback(I, interrupt::Event::Watchdog, cb);
}

} // namespace adc

