/**
 * @file dma_interrupt.hpp
 * @brief Interrupt dispatch layer and callback registries for DMA1 and DMA2
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/callback/callback.hpp"
#include "stm32/dma/core/dma_types.hpp"

namespace dma::interrupt
{

enum class Event : std::uint8_t
{
    TransferComplete,
    HalfTransfer,
    Error
};

struct StreamCallbacks
{
    stm32::Callback onComplete = nullptr;
    stm32::Callback onHalf     = nullptr;
    stm32::Callback onError    = nullptr;
};

class DmaEvent
{
public:
    static void setDeveloperCallback(Controller c, StreamId s, Event ev, stm32::Callback cb) noexcept;
    static void setUserCallback(Controller c, StreamId s, Event ev, stm32::Callback cb) noexcept;

    static void handleInterrupt(Controller c, StreamId s) noexcept;

private:
    // [Controller (0..1)][Stream (0..7)]
    inline static StreamCallbacks developerCallbacks_[2][8];
    inline static StreamCallbacks userCallbacks_[2][8];
};

inline void enableIRQ(IRQn_Type irq, std::uint32_t priority = 0) noexcept
{
    NVIC_SetPriority(irq, priority);
    NVIC_EnableIRQ(irq);
}

inline void disableIRQ(IRQn_Type irq) noexcept
{
    NVIC_DisableIRQ(irq);
}

} // namespace dma::interrupt
