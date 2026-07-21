#pragma once

#include <cstdint>
#include "stm32f446xx.h"

#include "dma/dma_types.hpp"

namespace i2c
{

enum class CommunicationSpeed : std::uint8_t
{
	STANDARD_MODE = 0,
	FAST_MODE
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

typedef struct
{

}I2cConfig;

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
