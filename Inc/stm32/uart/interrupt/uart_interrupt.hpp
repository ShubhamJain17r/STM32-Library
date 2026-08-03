#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/callback.hpp"

#include "stm32/uart/uart_traits.hpp"

namespace uart::interrupt
{

enum class Event : std::uint8_t
{
	TxEmpty,
	TxComplete,
	RxNotEmpty
};

inline void enableEvent(USART_TypeDef* uart, Event ev) noexcept
{
    switch(ev)
    {
        case Event::TxEmpty:
            uart->CR1 |= USART_CR1_TXEIE;
            break;

        case Event::TxComplete:
            uart->CR1 |= USART_CR1_TCIE;
            break;

        case Event::RxNotEmpty:
            uart->CR1 |= USART_CR1_RXNEIE;
            break;
    }
}

inline void disableEvent(USART_TypeDef* uart, Event ev) noexcept
{
    switch(ev)
    {
        case Event::TxEmpty:
            uart->CR1 &= ~USART_CR1_TXEIE;
            break;

        case Event::TxComplete:
            uart->CR1 &= ~USART_CR1_TCIE;
            break;

        case Event::RxNotEmpty:
            uart->CR1 &= ~USART_CR1_RXNEIE;
            break;
    }
}

inline void enableIRQ(IRQn_Type irq) noexcept
{
	NVIC_EnableIRQ(irq);
}

struct EventCallbacks
{
	Callback txEmpty = nullptr;
	Callback txComplete = nullptr;
	Callback rxNotEmpty = nullptr;
};

class UartEvent
{
public:
	static void setCallback(Instance, Event, Callback) noexcept;

	static void handleEvent(Instance) noexcept;

private:
	inline static EventCallbacks callbacks_[6];
};

} // namespace uart::interrupt
