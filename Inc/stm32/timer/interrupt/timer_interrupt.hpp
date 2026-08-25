#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/callback.hpp"
#include "stm32/timer/timer_types.hpp"
#include "stm32/timer/timer_traits.hpp"

namespace timer::interrupt
{

using ChannelCallback = void(*)(std::uint32_t captureValue);

struct TimerCallbacks
{
    stm32::Callback updateCallback = nullptr;
    stm32::Callback channelCallback[4] = {nullptr, nullptr, nullptr, nullptr};
    ChannelCallback captureCallback[4] = {nullptr, nullptr, nullptr, nullptr};
};

class TimerEvent
{
public:
    static void setUpdateCallback(Instance I, stm32::Callback cb) noexcept;
    static void setChannelCallback(Instance I, Channel ch, stm32::Callback cb) noexcept;
    static void setCaptureCallback(Instance I, Channel ch, ChannelCallback cb) noexcept;

    static void handleEvent(Instance I) noexcept;

private:
    inline static TimerCallbacks callbacks_[14];
};

inline void enableIRQ(IRQn_Type irq) noexcept
{
    NVIC_EnableIRQ(irq);
}

inline void disableIRQ(IRQn_Type irq) noexcept
{
    NVIC_DisableIRQ(irq);
}

} // namespace timer::interrupt
