/**
 * @file dma_types.hpp
 * @brief Core enumeration types for STM32 DMA configuration
 */

#pragma once

#include <cstdint>

namespace dma
{

/**
 * @brief DMA Controller Instance
 */
enum class Controller : std::uint8_t
{
    Dma1 = 0,
    Dma2 = 1
};

/**
 * @brief DMA Stream Identifier (Streams 0 through 7 per controller)
 */
enum class StreamId : std::uint8_t
{
    Stream0 = 0,
    Stream1 = 1,
    Stream2 = 2,
    Stream3 = 3,
    Stream4 = 4,
    Stream5 = 5,
    Stream6 = 6,
    Stream7 = 7
};

/**
 * @brief DMA Channel Selection (Channels 0 through 7 per stream)
 */
enum class ChannelId : std::uint8_t
{
    Channel0 = 0,
    Channel1 = 1,
    Channel2 = 2,
    Channel3 = 3,
    Channel4 = 4,
    Channel5 = 5,
    Channel6 = 6,
    Channel7 = 7
};

/**
 * @brief DMA Transfer Direction
 */
enum class Direction : std::uint8_t
{
    PeripheralToMemory = 0, // DIR = 00
    MemoryToPeripheral = 1, // DIR = 01
    MemoryToMemory     = 2  // DIR = 10 (Supported only on DMA2)
};

/**
 * @brief Stream Software Priority Level
 */
enum class Priority : std::uint8_t
{
    Low      = 0, // PL = 00
    Medium   = 1, // PL = 01
    High     = 2, // PL = 10
    VeryHigh = 3  // PL = 11
};

/**
 * @brief Data Transfer Width / Size
 */
enum class DataSize : std::uint8_t
{
    Byte     = 0, // 8-bit
    HalfWord = 1, // 16-bit
    Word     = 2  // 32-bit
};

/**
 * @brief Memory & Peripheral Burst Transfer Configuration
 */
enum class Burst : std::uint8_t
{
    Single = 0,
    Incr4  = 1,
    Incr8  = 2,
    Incr16 = 3
};

/**
 * @brief FIFO Threshold Selection (used when FIFO mode is enabled)
 */
enum class FifoThreshold : std::uint8_t
{
    Quarter       = 0, // 1/4 full FIFO
    Half          = 1, // 1/2 full FIFO
    ThreeQuarters = 2, // 3/4 full FIFO
    Full          = 3  // Full FIFO
};

/**
 * @brief DMA Operating Mode (Normal vs Circular)
 */
enum class Mode : std::uint8_t
{
    Normal   = 0,
    Circular = 1
};

/**
 * @brief Double Buffer Mode
 */
enum class DoubleBufferMode : std::uint8_t
{
    Disabled = 0,
    Enabled  = 1
};

/**
 * @brief Target Memory Buffer in Double Buffer Mode
 */
enum class TargetMemory : std::uint8_t
{
    Memory0 = 0,
    Memory1 = 1
};

/**
 * @brief Flow Controller
 */
enum class FlowControl : std::uint8_t
{
    Dma        = 0, // DMA is flow controller
    Peripheral = 1  // Peripheral is flow controller (PFCTRL)
};

} // namespace dma
