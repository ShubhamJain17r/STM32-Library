/**
 * @file dma_helper.hpp
 * @brief Low-level register manipulation helper functions for STM32 DMA streams
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/dma/core/dma_types.hpp"
#include "stm32/dma/core/dma_config.hpp"
#include "stm32/dma/core/dma_traits.hpp"

namespace dma::helper
{

/**
 * @brief Disables a stream and waits until the hardware acknowledges the disable (EN bit cleared).
 * @note Per RM0390 §9.3.16: Software must wait for EN to be cleared before reconfiguring the stream.
 */
inline void disableStream(DMA_Stream_TypeDef* stream) noexcept
{
    reg::clearBits(stream->CR, DMA_SxCR_EN);
    while(reg::isBitSet(stream->CR, DMA_SxCR_EN))
    {
        // Wait for EN bit to clear
    }
}

/**
 * @brief Enables a DMA stream.
 */
inline void enableStream(DMA_Stream_TypeDef* stream) noexcept
{
    reg::setBits(stream->CR, DMA_SxCR_EN);
}

/**
 * @brief Checks if a DMA stream is currently enabled.
 */
inline bool isStreamEnabled(const DMA_Stream_TypeDef* stream) noexcept
{
    return reg::isBitSet(stream->CR, DMA_SxCR_EN);
}

/**
 * @brief Returns the number of remaining data items from NDTR.
 */
inline std::uint16_t getRemainingItems(const DMA_Stream_TypeDef* stream) noexcept
{
    return static_cast<std::uint16_t>(reg::read(stream->NDTR));
}

/**
 * @brief Sets the number of data items in NDTR (Stream must be disabled).
 */
inline void setItemCount(DMA_Stream_TypeDef* stream, std::uint16_t count) noexcept
{
    reg::write(stream->NDTR, count);
}

/**
 * @brief Sets the Peripheral Address Register (PAR).
 */
inline void setPeripheralAddress(DMA_Stream_TypeDef* stream, std::uint32_t address) noexcept
{
    reg::write(stream->PAR, address);
}

/**
 * @brief Sets Memory 0 Address Register (M0AR).
 */
inline void setMemory0Address(DMA_Stream_TypeDef* stream, std::uint32_t address) noexcept
{
    reg::write(stream->M0AR, address);
}

/**
 * @brief Sets Memory 1 Address Register (M1AR) used in double-buffer mode.
 */
inline void setMemory1Address(DMA_Stream_TypeDef* stream, std::uint32_t address) noexcept
{
    reg::write(stream->M1AR, address);
}

/**
 * @brief Clears all status and interrupt flags for a stream in LIFCR or HIFCR.
 */
inline void clearAllFlags(DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = 0x3DU << shift; // Bits 0(FE), 2(DME), 3(TE), 4(HT), 5(TC)

    if(s < 4u)
    {
        reg::write(dma->LIFCR, mask);
    }
    else
    {
        reg::write(dma->HIFCR, mask);
    }
}

/**
 * @brief Clears the Transfer Complete flag (TCIF).
 */
inline void clearTransferComplete(DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = (1U << 5) << shift;

    if(s < 4u) reg::write(dma->LIFCR, mask);
    else       reg::write(dma->HIFCR, mask);
}

/**
 * @brief Clears the Half Transfer flag (HTIF).
 */
inline void clearHalfTransfer(DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = (1U << 4) << shift;

    if(s < 4u) reg::write(dma->LIFCR, mask);
    else       reg::write(dma->HIFCR, mask);
}

/**
 * @brief Clears error flags (TEIF, DMEIF, FEIF).
 */
inline void clearErrorFlags(DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = ((1U << 3) | (1U << 2) | (1U << 0)) << shift;

    if(s < 4u) reg::write(dma->LIFCR, mask);
    else       reg::write(dma->HIFCR, mask);
}

/**
 * @brief Checks if Transfer Complete flag (TCIF) is set.
 */
inline bool isTransferComplete(const DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = (1U << 5) << shift;

    return (s < 4u) ? reg::isBitSet(dma->LISR, mask) : reg::isBitSet(dma->HISR, mask);
}

/**
 * @brief Checks if Half Transfer flag (HTIF) is set.
 */
inline bool isHalfTransfer(const DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = (1U << 4) << shift;

    return (s < 4u) ? reg::isBitSet(dma->LISR, mask) : reg::isBitSet(dma->HISR, mask);
}

/**
 * @brief Checks if Transfer Error flag (TEIF) is set.
 */
inline bool isTransferError(const DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = (1U << 3) << shift;

    return (s < 4u) ? reg::isBitSet(dma->LISR, mask) : reg::isBitSet(dma->HISR, mask);
}

