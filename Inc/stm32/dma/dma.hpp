/**
 * @file dma.hpp
 * @brief Master umbrella header for STM32 DMA Subsystem
 */

#pragma once

#include "stm32/dma/core/dma_types.hpp"
#include "stm32/dma/core/dma_traits.hpp"
#include "stm32/dma/core/dma_request_traits.hpp"
#include "stm32/dma/core/dma_config.hpp"
#include "stm32/dma/core/dma_helper.hpp"
#include "stm32/dma/interrupt/dma_interrupt.hpp"
#include "stm32/dma/handler/dma_stream.hpp"

namespace dma
{

// ---------------------------------------------------------------------------
// Convenient Type Aliases for DMA1 Streams
// ---------------------------------------------------------------------------
using Dma1Stream0 = DmaStream<Controller::Dma1, StreamId::Stream0>;
using Dma1Stream1 = DmaStream<Controller::Dma1, StreamId::Stream1>;
using Dma1Stream2 = DmaStream<Controller::Dma1, StreamId::Stream2>;
using Dma1Stream3 = DmaStream<Controller::Dma1, StreamId::Stream3>;
using Dma1Stream4 = DmaStream<Controller::Dma1, StreamId::Stream4>;
using Dma1Stream5 = DmaStream<Controller::Dma1, StreamId::Stream5>;
using Dma1Stream6 = DmaStream<Controller::Dma1, StreamId::Stream6>;
using Dma1Stream7 = DmaStream<Controller::Dma1, StreamId::Stream7>;

// ---------------------------------------------------------------------------
// Convenient Type Aliases for DMA2 Streams
// ---------------------------------------------------------------------------
using Dma2Stream0 = DmaStream<Controller::Dma2, StreamId::Stream0>;
using Dma2Stream1 = DmaStream<Controller::Dma2, StreamId::Stream1>;
using Dma2Stream2 = DmaStream<Controller::Dma2, StreamId::Stream2>;
using Dma2Stream3 = DmaStream<Controller::Dma2, StreamId::Stream3>;
using Dma2Stream4 = DmaStream<Controller::Dma2, StreamId::Stream4>;
using Dma2Stream5 = DmaStream<Controller::Dma2, StreamId::Stream5>;
using Dma2Stream6 = DmaStream<Controller::Dma2, StreamId::Stream6>;
using Dma2Stream7 = DmaStream<Controller::Dma2, StreamId::Stream7>;

} // namespace dma
