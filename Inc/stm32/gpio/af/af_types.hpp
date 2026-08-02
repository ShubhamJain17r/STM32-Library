#pragma once

#include <cstdint>

namespace gpio::af
{

enum class AlternateFunction : std::uint8_t
{
	AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15,

	INVALID = 0xFF
};

enum class Signal
{
	I2C1_SDA, I2C1_SCL,
	I2C2_SDA, I2C2_SCL,
	I2C3_SDA, I2C3_SCL,

	SPI1_SCK, SPI1_MOSI, SPI1_MISO, SPI1_NSS,
	SPI2_SCK, SPI2_MOSI, SPI2_MISO, SPI2_NSS,
	SPI3_SCK, SPI3_MOSI, SPI3_MISO, SPI3_NSS,

	USART1_TX, USART1_RX,
	USART2_TX, USART2_RX,
	USART3_TX, USART3_RX,
	UART4_TX, UART4_RX,
	UART5_TX, UART5_RX,
	USART6_TX, USART6_RX
};

} // namespace gpio::af
