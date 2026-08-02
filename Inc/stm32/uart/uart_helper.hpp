#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/uart/uart_types.hpp"

#include "stm32/common/rcc.hpp"

namespace uart::helper
{

inline bool txReady(USART_TypeDef* uart)
{
	return uart->SR & USART_SR_TXE;
}

inline bool transmissionComplete(USART_TypeDef* uart)
{
	return uart->SR & USART_SR_TC;
}

inline bool rxReady(USART_TypeDef* uart)
{
	return uart->SR & USART_SR_RXNE;
}

inline void configureBaudRate(USART_TypeDef* uart, std::uint32_t baud, rcc::Bus bus)
{
	const std::uint32_t freq = frequency(bus);

	uart->BRR = (freq / baud);
}

inline void setOversampling(USART_TypeDef* uart, Oversampling over)
{
	uart->CR1 &= ~USART_CR1_OVER8;

	if(over == Oversampling::BY8)
	{
		uart->CR1 |= USART_CR1_OVER8;
	}
}

inline void enable(USART_TypeDef* uart)
{
	uart->CR1 |= USART_CR1_UE;
}

inline void disable(USART_TypeDef* uart)
{
	uart->CR1 &= ~USART_CR1_UE;
}

inline void setWordLength(USART_TypeDef* uart, WordLength len)
{
	uart->CR1 &= ~USART_CR1_M;

	if(len == WordLength::BITS_9)
	{
		uart->CR1 |= USART_CR1_M;
	}
}

inline void setParity(USART_TypeDef* uart, Parity parity)
{
    uart->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS);

    switch(parity)
    {
        case Parity::NONE:
            break;

        case Parity::EVEN:
            uart->CR1 |= USART_CR1_PCE;
            break;

        case Parity::ODD:
            uart->CR1 |= USART_CR1_PCE
                      |  USART_CR1_PS;
            break;
    }
}

inline void setMode(USART_TypeDef* uart, Mode mode)
{
    uart->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);

    switch(mode)
    {
        case Mode::RX:
            uart->CR1 |= USART_CR1_RE;
            break;

        case Mode::TX:
            uart->CR1 |= USART_CR1_TE;
            break;

        case Mode::TX_RX:
            uart->CR1 |= USART_CR1_TE | USART_CR1_RE;
            break;
    }
}

inline void setStopBits(USART_TypeDef* uart, StopBits stop)
{
	uart->CR2 &= ~USART_CR2_STOP;

    uart->CR2 |= (static_cast<uint32_t>(stop) << USART_CR2_STOP_Pos);
}

inline std::uint16_t read(USART_TypeDef* uart)
{
    return uart->DR;
}

inline void write(USART_TypeDef* uart, std::uint16_t data)
{
    uart->DR = data;
}

} // namespace uart::helper
