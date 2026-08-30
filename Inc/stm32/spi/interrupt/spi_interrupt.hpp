/**
 * @file spi_interrupt.hpp
 * @brief Interrupt event dispatch layer for SPI peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/callback/callback.hpp"
#include "stm32/spi/core/spi_types.hpp"
#include "stm32/spi/core/spi_traits.hpp"

namespace spi::interrupt
{

enum class Event : std::uint8_t
{
    TxEmpty,
    RxNotEmpty,
    Error,
    TransferComplete
};

struct SpiCallbacks
{
    stm32::Callback txEmpty          = nullptr;
    stm32::Callback rxNotEmpty       = nullptr;
    stm32::Callback error            = nullptr;
    stm32::Callback transferComplete = nullptr;
};

class SpiEvent
{
public:
    static void setDeveloperCallback(Instance I, Event ev, stm32::Callback cb) noexcept;
    static void setUserCallback(Instance I, Event ev, stm32::Callback cb) noexcept;

    static void handleEvent(Instance I) noexcept;
    static void triggerTransferComplete(Instance I) noexcept;

private:
    inline static SpiCallbacks developerCallbacks_[4];
    inline static SpiCallbacks userCallbacks_[4];
};

inline void enableIRQ(IRQn_Type irq) noexcept
{
    NVIC_EnableIRQ(irq);
}

inline void disableIRQ(IRQn_Type irq) noexcept
{
    NVIC_DisableIRQ(irq);
}

} // namespace spi::interrupt

