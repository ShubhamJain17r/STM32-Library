#include "stm32/timer/interrupt/timer_interrupt.hpp"
#include "stm32/timer/core/timer_helper.hpp"

namespace timer::interrupt
{

static constexpr std::size_t index(Instance I) noexcept
{
    return static_cast<std::size_t>(I);
}

static TIM_TypeDef* getPeripheral(Instance I) noexcept
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
    const auto chIdx = static_cast<std::uint8_t>(ch) - 1U;
    if(chIdx < 4)
    {
        callbacks_[index(I)].channelCallback[chIdx] = cb;
    }
}

void TimerEvent::setCaptureCallback(Instance I, Channel ch, ChannelCallback cb) noexcept
{
    const auto chIdx = static_cast<std::uint8_t>(ch) - 1U;
    if(chIdx < 4)
    {
        callbacks_[index(I)].captureCallback[chIdx] = cb;
    }
}

void TimerEvent::handleEvent(Instance I) noexcept
{
    auto* tim = getPeripheral(I);
    if(!tim) return;

    const auto& cb = callbacks_[index(I)];

    // Check Update Interrupt Flag (UIF)
    if(helper::isUpdateFlag(tim) && reg::isAnyBitSet(tim->DIER, TIM_DIER_UIE))
    {
        helper::clearUpdateFlag(tim);
        if(cb.updateCallback)
        {
            cb.updateCallback();
        }
    }

    // Check CC1..CC4 Flags (CC1IF..CC4IF)
    for(std::uint8_t ch = 1; ch <= 4; ++ch)
    {
        const auto channel = static_cast<Channel>(ch);
        const auto chIdx = ch - 1U;
        const auto dierMask = 1U << ch;

        if(helper::isChannelFlag(tim, channel) && reg::isAnyBitSet(tim->DIER, dierMask))
        {
            helper::clearChannelFlag(tim, channel);

            if(cb.channelCallback[chIdx])
            {
                cb.channelCallback[chIdx]();
            }

            if(cb.captureCallback[chIdx])
            {
                const std::uint32_t val = helper::getCapture(tim, channel);
                cb.captureCallback[chIdx](val);
            }
        }
    }
}

} // namespace timer::interrupt
