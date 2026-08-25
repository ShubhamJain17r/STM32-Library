#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/common/rcc.hpp"
#include "stm32/common/rcc_enable.hpp"

#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/gpio/gpio_helper.hpp"
#include "stm32/gpio/af/af_helper.hpp"

#include "stm32/timer/timer_types.hpp"
#include "stm32/timer/timer_traits.hpp"
#include "stm32/timer/timer_af_traits.hpp"
#include "stm32/timer/timer_helper.hpp"

namespace timer
{

template<Instance I>
class PwmHandler
{
public:
    static_assert(!Traits<I>::isBasic,
        "Basic timers (TIM6/TIM7) do not support PWM channels. Use Timer6 or Timer7 for timebase only.");

    PwmHandler() = delete;
    ~PwmHandler();

    PwmHandler(const PwmHandler&) = delete;
    PwmHandler& operator=(const PwmHandler&) = delete;

    PwmHandler(PwmHandler&&) = delete;
    PwmHandler& operator=(PwmHandler&&) = delete;

    /// @brief Construct PWM at a given carrier frequency in Hz
    explicit PwmHandler(std::uint32_t frequencyHz);

    /// @brief Change the PWM carrier frequency (duty cycles scale proportionally)
    void setFrequency(std::uint32_t frequencyHz) noexcept;

    /// @brief Enable a PWM channel on a specific GPIO pin with automatic AF configuration.
    ///        Returns false if the pin is not valid for this timer/channel combination.
    bool enableChannel(Channel ch, gpio::Pin pin,
                       Polarity polarity = Polarity::ActiveHigh,
                       PwmMode mode = PwmMode::Mode1) noexcept;

    /// @brief Enable a channel in Output Compare mode with automatic AF configuration.
    bool enableOutputCompare(Channel ch, gpio::Pin pin,
                             OutputCompareMode mode,
                             Polarity polarity = Polarity::ActiveHigh) noexcept;

    /// @brief Disable a PWM/OC channel output
    void disableChannel(Channel ch) noexcept;

    /// @brief Set duty cycle as a percentage [0.0, 100.0]
    void setDutyCycle(Channel ch, float percentage) noexcept;

    /// @brief Set duty cycle in permille [0, 1000] for 0.1% resolution
    void setDutyPermille(Channel ch, std::uint16_t permille) noexcept;

    /// @brief Write the CCR register directly
    void setRawCompare(Channel ch, std::uint32_t ccrValue) noexcept;

    std::uint32_t getRawCompare(Channel ch) const noexcept;
    std::uint32_t getAutoReload() const noexcept;

