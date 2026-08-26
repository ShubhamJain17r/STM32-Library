#include "stm32/common/registers/registers.hpp"
#include "stm32/uart/interrupt/uart_interrupt.hpp"

namespace uart::interrupt
{

static constexpr std::size_t index(Instance I)
{
	return static_cast<std::size_t>(I);
}

static USART_TypeDef* getPeripheral(Instance I) noexcept
{
	switch(I)
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

static void handleInstance(Instance I, USART_TypeDef* uart, const EventCallbacks& dev, const EventCallbacks& user)
{
	(void)I;
	const std::uint32_t status   = reg::read(uart->SR);
	const std::uint32_t control1 = reg::read(uart->CR1);

	// 1. RXNE (Read data register not empty)
	if((status & USART_SR_RXNE) && (control1 & USART_CR1_RXNEIE))
	{
		if(dev.rxNotEmpty)
		{
			dev.rxNotEmpty();
		}
	}

	// 2. TXE (Transmit data register empty)
	if((status & USART_SR_TXE) && (control1 & USART_CR1_TXEIE))
	{
		if(dev.txEmpty)
		{
			dev.txEmpty();
		}
	}

	// 3. TC (Transmission complete)
	if((status & USART_SR_TC) && (control1 & USART_CR1_TCIE))
	{
		if(user.txComplete)
		{
			user.txComplete();
		}
	}

	// 4. IDLE (Idle line detected)
	if((status & USART_SR_IDLE) && (control1 & USART_CR1_IDLEIE))
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

void UartEvent::handleEvent(Instance I)
{
	auto* uart = getPeripheral(I);
	if(!uart) return;

	handleInstance(I,
	               uart,
	               developerCallbacks_[index(I)],
	               userCallbacks_[index(I)]);
}

} // namespace uart::interrupt
