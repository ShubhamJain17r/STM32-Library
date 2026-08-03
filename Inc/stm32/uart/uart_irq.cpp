#include "stm32/uart/uart_manager.hpp"

extern "C"
{

void USART1_IRQHandler()
{
	uart::interrupt::UartEvent::handleEvent(uart::Instance::usart1);
}

void USART2_IRQHandler()
{
	uart::interrupt::UartEvent::handleEvent(uart::Instance::usart2);
}

void USART3_IRQHandler()
{
	uart::interrupt::UartEvent::handleEvent(uart::Instance::usart3);
}

void UART4_IRQHandler()
{
	uart::interrupt::UartEvent::handleEvent(uart::Instance::uart4);
}

void UART5_IRQHandler()
{
	uart::interrupt::UartEvent::handleEvent(uart::Instance::uart5);
}

void USART6_IRQHandler()
{
	uart::interrupt::UartEvent::handleEvent(uart::Instance::usart6);
}

} // extern "C"
