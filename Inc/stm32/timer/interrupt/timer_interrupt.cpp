#include "stm32/timer/interrupt/timer_interrupt.hpp"
#include "stm32/timer/timer_helper.hpp"

namespace timer::interrupt
{

static constexpr std::size_t index(Instance I) noexcept
{
    return static_cast<std::size_t>(I);
}

static TIM_TypeDef* peripheral(Instance I) noexcept
{
    switch(I)
    {
        case Instance::tim1:  return TIM1;
        case Instance::tim2:  return TIM2;
        case Instance::tim3:  return TIM3;
        case Instance::tim4:  return TIM4;
        case Instance::tim5:  return TIM5;
        case Instance::tim6:  return TIM6;
        case Instance::tim7:  return TIM7;
        case Instance::tim8:  return TIM8;
        case Instance::tim9:  return TIM9;
        case Instance::tim10: return TIM10;
        case Instance::tim11: return TIM11;
        case Instance::tim12: return TIM12;
        case Instance::tim13: return TIM13;
        case Instance::tim14: return TIM14;
    }
    return nullptr;
}

void TimerEvent::setUpdateCallback(Instance I, stm32::Callback cb) noexcept
{
    callbacks_[index(I)].updateCallback = cb;
}

void TimerEvent::setChannelCallback(Instance I, Channel ch, stm32::Callback cb) noexcept
{
    const auto chIdx = static_cast<std::size_t>(ch) - 1u;
    if(chIdx < 4u)
    {
        callbacks_[index(I)].channelCallback[chIdx] = cb;
    }
}

void TimerEvent::setCaptureCallback(Instance I, Channel ch, ChannelCallback cb) noexcept
{
    const auto chIdx = static_cast<std::size_t>(ch) - 1u;
    if(chIdx < 4u)
    {
        callbacks_[index(I)].captureCallback[chIdx] = cb;
    }
}

void TimerEvent::handleEvent(Instance I) noexcept
{
    TIM_TypeDef* timer = peripheral(I);
    if(!timer) return;

    auto& cb = callbacks_[index(I)];

    // Handle Update Interrupt Flag (UIF)
    if(helper::isUpdateFlag(timer) && reg::isAnyBitSet(timer->DIER, TIM_DIER_UIE))
    {
        helper::clearUpdateFlag(timer);
        if(cb.updateCallback)
        {
            cb.updateCallback();
        }
    }

    // Handle Channel 1..4 Capture/Compare Flags
    for(std::uint8_t c = 1u; c <= 4u; ++c)
    {
        const auto ch = static_cast<Channel>(c);
        const auto chIdx = static_cast<std::size_t>(c - 1u);
        const std::uint32_t ieMask = 1U << c;

        if(helper::isChannelFlag(timer, ch) && reg::isAnyBitSet(timer->DIER, ieMask))
        {
            helper::clearChannelFlag(timer, ch);

            if(cb.channelCallback[chIdx])
            {
                cb.channelCallback[chIdx]();
            }

            if(cb.captureCallback[chIdx])
            {
                cb.captureCallback[chIdx](helper::getCapture(timer, ch));
            }
        }
    }
}

} // namespace timer::interrupt
