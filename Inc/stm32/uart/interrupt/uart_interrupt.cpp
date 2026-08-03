#include <stm32/uart/interrupt/uart_interrupt.hpp>

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

void UartEvent::setUserCallback(Instance I, Event intr, Callback cb) noexcept
{
	switch(intr)
	{
		case Event::TxEmpty:
			userCallbacks_[index(I)].txEmpty = cb;
			break;
		case Event::TxComplete:
			userCallbacks_[index(I)].txComplete = cb;
			break;
		case Event::RxNotEmpty:
			userCallbacks_[index(I)].rxNotEmpty = cb;
			break;
	}
}

void UartEvent::setDeveloperCallback(Instance I, Event intr, Callback cb) noexcept
{
	switch(intr)
	{
		case Event::TxEmpty:
			developerCallbacks_[index(I)].txEmpty = cb;
			break;
		case Event::TxComplete:
			developerCallbacks_[index(I)].txComplete = cb;
			break;
		case Event::RxNotEmpty:
			developerCallbacks_[index(I)].rxNotEmpty = cb;
			break;
	}
}

void UartEvent::handleEvent(Instance I) noexcept
{
	USART_TypeDef* uart = peripheral(I);

	EventCallbacks& dev = developerCallbacks_[index(I)];
	EventCallbacks& user = userCallbacks_[index(I)];

	if((uart->SR & USART_SR_RXNE) && (uart->CR1 & USART_CR1_RXNEIE))
	{
		if(dev.rxNotEmpty)
		{
			dev.rxNotEmpty();
		}
		if(user.rxNotEmpty)
		{
			user.rxNotEmpty();
		}
	}

	if((uart->SR & USART_SR_TXE) && (uart->CR1 & USART_CR1_TXEIE))
	{
		if(dev.txEmpty)
		{
			dev.txEmpty();
		}
		if(user.txEmpty)
		{
			user.txEmpty();
		}
	}

	if((uart->SR & USART_SR_TC) && (uart->CR1 & USART_CR1_TCIE))
	{
		if(dev.txComplete)
		{
			dev.txComplete();
		}
		if(user.txComplete)
		{
			user.txComplete();
		}
	}
}

} // namespace uart::interrupt
