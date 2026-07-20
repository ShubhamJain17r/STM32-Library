#pragma once
#include <cstdint>

namespace dma {

enum class Event
{
	CONFIG_ERROR,
	TRANSFER_COMPLETE,
	HALF_TRANSFER_COMPLETE,
	TRANSFER_ERROR,
	DIRECT_MODE_ERROR
};

enum class Channel : std::uint8_t {
    CH0 = 0, CH1, CH2, CH3, CH4, CH5, CH6, CH7
};

enum class Priority : std::uint8_t {
    LOW = 0, MEDIUM, HIGH, VERY_HIGH
};

enum class DataSize : std::uint8_t {
    BYTE = 0,
    HALF_WORD,
	WORD
};

enum class TransferDirection : std::uint8_t {
    PERIPHERAL_TO_MEMORY = 0b00,
    MEMORY_TO_PERIPHERAL = 0b01,
    MEMORY_TO_MEMORY     = 0b10
};

enum class OperationalMode : std::uint8_t {
    NORMAL = 0,
    CIRCULAR,
    DOUBLE_BUFFER
};

enum class FlowController : std::uint8_t {
	DMA = 0, PERIPHERAL
};

struct StreamConfig {
    Channel channel;
    TransferDirection direction;
    FlowController flowController = FlowController::DMA;
    Priority priority = Priority::LOW;
    DataSize dataSize = DataSize::BYTE;
    bool peripheralIncrement = false;
    bool memoryIncrement = false;
};

} // namespace dma
