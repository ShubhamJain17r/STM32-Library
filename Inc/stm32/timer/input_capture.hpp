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
#include "stm32/timer/interrupt/timer_interrupt.hpp"

namespace timer
{

template<Instance I>
class InputCaptureHandler
{
public:
    static_assert(!Traits<I>::isBasic,
        "Basic timers (TIM6/TIM7) do not support Input Capture. Use a General Purpose or Advanced timer.");

    InputCaptureHandler() = delete;
    ~InputCaptureHandler();

    InputCaptureHandler(const InputCaptureHandler&) = delete;
    InputCaptureHandler& operator=(const InputCaptureHandler&) = delete;

    InputCaptureHandler(InputCaptureHandler&&) = delete;
    InputCaptureHandler& operator=(InputCaptureHandler&&) = delete;

    /// @brief Construct input capture with a given timer prescaler (0 = full clock rate)
    explicit InputCaptureHandler(std::uint32_t prescaler = 0u);

    void setPrescaler(std::uint32_t psc) noexcept;

    /// @brief Enable a capture channel on a specific GPIO pin with automatic AF input configuration.
    ///        Returns false if the pin is not valid for this timer/channel combination.
    bool enableChannel(Channel ch, gpio::Pin pin,
                       CaptureEdge edge = CaptureEdge::Rising,
                       InputPrescaler psc = InputPrescaler::Div1) noexcept;

    void disableChannel(Channel ch) noexcept;

    /// @brief Read the last captured counter value for a channel
    std::uint32_t getCaptureValue(Channel ch) const noexcept;

    /// @brief Set capture interrupt callback: receives the raw captured counter value
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

    // Set ARR to maximum to allow the counter to free-run
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

    // 2. Configure GPIO as Alternate Function Input (no output type needed)
    pin.enableClock();
    gpio::af::setAlternateFunction(pin, afVal);
    gpio::helper::setPull(pin, gpio::Pull::NONE);
    gpio::helper::setMode(pin, gpio::Mode::ALTERNATE);

    // 3. Configure timer channel for Input Capture
    helper::configureInputCapture(Traits<I>::peripheral(), ch, edge, psc);
    helper::enableChannelOutput(Traits<I>::peripheral(), ch); // CC1E enables capture in IC mode

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

// ---------------------------------------------------------------------------
// Convenient Type Aliases
// ---------------------------------------------------------------------------
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

} // namespace timer
