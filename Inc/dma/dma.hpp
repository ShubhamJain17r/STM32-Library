#pragma once
#include <cstdint>

namespace dma {

enum class StatusFlag
{
	TRANSFER_COMPLETE,
	HALF_TRANSFER_COMPLETE,
	TRANSFER_ERROR,
	DIRECT_MODE_ERROR,
	FIFO_MODE_ERROR
};

enum class Channel : std::uint8_t {
    CH0 = 0, CH1, CH2, CH3, CH4, CH5, CH6, CH7
};

enum class BurstType : std::uint8_t {
    SINGLE = 0, INCR4, INCR8, INCR16
};

enum class CurrentTarget
{
	MEMORY0, MEMORY1
};

enum class Priority : std::uint8_t {
    LOW = 0, MEDIUM, HIGH, VERY_HIGH
};

enum class DataSize : std::uint8_t {
    BYTE = 0, HALF_WORD, WORD
};

enum class FifoStatus : std::uint8_t {
    LESS_THAN_QUARTER     = 0b000, // 0 < fifo_level < 1/4
    QUARTER_TO_HALF       = 0b001, // 1/4 <= fifo_level < 1/2
    HALF_TO_THREE_QUARTER = 0b010, // 1/2 <= fifo_level < 3/4
    THREE_QUARTER_TO_FULL = 0b011, // 3/4 <= fifo_level < full
    EMPTY                 = 0b100, // FIFO is empty
    FULL                  = 0b101  // FIFO is full
};

enum class FifoThreshold : std::uint8_t {
    QUARTER = 0,
    HALF,
    THREE_QUARTERS,
    FULL
};

enum class TransferDirection : std::uint8_t {
    PERIPHERAL_TO_MEMORY,
    MEMORY_TO_PERIPHERAL,
    MEMORY_TO_MEMORY
};

} // namespace dma
