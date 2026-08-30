/**
 * @file i2c_interrupt.hpp
 * @brief Interrupt event and error dispatch layer for I2C peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/callback/callback.hpp"
#include "stm32/i2c/core/i2c_types.hpp"
#include "stm32/i2c/core/i2c_traits.hpp"

namespace i2c::interrupt
{

enum class Event : std::uint8_t
{
    Start,
    Address,
    ByteTransferFinished,
    RxNotEmpty,
    TxEmpty,
    Stop,
    Error
};

struct I2cCallbacks
{
    stm32::Callback start                = nullptr;
    stm32::Callback address              = nullptr;
    stm32::Callback byteTransferFinished = nullptr;
    stm32::Callback rxNotEmpty           = nullptr;
    stm32::Callback txEmpty              = nullptr;
    stm32::Callback stop                 = nullptr;
    stm32::Callback error                = nullptr;
};

class I2cEvent
{
public:
    static void setDeveloperCallback(Instance I, Event ev, stm32::Callback cb) noexcept;
    static void setUserCallback(Instance I, Event ev, stm32::Callback cb) noexcept;

    static void handleEvent(Instance I) noexcept;
    static void handleError(Instance I) noexcept;

private:
    inline static I2cCallbacks developerCallbacks_[3];
    inline static I2cCallbacks userCallbacks_[3];
};

inline void enableIRQ(IRQn_Type irq) noexcept
{
    NVIC_EnableIRQ(irq);
}

inline void disableIRQ(IRQn_Type irq) noexcept
{
    NVIC_DisableIRQ(irq);
}

} // namespace i2c::interrupt

