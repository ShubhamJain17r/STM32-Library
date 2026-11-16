/**
 * @file one_pulse.hpp
 * @brief High-level One-Pulse Mode (OPM) hardware driver for deterministic pulse generation
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"
#include "stm32/gpio/af/af_helper.hpp"

#include "stm32/timer/core/timer_types.hpp"
#include "stm32/timer/core/timer_traits.hpp"
#include "stm32/timer/core/timer_af_traits.hpp"
#include "stm32/timer/core/timer_helper.hpp"

namespace timer
{

template<Instance I>
class OnePulseHandler
{
public:
    static_assert(!Traits<I>::isBasic,
        "Basic timers (TIM6/TIM7) do not support One-Pulse Mode channels.");

    OnePulseHandler() noexcept
    {
        initClock();
        auto* tim = Traits<I>::peripheral();
        const std::uint32_t timerClk = rcc::getTimerFrequency(Traits<I>::bus);
        const std::uint32_t psc = (timerClk >= 1'000'000u) ? ((timerClk / 1'000'000u) - 1u) : 0u;
        helper::setPrescaler(tim, psc);
        helper::setOnePulseMode(tim, true);
    }

    ~OnePulseHandler()
    {
        helper::disableCounter(Traits<I>::peripheral());
    }

    OnePulseHandler(const OnePulseHandler&) = delete;
    OnePulseHandler& operator=(const OnePulseHandler&) = delete;
    OnePulseHandler(OnePulseHandler&&) = delete;
    OnePulseHandler& operator=(OnePulseHandler&&) = delete;

    bool enableChannel(Channel ch, gpio::Pin pin, Polarity polarity = Polarity::ActiveHigh) noexcept
    {
        if(static_cast<std::uint8_t>(ch) > Traits<I>::channelCount)
        {
            return false;
        }

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

        pin.enableClock();
        gpio::af::setAlternateFunction(pin, afVal);
        gpio::helper::setOutputType(pin, gpio::OutputType::PUSH_PULL);
        gpio::helper::setOutputSpeed(pin, gpio::OutputSpeed::HIGH);
        gpio::helper::setPull(pin, gpio::Pull::NONE);
        gpio::helper::setMode(pin, gpio::Mode::ALTERNATE);

        auto* tim = Traits<I>::peripheral();
        helper::configureOutputCompare(tim, ch, OutputCompareMode::Pwm2, polarity);
        helper::enableChannelOutput(tim, ch);

        if constexpr(Traits<I>::isAdvanced)
        {
            helper::enableMainOutput(tim);
        }

        return true;
    }

    void triggerPulse(Channel ch, std::uint32_t delayUs, std::uint32_t widthUs) noexcept
    {
        auto* tim = Traits<I>::peripheral();
        helper::disableCounter(tim);
        helper::setCompare(tim, ch, delayUs);
        helper::setAutoReload(tim, delayUs + widthUs);
        helper::setCounter(tim, 0);
        helper::generateUpdate(tim);
        helper::clearUpdateFlag(tim);
        helper::enableCounter(tim);
    }

private:
    void initClock() noexcept
    {
        rcc::enablePeripheralClock(Traits<I>::peripheral());
    }
};

using Pulse1  = OnePulseHandler<Instance::tim1>;
using Pulse2  = OnePulseHandler<Instance::tim2>;
using Pulse3  = OnePulseHandler<Instance::tim3>;
using Pulse4  = OnePulseHandler<Instance::tim4>;
using Pulse5  = OnePulseHandler<Instance::tim5>;
using Pulse8  = OnePulseHandler<Instance::tim8>;
using Pulse9  = OnePulseHandler<Instance::tim9>;
using Pulse10 = OnePulseHandler<Instance::tim10>;
using Pulse11 = OnePulseHandler<Instance::tim11>;
using Pulse12 = OnePulseHandler<Instance::tim12>;
using Pulse13 = OnePulseHandler<Instance::tim13>;
using Pulse14 = OnePulseHandler<Instance::tim14>;

} // namespace timer
