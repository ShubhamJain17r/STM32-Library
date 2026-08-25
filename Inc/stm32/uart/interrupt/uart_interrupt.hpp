#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/common/callback.hpp"

#include "stm32/uart/uart_traits.hpp"

namespace uart::interrupt
{

enum class Event : std::uint8_t
{
	TxEmpty,
	TxComplete,
	RxNotEmpty,
	IdleState
};

inline void enableEvent(USART_TypeDef* uart, Event ev) noexcept
{
    switch(ev)
    {
        case Event::TxEmpty:
            uart->CR1 |= USART_CR1_TXEIE;
            reg::setBits(uart->CR1, USART_CR1_TXEIE);
            break;

        case Event::TxComplete:
            uart->CR1 |= USART_CR1_TCIE;
            reg::setBits(uart->CR1, USART_CR1_TCIE);
            break;

        case Event::RxNotEmpty:
            uart->CR1 |= USART_CR1_RXNEIE;
            reg::setBits(uart->CR1, USART_CR1_RXNEIE);
            break;

        case Event::IdleState:
            uart->CR1 |= USART_CR1_IDLEIE;
            reg::setBits(uart->CR1, USART_CR1_IDLEIE);
            break;
    }
}

inline void disableEvent(USART_TypeDef* uart, Event ev) noexcept
{
    switch(ev)
    {
        case Event::TxEmpty:
            uart->CR1 &= ~USART_CR1_TXEIE;
            reg::clearBits(uart->CR1, USART_CR1_TXEIE);
            break;

        case Event::TxComplete:
            uart->CR1 &= ~USART_CR1_TCIE;
            reg::clearBits(uart->CR1, USART_CR1_TCIE);
            break;

        case Event::RxNotEmpty:
            uart->CR1 &= ~USART_CR1_RXNEIE;
            reg::clearBits(uart->CR1, USART_CR1_RXNEIE);
            break;

        case Event::IdleState:
            uart->CR1 &= ~USART_CR1_IDLEIE;
            reg::clearBits(uart->CR1, USART_CR1_IDLEIE);
            break;
    }
}

inline void enableIRQ(IRQn_Type irq) noexcept
{
	NVIC_EnableIRQ(irq);
}

struct EventCallbacks
{
	stm32::Callback txEmpty    = nullptr;
	stm32::Callback txComplete = nullptr;
	stm32::Callback rxNotEmpty = nullptr;
	stm32::Callback idleState  = nullptr;
};

class UartEvent
{
public:
	static void setDeveloperCallback(Instance, Event, stm32::Callback) noexcept;
	static void setUserCallback(Instance, Event, stm32::Callback) noexcept;

	static void handleEvent(Instance) noexcept;

private:
	inline static EventCallbacks developerCallbacks_[6];
	inline static EventCallbacks userCallbacks_[6];
};

} // namespace uart::interrupt
