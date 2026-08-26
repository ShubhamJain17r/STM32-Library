/**
 * @file timer.hpp
 * @brief High-level Timebase, delay, and periodic interrupt driver
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/common/callback/callback.hpp"

#include "stm32/timer/core/timer_types.hpp"
#include "stm32/timer/core/timer_traits.hpp"
#include "stm32/timer/core/timer_helper.hpp"
#include "stm32/timer/interrupt/timer_interrupt.hpp"

namespace timer
{

template<Instance I>
class TimerHandler
{
public:
    TimerHandler() = delete;
    ~TimerHandler();

    TimerHandler(const TimerHandler&) = delete;
    TimerHandler& operator=(const TimerHandler&) = delete;

    TimerHandler(TimerHandler&&) = delete;
    TimerHandler& operator=(TimerHandler&&) = delete;

    explicit TimerHandler(std::uint32_t frequencyHz);
    TimerHandler(std::uint32_t prescaler, std::uint32_t autoReload);

    void setFrequency(std::uint32_t frequencyHz) noexcept;
    void setPeriodUs(std::uint32_t periodUs) noexcept;
    void setPeriodMs(std::uint32_t periodMs) noexcept;

    void setPrescaler(std::uint32_t psc) noexcept;
    void setAutoReload(std::uint32_t arr) noexcept;

    void setOnePulseMode(bool enable) noexcept;
    void setCounterMode(CounterMode mode) noexcept;

    void start() noexcept;
    void stop() noexcept;
    void reset() noexcept;

    std::uint32_t counter() const noexcept;

    void setCallback(stm32::Callback callback) noexcept;
    void enableInterrupt() noexcept;
    void disableInterrupt() noexcept;

    void delayUs(std::uint32_t us) noexcept;
    void delayMs(std::uint32_t ms) noexcept;

private:
    void initClock() noexcept;
    void calculatePscArr(std::uint32_t targetFreqHz, std::uint32_t& pscOut, std::uint32_t& arrOut) const noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

template<Instance I>
void TimerHandler<I>::initClock() noexcept
{
    rcc::enablePeripheralClock(Traits<I>::peripheral());
}

template<Instance I>
TimerHandler<I>::TimerHandler(std::uint32_t frequencyHz)
{
    initClock();
    setFrequency(frequencyHz);
}

template<Instance I>
TimerHandler<I>::TimerHandler(std::uint32_t prescaler, std::uint32_t autoReload)
{
    initClock();
    setPrescaler(prescaler);
    setAutoReload(autoReload);
    helper::generateUpdate(Traits<I>::peripheral());
}

template<Instance I>
TimerHandler<I>::~TimerHandler()
{
    stop();
    disableInterrupt();
}

template<Instance I>
void TimerHandler<I>::calculatePscArr(std::uint32_t targetFreqHz, std::uint32_t& pscOut, std::uint32_t& arrOut) const noexcept
{
    if(targetFreqHz == 0u) targetFreqHz = 1u;

    const std::uint32_t timClk = rcc::getTimerFrequency(Traits<I>::bus);

    if constexpr(Traits<I>::is32Bit)
    {
        if(timClk >= 1'000'000U && targetFreqHz <= 1'000'000U)
        {
            pscOut = (timClk / 1'000'000U) - 1U;
            arrOut = (1'000'000U / targetFreqHz) - 1U;
            return;
        }
    }

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
void TimerHandler<I>::setFrequency(std::uint32_t frequencyHz) noexcept
{
    std::uint32_t psc = 0u;
    std::uint32_t arr = 0u;
    calculatePscArr(frequencyHz, psc, arr);

    helper::setPrescaler(Traits<I>::peripheral(), psc);
    helper::setAutoReload(Traits<I>::peripheral(), arr);
    helper::generateUpdate(Traits<I>::peripheral());
}

template<Instance I>
void TimerHandler<I>::setPeriodUs(std::uint32_t periodUs) noexcept
{
    if(periodUs == 0u) periodUs = 1u;

    const std::uint32_t timClk = rcc::getTimerFrequency(Traits<I>::bus);
    const std::uint32_t psc = (timClk / 1'000'000U) - 1U;

    helper::setPrescaler(Traits<I>::peripheral(), psc);
    helper::setAutoReload(Traits<I>::peripheral(), periodUs - 1U);
    helper::generateUpdate(Traits<I>::peripheral());
}

template<Instance I>
void TimerHandler<I>::setPeriodMs(std::uint32_t periodMs) noexcept
{
    setPeriodUs(periodMs * 1000U);
}

template<Instance I>
void TimerHandler<I>::setPrescaler(std::uint32_t psc) noexcept
{
    helper::setPrescaler(Traits<I>::peripheral(), psc);
}

template<Instance I>
void TimerHandler<I>::setAutoReload(std::uint32_t arr) noexcept
{
    helper::setAutoReload(Traits<I>::peripheral(), arr);
}

template<Instance I>
void TimerHandler<I>::setOnePulseMode(bool enable) noexcept
{
    helper::setOnePulseMode(Traits<I>::peripheral(), enable);
}

template<Instance I>
void TimerHandler<I>::setCounterMode(CounterMode mode) noexcept
{
    if constexpr(!Traits<I>::isBasic)
    {
        helper::setCounterMode(Traits<I>::peripheral(), mode);
    }
}

template<Instance I>
void TimerHandler<I>::start() noexcept
{
    helper::enableCounter(Traits<I>::peripheral());
}

template<Instance I>
void TimerHandler<I>::stop() noexcept
{
    helper::disableCounter(Traits<I>::peripheral());
}

template<Instance I>
void TimerHandler<I>::reset() noexcept
{
    helper::setCounter(Traits<I>::peripheral(), 0u);
}

template<Instance I>
std::uint32_t TimerHandler<I>::counter() const noexcept
{
    return helper::getCounter(Traits<I>::peripheral());
}

template<Instance I>
void TimerHandler<I>::setCallback(stm32::Callback callback) noexcept
{
    interrupt::TimerEvent::setUpdateCallback(I, callback);
}

template<Instance I>
void TimerHandler<I>::enableInterrupt() noexcept
{
    helper::clearUpdateFlag(Traits<I>::peripheral());
    helper::enableUpdateInterrupt(Traits<I>::peripheral());
    interrupt::enableIRQ(Traits<I>::irq);
}

template<Instance I>
void TimerHandler<I>::disableInterrupt() noexcept
{
    helper::disableUpdateInterrupt(Traits<I>::peripheral());
}

template<Instance I>
void TimerHandler<I>::delayUs(std::uint32_t us) noexcept
{
    setPeriodUs(us);
    reset();
    helper::clearUpdateFlag(Traits<I>::peripheral());
    setOnePulseMode(true);
    start();
    while(!helper::isUpdateFlag(Traits<I>::peripheral()));
    helper::clearUpdateFlag(Traits<I>::peripheral());
    setOnePulseMode(false);
}

template<Instance I>
void TimerHandler<I>::delayMs(std::uint32_t ms) noexcept
{
    while(ms--)
    {
        delayUs(1000u);
    }
}

using Timer1  = TimerHandler<Instance::tim1>;
using Timer2  = TimerHandler<Instance::tim2>;
using Timer3  = TimerHandler<Instance::tim3>;
using Timer4  = TimerHandler<Instance::tim4>;
using Timer5  = TimerHandler<Instance::tim5>;
using Timer6  = TimerHandler<Instance::tim6>;
using Timer7  = TimerHandler<Instance::tim7>;
using Timer8  = TimerHandler<Instance::tim8>;
using Timer9  = TimerHandler<Instance::tim9>;
using Timer10 = TimerHandler<Instance::tim10>;
using Timer11 = TimerHandler<Instance::tim11>;
using Timer12 = TimerHandler<Instance::tim12>;
using Timer13 = TimerHandler<Instance::tim13>;
using Timer14 = TimerHandler<Instance::tim14>;

using BasicTimer6 = Timer6;
using BasicTimer7 = Timer7;

} // namespace timer