    void start() noexcept;
    void stop() noexcept;

private:
    void initClock() noexcept;
    void calculatePscArr(std::uint32_t targetFreqHz, std::uint32_t& pscOut, std::uint32_t& arrOut) const noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

template<Instance I>
void PwmHandler<I>::initClock() noexcept
{
    rcc::enablePeripheralClock(Traits<I>::peripheral());
}

template<Instance I>
PwmHandler<I>::PwmHandler(std::uint32_t frequencyHz)
{
    initClock();
    setFrequency(frequencyHz);
    helper::enableAutoReloadPreload(Traits<I>::peripheral());
}

template<Instance I>
PwmHandler<I>::~PwmHandler()
{
    stop();
}

template<Instance I>
void PwmHandler<I>::calculatePscArr(std::uint32_t targetFreqHz, std::uint32_t& pscOut, std::uint32_t& arrOut) const noexcept
{
    if(targetFreqHz == 0u) targetFreqHz = 1u;

    const std::uint32_t timClk = rcc::getTimerFrequency(Traits<I>::bus);

    // Target high ARR resolution (ideally ARR ~ 1000..65535) for smooth duty cycle steps
    std::uint32_t totalDiv = (timClk + (targetFreqHz / 2u)) / targetFreqHz;
    if(totalDiv == 0u) totalDiv = 1u;

    std::uint32_t psc = (totalDiv / 65536U);
    if(psc > 65535U) psc = 65535U;

    std::uint32_t arr = (totalDiv / (psc + 1U));
    if(arr > 0u) arr -= 1U;
    if(arr > 65535U && !Traits<I>::is32Bit) arr = 65535U;

    pscOut = psc;
    arrOut = arr;
}

template<Instance I>
void PwmHandler<I>::setFrequency(std::uint32_t frequencyHz) noexcept
{
    std::uint32_t psc = 0u;
    std::uint32_t arr = 0u;
    calculatePscArr(frequencyHz, psc, arr);

    helper::setPrescaler(Traits<I>::peripheral(), psc);
    helper::setAutoReload(Traits<I>::peripheral(), arr);
    helper::generateUpdate(Traits<I>::peripheral());
}

template<Instance I>
bool PwmHandler<I>::enableChannel(Channel ch, gpio::Pin pin, Polarity polarity, PwmMode mode) noexcept
{
    return enableOutputCompare(ch, pin, static_cast<OutputCompareMode>(mode), polarity);
}

template<Instance I>
bool PwmHandler<I>::enableOutputCompare(Channel ch, gpio::Pin pin, OutputCompareMode mode, Polarity polarity) noexcept
{
    // Guard: channel must exist on this timer
    if(static_cast<std::uint8_t>(ch) > Traits<I>::channelCount)
    {
        return false;
    }

    // 1. Validate pin and retrieve Alternate Function
    gpio::af::AlternateFunction afVal = gpio::af::AlternateFunction::INVALID;

    switch(ch)
    {
        case Channel::CH1: afVal = af::alternateFunction<I, Channel::CH1>(pin); break;
        case Channel::CH2: afVal = af::alternateFunction<I, Channel::CH2>(pin); break;
        case Channel::CH3: afVal = af::alternateFunction<I, Channel::CH3>(pin); break;
        case Channel::CH4: afVal = af::alternateFunction<I, Channel::CH4>(pin); break;
    }

    if(afVal == gpio::af::AlternateFunction::INVALID)
    {
        return false;
    }

    // 2. Configure GPIO: clock, alternate function, output type, speed, pull
    pin.enableClock();
    gpio::af::setAlternateFunction(pin, afVal);
    gpio::helper::setOutputType(pin, gpio::OutputType::PUSH_PULL);
    gpio::helper::setOutputSpeed(pin, gpio::OutputSpeed::HIGH);
    gpio::helper::setPull(pin, gpio::Pull::NONE);
    gpio::helper::setMode(pin, gpio::Mode::ALTERNATE);

    // 3. Configure timer channel for Output Compare / PWM
    helper::configureOutputCompare(Traits<I>::peripheral(), ch, mode, polarity);
    helper::enableChannelOutput(Traits<I>::peripheral(), ch);

    // 4. Advanced timers (TIM1/TIM8) require MOE to be set in BDTR
    if constexpr(Traits<I>::isAdvanced)
    {
        helper::enableMainOutput(Traits<I>::peripheral());
    }

    return true;
}

template<Instance I>
void PwmHandler<I>::disableChannel(Channel ch) noexcept
{
    helper::disableChannelOutput(Traits<I>::peripheral(), ch);
}

template<Instance I>
void PwmHandler<I>::setDutyCycle(Channel ch, float percentage) noexcept
{
    if(percentage < 0.0f)   percentage = 0.0f;
    if(percentage > 100.0f) percentage = 100.0f;

    const std::uint32_t arr = helper::getAutoReload(Traits<I>::peripheral());
    const std::uint32_t ccr = static_cast<std::uint32_t>((percentage * static_cast<float>(arr + 1U)) / 100.0f);

    helper::setCompare(Traits<I>::peripheral(), ch, ccr);
}

template<Instance I>
void PwmHandler<I>::setDutyPermille(Channel ch, std::uint16_t permille) noexcept
{
    if(permille > 1000U) permille = 1000U;

    const std::uint32_t arr = helper::getAutoReload(Traits<I>::peripheral());
    const std::uint32_t ccr = (static_cast<std::uint32_t>(permille) * (arr + 1U)) / 1000U;

    helper::setCompare(Traits<I>::peripheral(), ch, ccr);
}

template<Instance I>
void PwmHandler<I>::setRawCompare(Channel ch, std::uint32_t ccrValue) noexcept
{
    helper::setCompare(Traits<I>::peripheral(), ch, ccrValue);
}

template<Instance I>
std::uint32_t PwmHandler<I>::getRawCompare(Channel ch) const noexcept
{
    return helper::getCompare(Traits<I>::peripheral(), ch);
}

template<Instance I>
std::uint32_t PwmHandler<I>::getAutoReload() const noexcept
{
    return helper::getAutoReload(Traits<I>::peripheral());
}

template<Instance I>
void PwmHandler<I>::start() noexcept
{
    helper::enableCounter(Traits<I>::peripheral());
}

template<Instance I>
void PwmHandler<I>::stop() noexcept
{
    helper::disableCounter(Traits<I>::peripheral());
}

// ---------------------------------------------------------------------------
// Convenient Type Aliases (no Pwm6/Pwm7 — basic timers have no channels)
// ---------------------------------------------------------------------------
using Pwm1  = PwmHandler<Instance::tim1>;
using Pwm2  = PwmHandler<Instance::tim2>;
using Pwm3  = PwmHandler<Instance::tim3>;
using Pwm4  = PwmHandler<Instance::tim4>;
using Pwm5  = PwmHandler<Instance::tim5>;
using Pwm8  = PwmHandler<Instance::tim8>;
using Pwm9  = PwmHandler<Instance::tim9>;
using Pwm10 = PwmHandler<Instance::tim10>;
using Pwm11 = PwmHandler<Instance::tim11>;
using Pwm12 = PwmHandler<Instance::tim12>;
using Pwm13 = PwmHandler<Instance::tim13>;
using Pwm14 = PwmHandler<Instance::tim14>;

} // namespace timer
