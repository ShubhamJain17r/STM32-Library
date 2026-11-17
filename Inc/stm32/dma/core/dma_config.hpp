/**
 * @file dma_config.hpp
 * @brief High-level DMA stream configuration structures and builders
 */

#pragma once

#include "stm32/dma/core/dma_types.hpp"

namespace dma
{

/**
 * @brief Full Stream Configuration Structure with safe bare-metal defaults
 */
struct DmaStreamConfig
{
    ChannelId        channel        = ChannelId::Channel0;
    Direction        direction      = Direction::MemoryToPeripheral;
    Priority         priority       = Priority::Medium;
    Mode             mode           = Mode::Normal;
    DataSize         srcSize        = DataSize::Byte;
    DataSize         dstSize        = DataSize::Byte;
    bool             srcInc         = true;
    bool             dstInc         = false;
    bool             fifoEnable     = false;
    FifoThreshold    fifoThreshold  = FifoThreshold::Full;
    Burst            memBurst       = Burst::Single;
    Burst            periphBurst    = Burst::Single;
    DoubleBufferMode doubleBuffer   = DoubleBufferMode::Disabled;
    FlowControl      flowControl    = FlowControl::Dma;

    /**
     * @brief Creates a Memory-to-Memory configuration (Supported on DMA2 only)
     */
    static constexpr DmaStreamConfig memoryToMemory(DataSize size = DataSize::Word,
                                                    Priority prio = Priority::High) noexcept
    {
        DmaStreamConfig cfg{};
        cfg.direction     = Direction::MemoryToMemory;
        cfg.priority      = prio;
        cfg.srcSize       = size;
        cfg.dstSize       = size;
        cfg.srcInc        = true;
        cfg.dstInc        = true;
        cfg.mode          = Mode::Normal;
        cfg.fifoEnable    = true; // FIFO mode must be enabled for M2M transfers per RM0390
        cfg.fifoThreshold = FifoThreshold::Full;
        return cfg;
    }

    /**
     * @brief Creates a Memory-to-Peripheral configuration (e.g. UART TX, SPI TX)
     */
    static constexpr DmaStreamConfig memoryToPeripheral(ChannelId ch,
                                                        DataSize size = DataSize::Byte,
                                                        Priority prio = Priority::Medium) noexcept
    {
        DmaStreamConfig cfg{};
        cfg.channel   = ch;
        cfg.direction = Direction::MemoryToPeripheral;
        cfg.priority  = prio;
        cfg.srcSize   = size;
        cfg.dstSize   = size;
        cfg.srcInc    = true;
        cfg.dstInc    = false;
        cfg.mode      = Mode::Normal;
        return cfg;
    }

    /**
     * @brief Creates a Peripheral-to-Memory configuration (e.g. UART RX, ADC, SPI RX)
     */
    static constexpr DmaStreamConfig peripheralToMemory(ChannelId ch,
                                                        DataSize size = DataSize::Byte,
                                                        Mode streamMode = Mode::Normal,
                                                        Priority prio = Priority::Medium) noexcept
    {
        DmaStreamConfig cfg{};
        cfg.channel   = ch;
        cfg.direction = Direction::PeripheralToMemory;
        cfg.priority  = prio;
        cfg.srcSize   = size;
        cfg.dstSize   = size;
        cfg.srcInc    = false;
        cfg.dstInc    = true;
        cfg.mode      = streamMode;
        return cfg;
    }
};

} // namespace dma
