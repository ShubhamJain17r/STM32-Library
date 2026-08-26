/**
 * @file uart_helper.hpp
 * @brief Low-level register manipulation helper functions for USART
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/uart/core/uart_types.hpp"

namespace uart::helper
{

inline bool txReady(const USART_TypeDef* uart) noexcept
{
	return reg::isAnyBitSet(uart->SR, USART_SR_TXE);
}

inline bool transmissionComplete(const USART_TypeDef* uart) noexcept
{
	return reg::isAnyBitSet(uart->SR, USART_SR_TC);
}

inline bool rxReady(const USART_TypeDef* uart) noexcept
{
	return reg::isAnyBitSet(uart->SR, USART_SR_RXNE);
}

inline void configureBaudRate(USART_TypeDef* uart, std::uint32_t baud, rcc::Bus bus) noexcept
{
	const std::uint32_t freq = rcc::frequency(bus);
	const bool over8 = reg::isAnyBitSet(uart->CR1, USART_CR1_OVER8);

	if(over8)
	{
		const std::uint32_t usartdiv = ((freq * 2u) + baud) / (baud * 2u);
		const std::uint32_t mantissa = (usartdiv >> 4u);
		const std::uint32_t fraction = (usartdiv & 0xFu) >> 1u;
		reg::write(uart->BRR, (mantissa << 4u) | fraction);
	}
	else
	{
		const std::uint32_t usartdiv = (freq + (baud / 2u)) / baud;
		reg::write(uart->BRR, usartdiv);
	}
}

inline void setOversampling(USART_TypeDef* uart, Oversampling over) noexcept
{
	if(over == Oversampling::BY8)
	{
		reg::setBits(uart->CR1, USART_CR1_OVER8);
	}
	else
	{
		reg::clearBits(uart->CR1, USART_CR1_OVER8);
	}
}

inline void enable(USART_TypeDef* uart) noexcept
{
	reg::setBits(uart->CR1, USART_CR1_UE);
}

inline void disable(USART_TypeDef* uart) noexcept
{
	reg::clearBits(uart->CR1, USART_CR1_UE);
}

inline void setWordLength(USART_TypeDef* uart, WordLength len) noexcept
{
	if(len == WordLength::BITS_9)
	{
		reg::setBits(uart->CR1, USART_CR1_M);
	}
	else
	{
		reg::clearBits(uart->CR1, USART_CR1_M);
	}
}

inline void setParity(USART_TypeDef* uart, Parity parity) noexcept
{
    switch(parity)
    {
        case Parity::NONE:
            reg::clearBits(uart->CR1, USART_CR1_PCE | USART_CR1_PS);
            break;

        case Parity::EVEN:
            reg::modifyBits(uart->CR1, USART_CR1_PS, USART_CR1_PCE);
            break;

        case Parity::ODD:
            reg::setBits(uart->CR1, USART_CR1_PCE | USART_CR1_PS);
            break;
    }
}

inline void setMode(USART_TypeDef* uart, Mode mode) noexcept
{
    switch(mode)
    {
        case Mode::RX:
            reg::modifyBits(uart->CR1, USART_CR1_TE, USART_CR1_RE);
            break;

        case Mode::TX:
            reg::modifyBits(uart->CR1, USART_CR1_RE, USART_CR1_TE);
            break;

        case Mode::TX_RX:
            reg::setBits(uart->CR1, USART_CR1_TE | USART_CR1_RE);
            break;
    }
}

inline void setStopBits(USART_TypeDef* uart, StopBits stop) noexcept
{
    reg::modifyBits(uart->CR2, USART_CR2_STOP, static_cast<std::uint32_t>(stop) << USART_CR2_STOP_Pos);
}

inline std::uint16_t read(USART_TypeDef* uart) noexcept
{
	while(!rxReady(uart));

    return static_cast<std::uint16_t>(reg::read(uart->DR));
}

inline void write(USART_TypeDef* uart, std::uint16_t data) noexcept
{
	while(!txReady(uart));

    reg::write(uart->DR, data);
}

} // namespace uart::helper

