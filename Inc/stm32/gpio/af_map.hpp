#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin_map.hpp"

namespace af
{

enum class Signal
{
	I2C1_SDA, I2C1_SCL,
	I2C2_SDA, I2C2_SCL,
	I2C3_SDA, I2C3_SCL,

	SPI1_SCK, SPI1_MOSI, SPI1_MISO, SPI1_NSS,
	SPI2_SCK, SPI2_MOSI, SPI2_MISO, SPI2_NSS,
	SPI3_SCK, SPI3_MOSI, SPI3_MISO, SPI3_NSS,

	UART1_TX, UART1_RX,
	UART2_TX, UART2_RX,
	UART3_TX, UART3_RX,

	TOTAL
};

constexpr std::uint8_t afTable[Signal::TOTAL][128];

} // namespace af
