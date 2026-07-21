#pragma once

#include <cstdint>
#include "stm32f446xx.h"

#include "dma/dma_types.hpp"

namespace i2c
{

enum class Event
{
	START_GENERATED, ADDRESS_SENT, BYTE_TRANSFER_FINISHED
};

enum class Error
{
	BUS_ERROR, ARBITRATION_LOST, ACKNOWLEDGE_FAILURE, DATA_ERROR
};

enum class OperatingMode
{
	TARGET, CONTROLLER
};

enum class Operation : std::uint8_t
{
	WRITE = 0,
	READ
};

struct DmaMapping {
        DMA_TypeDef* txDmaBase;
        DMA_Stream_TypeDef* txStream;
        DMA_TypeDef* rxDmaBase;
        DMA_Stream_TypeDef* rxStream;
        dma::Channel channel;
};

struct GpioMapping {
	GPIO_TypeDef* sdaPort;
	std::uint8_t sdaPin;
	GPIO_TypeDef* sclPort;
	std::uint8_t sclPin;
};

} // namespace i2c
