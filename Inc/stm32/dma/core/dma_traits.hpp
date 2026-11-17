/**
 * @file dma_traits.hpp
 * @brief Hardware traits and register mapping for DMA1 and DMA2 streams on STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/dma/core/dma_types.hpp"

namespace dma
{

template<Controller C, StreamId S>
struct Traits;

// Helper to determine flag shift position based on stream index (0..7)
constexpr std::uint8_t getStreamFlagShift(std::uint8_t streamIdx) noexcept
{
    const std::uint8_t mod = streamIdx % 4u;
    return (mod == 0u) ? 0u :
           (mod == 1u) ? 6u :
           (mod == 2u) ? 16u : 22u;
}

// ---------------------------------------------------------------------------
// DMA1 Streams (0 through 7)
// ---------------------------------------------------------------------------

template<>
struct Traits<Controller::Dma1, StreamId::Stream0>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream0;
    static constexpr std::uint8_t streamIndex = 0;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(0);
    static constexpr IRQn_Type irq           = DMA1_Stream0_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream0; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream1>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream1;
    static constexpr std::uint8_t streamIndex = 1;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(1);
    static constexpr IRQn_Type irq           = DMA1_Stream1_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream1; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream2>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream2;
    static constexpr std::uint8_t streamIndex = 2;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(2);
    static constexpr IRQn_Type irq           = DMA1_Stream2_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream2; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream3>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream3;
    static constexpr std::uint8_t streamIndex = 3;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(3);
    static constexpr IRQn_Type irq           = DMA1_Stream3_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream3; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream4>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream4;
    static constexpr std::uint8_t streamIndex = 4;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(4);
    static constexpr IRQn_Type irq           = DMA1_Stream4_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream4; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream5>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream5;
    static constexpr std::uint8_t streamIndex = 5;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(5);
    static constexpr IRQn_Type irq           = DMA1_Stream5_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream5; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream6>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream6;
    static constexpr std::uint8_t streamIndex = 6;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(6);
    static constexpr IRQn_Type irq           = DMA1_Stream6_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream6; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

template<>
struct Traits<Controller::Dma1, StreamId::Stream7>
{
    static constexpr Controller controllerId = Controller::Dma1;
    static constexpr StreamId streamId       = StreamId::Stream7;
    static constexpr std::uint8_t streamIndex = 7;
    static constexpr std::uint8_t controllerIndex = 0;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(7);
    static constexpr IRQn_Type irq           = DMA1_Stream7_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA1; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA1_Stream7; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA1); }
};

// ---------------------------------------------------------------------------
// DMA2 Streams (0 through 7)
// ---------------------------------------------------------------------------

template<>
struct Traits<Controller::Dma2, StreamId::Stream0>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream0;
    static constexpr std::uint8_t streamIndex = 0;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(0);
    static constexpr IRQn_Type irq           = DMA2_Stream0_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream0; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream1>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream1;
    static constexpr std::uint8_t streamIndex = 1;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(1);
    static constexpr IRQn_Type irq           = DMA2_Stream1_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream1; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream2>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream2;
    static constexpr std::uint8_t streamIndex = 2;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(2);
    static constexpr IRQn_Type irq           = DMA2_Stream2_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream2; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream3>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream3;
    static constexpr std::uint8_t streamIndex = 3;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = false;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(3);
    static constexpr IRQn_Type irq           = DMA2_Stream3_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream3; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream4>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream4;
    static constexpr std::uint8_t streamIndex = 4;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(4);
    static constexpr IRQn_Type irq           = DMA2_Stream4_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream4; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream5>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream5;
    static constexpr std::uint8_t streamIndex = 5;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(5);
    static constexpr IRQn_Type irq           = DMA2_Stream5_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream5; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream6>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream6;
    static constexpr std::uint8_t streamIndex = 6;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(6);
    static constexpr IRQn_Type irq           = DMA2_Stream6_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream6; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

template<>
struct Traits<Controller::Dma2, StreamId::Stream7>
{
    static constexpr Controller controllerId = Controller::Dma2;
    static constexpr StreamId streamId       = StreamId::Stream7;
    static constexpr std::uint8_t streamIndex = 7;
    static constexpr std::uint8_t controllerIndex = 1;
    static constexpr bool isHighRegister     = true;
    static constexpr std::uint8_t flagShift  = getStreamFlagShift(7);
    static constexpr IRQn_Type irq           = DMA2_Stream7_IRQn;

    static DMA_TypeDef* controller() noexcept { return DMA2; }
    static DMA_Stream_TypeDef* stream() noexcept { return DMA2_Stream7; }
    static void enableClock() noexcept { rcc::enablePeripheralClock(DMA2); }
};

} // namespace dma
