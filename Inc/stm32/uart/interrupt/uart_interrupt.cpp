#include "stm32/common/registers.hpp"
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

void UartEvent::setUserCallback(Instance I, Event intr, stm32::Callback cb) noexcept
{
	switch(intr)
	{
		case Event::TxComplete:
			userCallbacks_[index(I)].txComplete = cb;
			break;
		case Event::IdleState:
			userCallbacks_[index(I)].idleState = cb;
			break;
		default:
			break;
	}
}

void UartEvent::setDeveloperCallback(Instance I, Event intr, stm32::Callback cb) noexcept
{
	switch(intr)
	{
		case Event::TxEmpty:
			developerCallbacks_[index(I)].txEmpty = cb;
			break;
		case Event::RxNotEmpty:
			developerCallbacks_[index(I)].rxNotEmpty = cb;
			break;
		case Event::IdleState:
			developerCallbacks_[index(I)].idleState = cb;
			break;
		default:
			break;
	}
}

void UartEvent::handleEvent(Instance I) noexcept
{
	USART_TypeDef* uart = peripheral(I);
	if(!uart)
	{
		return;
	}

	EventCallbacks& dev = developerCallbacks_[index(I)];
	EventCallbacks& user = userCallbacks_[index(I)];

	if(reg::isAnyBitSet(uart->SR, USART_SR_RXNE) && reg::isAnyBitSet(uart->CR1, USART_CR1_RXNEIE))
	{
		if(dev.rxNotEmpty)
		{
			dev.rxNotEmpty();
		}
	}

	if(reg::isAnyBitSet(uart->SR, USART_SR_TXE) && reg::isAnyBitSet(uart->CR1, USART_CR1_TXEIE))
	{
		if(dev.txEmpty)
		{
			dev.txEmpty();
		}
	}

	if(reg::isAnyBitSet(uart->SR, USART_SR_TC) && reg::isAnyBitSet(uart->CR1, USART_CR1_TCIE))
	{
		if(user.txComplete)
		{
			user.txComplete();
		}
	}

	if(reg::isAnyBitSet(uart->SR, USART_SR_IDLE) && reg::isAnyBitSet(uart->CR1, USART_CR1_IDLEIE))
	{
		if(dev.idleState)
		{
			dev.idleState();
		}
		if(user.idleState)
		{
			user.idleState();
		}
	}
}

} // namespace uart::interrupt