/**
 * @brief Checks if any error flag (TEIF, DMEIF, FEIF) is set.
 */
inline bool isAnyError(const DMA_TypeDef* dma, StreamId stream) noexcept
{
    const auto s = static_cast<std::uint8_t>(stream);
    const std::uint8_t shift = getStreamFlagShift(s);
    const std::uint32_t mask = ((1U << 3) | (1U << 2) | (1U << 0)) << shift;

    return (s < 4u) ? reg::isAnyBitSet(dma->LISR, mask) : reg::isAnyBitSet(dma->HISR, mask);
}

/**
 * @brief Configures CR and FCR registers for a given stream based on DmaStreamConfig.
 * Stream must be disabled before calling this function.
 */
inline void applyConfiguration(DMA_Stream_TypeDef* stream, const DmaStreamConfig& cfg) noexcept
{
    // Configure FIFO Control Register (FCR)
    std::uint32_t fcrVal = 0;
    if(cfg.fifoEnable)
    {
        fcrVal |= DMA_SxFCR_DMDIS; // Disable direct mode -> Enable FIFO
        fcrVal |= (static_cast<std::uint32_t>(cfg.fifoThreshold) & 0x03U) << DMA_SxFCR_FTH_Pos;
        fcrVal |= DMA_SxFCR_FEIE;  // Enable FIFO error interrupt
    }
    reg::write(stream->FCR, fcrVal);

    // Determine PSIZE, MSIZE, PINC, MINC based on Direction
    DataSize periphSize = cfg.dstSize;
    DataSize memSize    = cfg.srcSize;
    bool periphInc      = cfg.dstInc;
    bool memInc         = cfg.srcInc;

    if(cfg.direction == Direction::PeripheralToMemory)
    {
        periphSize = cfg.srcSize;
        memSize    = cfg.dstSize;
        periphInc  = cfg.srcInc;
        memInc     = cfg.dstInc;
    }
    else if(cfg.direction == Direction::MemoryToMemory)
    {
        // In M2M: PAR is source (periphSize), M0AR is destination (memSize)
        periphSize = cfg.srcSize;
        memSize    = cfg.dstSize;
        periphInc  = cfg.srcInc;
        memInc     = cfg.dstInc;
    }

    std::uint32_t crVal = 0;
    // Channel selection
    crVal |= (static_cast<std::uint32_t>(cfg.channel) & 0x07U) << DMA_SxCR_CHSEL_Pos;
    // Memory burst & peripheral burst
    crVal |= (static_cast<std::uint32_t>(cfg.memBurst) & 0x03U) << DMA_SxCR_MBURST_Pos;
    crVal |= (static_cast<std::uint32_t>(cfg.periphBurst) & 0x03U) << DMA_SxCR_PBURST_Pos;
    // Priority
    crVal |= (static_cast<std::uint32_t>(cfg.priority) & 0x03U) << DMA_SxCR_PL_Pos;
    // Sizes
    crVal |= (static_cast<std::uint32_t>(memSize) & 0x03U) << DMA_SxCR_MSIZE_Pos;
    crVal |= (static_cast<std::uint32_t>(periphSize) & 0x03U) << DMA_SxCR_PSIZE_Pos;
    // Increments
    if(memInc)    crVal |= DMA_SxCR_MINC;
    if(periphInc) crVal |= DMA_SxCR_PINC;
    // Circular mode
    if(cfg.mode == Mode::Circular) crVal |= DMA_SxCR_CIRC;
    // Direction
    crVal |= (static_cast<std::uint32_t>(cfg.direction) & 0x03U) << DMA_SxCR_DIR_Pos;
    // Double buffer
    if(cfg.doubleBuffer == DoubleBufferMode::Enabled) crVal |= DMA_SxCR_DBM;
    // Flow control
    if(cfg.flowControl == FlowControl::Peripheral) crVal |= DMA_SxCR_PFCTRL;

    // Interrupt enables: Transfer Complete (TCIE), Half Transfer (HTIE), Error (TEIE), Direct Mode Error (DMEIE)
    crVal |= (DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);

    reg::write(stream->CR, crVal);
}

/**
 * @brief Enables or disables Half Transfer interrupt (HTIE).
 */
inline void setHalfTransferInterrupt(DMA_Stream_TypeDef* stream, bool enable) noexcept
{
    if(enable) reg::setBits(stream->CR, DMA_SxCR_HTIE);
    else       reg::clearBits(stream->CR, DMA_SxCR_HTIE);
}

} // namespace dma::helper
