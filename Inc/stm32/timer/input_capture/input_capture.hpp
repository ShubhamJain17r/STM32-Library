/**
 * @file input_capture.hpp
 * @brief High-level Input Capture driver for frequency and pulse measurement
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
#include "stm32/timer/interrupt/timer_interrupt.hpp"

namespace timer
{

template<Instance I>
class InputCaptureHandler
{
public:
    static_assert(!Traits<I>::isBasic,
        "Basic timers (TIM6/TIM7) do not support Input Capture. Use a General Purpose or Advanced timer.");

    ~InputCaptureHandler();

    InputCaptureHandler(const InputCaptureHandler&) = delete;
    InputCaptureHandler& operator=(const InputCaptureHandler&) = delete;

    InputCaptureHandler(InputCaptureHandler&&) = delete;
    InputCaptureHandler& operator=(InputCaptureHandler&&) = delete;

    explicit InputCaptureHandler(std::uint32_t prescaler = 0u);

    void setPrescaler(std::uint32_t psc) noexcept;

    bool enableChannel(Channel ch, gpio::Pin pin,
                       CaptureEdge edge = CaptureEdge::Rising,
                       InputPrescaler psc = InputPrescaler::Div1) noexcept;

    void disableChannel(Channel ch) noexcept;

    std::uint32_t getCaptureValue(Channel ch) const noexcept;
    float readFrequency(Channel ch) noexcept;

    void setCallback(Channel ch, interrupt::ChannelCallback callback) noexcept;
    void enableInterrupt(Channel ch) noexcept;
    void disableInterrupt(Channel ch) noexcept;

    void start() noexcept;
    void stop() noexcept;

private:
    void initClock() noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

template<Instance I>
void InputCaptureHandler<I>::initClock() noexcept
{
    rcc::enablePeripheralClock(Traits<I>::peripheral());
}

template<Instance I>
InputCaptureHandler<I>::InputCaptureHandler(std::uint32_t prescaler)
{
    initClock();
    helper::setPrescaler(Traits<I>::peripheral(), prescaler);

    if constexpr(Traits<I>::is32Bit)
        helper::setAutoReload(Traits<I>::peripheral(), 0xFFFF'FFFFU);
    else
        helper::setAutoReload(Traits<I>::peripheral(), 0xFFFFU);

    helper::generateUpdate(Traits<I>::peripheral());
}

template<Instance I>
InputCaptureHandler<I>::~InputCaptureHandler()
{
    stop();
}

template<Instance I>
void InputCaptureHandler<I>::setPrescaler(std::uint32_t psc) noexcept
{
    helper::setPrescaler(Traits<I>::peripheral(), psc);
    helper::generateUpdate(Traits<I>::peripheral());
}

template<Instance I>
bool InputCaptureHandler<I>::enableChannel(Channel ch, gpio::Pin pin, CaptureEdge edge, InputPrescaler psc) noexcept
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
    gpio::helper::setPull(pin, gpio::Pull::NONE);
    gpio::helper::setMode(pin, gpio::Mode::ALTERNATE);

    helper::configureInputCapture(Traits<I>::peripheral(), ch, edge, psc);
    helper::enableChannelOutput(Traits<I>::peripheral(), ch);

    return true;
}

template<Instance I>
void InputCaptureHandler<I>::disableChannel(Channel ch) noexcept
{
    helper::disableChannelOutput(Traits<I>::peripheral(), ch);
}

template<Instance I>
std::uint32_t InputCaptureHandler<I>::getCaptureValue(Channel ch) const noexcept
{
    return helper::getCapture(Traits<I>::peripheral(), ch);
}

template<Instance I>
void InputCaptureHandler<I>::setCallback(Channel ch, interrupt::ChannelCallback callback) noexcept
{
    interrupt::TimerEvent::setCaptureCallback(I, ch, callback);
}

template<Instance I>
void InputCaptureHandler<I>::enableInterrupt(Channel ch) noexcept
{
    helper::clearChannelFlag(Traits<I>::peripheral(), ch);
    helper::enableChannelInterrupt(Traits<I>::peripheral(), ch);
    interrupt::enableIRQ(Traits<I>::irq);
}

template<Instance I>
void InputCaptureHandler<I>::disableInterrupt(Channel ch) noexcept
{
    helper::disableChannelInterrupt(Traits<I>::peripheral(), ch);
}

template<Instance I>
void InputCaptureHandler<I>::start() noexcept
{
    helper::enableCounter(Traits<I>::peripheral());
}

template<Instance I>
void InputCaptureHandler<I>::stop() noexcept
{
    helper::disableCounter(Traits<I>::peripheral());
}

template<Instance I>
float InputCaptureHandler<I>::readFrequency(Channel ch) noexcept
{
    auto* tim = Traits<I>::peripheral();
    std::uint32_t flag = 0;
    switch(ch)
    {
        case Channel::CH1: flag = TIM_SR_CC1IF; break;
        case Channel::CH2: flag = TIM_SR_CC2IF; break;
        case Channel::CH3: flag = TIM_SR_CC3IF; break;
        case Channel::CH4: flag = TIM_SR_CC4IF; break;
    }

    // Clear pending flag and wait for first edge
    reg::clearBits(tim->SR, flag);
    std::uint32_t timeout = 1000000u;
    while(!reg::isAnyBitSet(tim->SR, flag) && --timeout) {}
    if(timeout == 0) return 0.0f;
    const std::uint32_t val1 = getCaptureValue(ch);

    // Clear flag and wait for second edge
    reg::clearBits(tim->SR, flag);
    timeout = 1000000u;
    while(!reg::isAnyBitSet(tim->SR, flag) && --timeout) {}
    if(timeout == 0) return 0.0f;
    const std::uint32_t val2 = getCaptureValue(ch);

    const std::uint32_t arr = helper::getAutoReload(tim);
    const std::uint32_t delta = (val2 >= val1) ? (val2 - val1) : (arr - val1 + val2 + 1u);
    if(delta == 0) return 0.0f;

    const std::uint32_t timerClk = rcc::getTimerFrequency(Traits<I>::bus);
    const std::uint32_t psc = helper::getPrescaler(tim) + 1u;
    return static_cast<float>(timerClk) / (static_cast<float>(psc) * static_cast<float>(delta));
}

using InputCapture1  = InputCaptureHandler<Instance::tim1>;
using InputCapture2  = InputCaptureHandler<Instance::tim2>;
using InputCapture3  = InputCaptureHandler<Instance::tim3>;
using InputCapture4  = InputCaptureHandler<Instance::tim4>;
using InputCapture5  = InputCaptureHandler<Instance::tim5>;
using InputCapture8  = InputCaptureHandler<Instance::tim8>;
using InputCapture9  = InputCaptureHandler<Instance::tim9>;
using InputCapture10 = InputCaptureHandler<Instance::tim10>;
using InputCapture11 = InputCaptureHandler<Instance::tim11>;
using InputCapture12 = InputCaptureHandler<Instance::tim12>;
using InputCapture13 = InputCaptureHandler<Instance::tim13>;
using InputCapture14 = InputCaptureHandler<Instance::tim14>;

template<Instance I>
using Capture = InputCaptureHandler<I>;

using Capture1  = InputCapture1;
using Capture2  = InputCapture2;
using Capture3  = InputCapture3;
using Capture4  = InputCapture4;
using Capture5  = InputCapture5;
using Capture8  = InputCapture8;
using Capture9  = InputCapture9;
using Capture10 = InputCapture10;
using Capture11 = InputCapture11;
using Capture12 = InputCapture12;
using Capture13 = InputCapture13;
using Capture14 = InputCapture14;

} // namespace timer

