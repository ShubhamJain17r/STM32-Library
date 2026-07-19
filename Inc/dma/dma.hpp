#pragma once
#include <cstdint>

namespace dma {

enum class Channel : std::uint8_t {
    CH0 = 0, CH1, CH2, CH3, CH4, CH5, CH6, CH7
};

enum class BurstType : std::uint8_t {
    SINGLE = 0, INCR4, INCR8, INCR16
};

enum class Priority : std::uint8_t {
    LOW = 0, MEDIUM, HIGH, VERY_HIGH
};

enum class DataSize : std::uint8_t {
    BYTE = 0, HALF_WORD, WORD
};

// Fixed: Unified to UPPER_CASE, corrected spelling ("quarter" vs "forth")
enum class FifoThreshold : std::uint8_t {
    QUARTER = 0,
    HALF,
    THREE_QUARTERS,
    FULL
};

// Fixed: Unified to UPPER_CASE with clear snake_case separators
enum class TransferDirection : std::uint8_t {
    PERIPHERAL_TO_MEMORY,
    MEMORY_TO_PERIPHERAL,
    MEMORY_TO_MEMORY
};

} // namespace dma
