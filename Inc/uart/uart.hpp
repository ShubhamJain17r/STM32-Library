#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <array>

#include "uart/uart_types.hpp"

#include "gpio/gpio.hpp"
#include "gpio/gpio_types.hpp"

#include "dma/dma.hpp"
#include "dma/dma_types.hpp"

namespace uart
{

class UartHandler
{

private:
	USART_TypeDef* usartBase_;

	gpio::Pin mosi_;
	gpio::Pin miso_;

	dma::DmaStream txDma_;
	dma::DmaStream rxDma_;

public:
	UartHandler() = delete;
	~UartHandler() = default;

	explicit UartHandler(USART_TypeDef* usartBase);

	UartHandler(const UartHandler&) = delete;
	UartHandler& operator=(const UartHandler&) = delete;

	UartHandler(UartHandler&&) noexcept = default;
	UartHandler& operator=(UartHandler&&) noexcept = default;

private:

public:

};

} // namespace uart
