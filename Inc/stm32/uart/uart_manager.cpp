#include "stm32/uart/uart_manager.hpp"

namespace uart::interrupt
{

static constexpr std::size_t index(Instance I)
{
	return static_cast<std::size_t>(I);
}

inline USART_TypeDef* peripheral(Instance instance)
{
    switch(instance)
    {
        case Instance::usart1: return USART1;
        case Instance::usart2: return USART2;
        case Instance::usart3: return USART3;
        case Instance::uart4:  return UART4;
        case Instance::uart5:  return UART5;
        case Instance::usart6: return USART6;
    }

    return nullptr;
}

void UartEvent::setCallback(Instance I, Event intr, Callback cb) noexcept
{
	switch(intr)
	{
		case Event::TxEmpty:
			callbacks_[index(I)].txEmpty = cb;
			break;
		case Event::TxComplete:
			callbacks_[index(I)].txComplete = cb;
			break;
		case Event::RxNotEmpty:
			callbacks_[index(I)].rxNotEmpty = cb;
			break;
	}
}

void UartEvent::handleEvent(Instance I) noexcept
{
	USART_TypeDef* uart = peripheral(I);

	EventCallbacks& cb = callbacks_[index(I)];

	if((uart->SR & USART_SR_RXNE) && (uart->CR1 & USART_CR1_RXNEIE))
	{
		if(cb.rxNotEmpty)
		{
			cb.rxNotEmpty();
		}
	}

	if((uart->SR & USART_SR_TXE) && (uart->CR1 & USART_CR1_TXEIE))
	{
		if(cb.txEmpty)
		{
			cb.txEmpty();
		}
	}

	if((uart->SR & USART_SR_TC) && (uart->CR1 & USART_CR1_TCIE))
	{
		if(cb.txComplete)
		{
			cb.txComplete();
		}
	}
}

}
