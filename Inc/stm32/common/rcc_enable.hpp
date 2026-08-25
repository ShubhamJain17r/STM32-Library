#pragma once

#include "stm32f446xx.h"
#include "stm32/common/registers.hpp"

namespace rcc
{

inline void enablePeripheralClock(GPIO_TypeDef* port) noexcept
{
	if(port == GPIOA) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; return; }
	if(port == GPIOB) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; return; }
	if(port == GPIOC) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; return; }
	if(port == GPIOD) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; return; }
	if(port == GPIOE) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; return; }
	if(port == GPIOF) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; return; }
	if(port == GPIOG) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; return; }
	if(port == GPIOH) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; return; }
	if(port == GPIOA) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); return; }
	if(port == GPIOB) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); return; }
	if(port == GPIOC) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); return; }
	if(port == GPIOD) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); return; }
	if(port == GPIOE) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); return; }
	if(port == GPIOF) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN); return; }
	if(port == GPIOG) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN); return; }
	if(port == GPIOH) { reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN); return; }
}

inline void enablePeripheralClock(USART_TypeDef* port) noexcept
{
	if(port == USART1) { RCC->APB2ENR |= RCC_APB2ENR_USART1EN; return; }
	if(port == USART2) { RCC->APB1ENR |= RCC_APB1ENR_USART2EN; return; }
	if(port == USART3) { RCC->APB1ENR |= RCC_APB1ENR_USART3EN; return; }
	if(port == UART4)  { RCC->APB1ENR |= RCC_APB1ENR_UART4EN;  return; }
	if(port == UART5)  { RCC->APB1ENR |= RCC_APB1ENR_UART5EN;  return; }
	if(port == USART6) { RCC->APB2ENR |= RCC_APB2ENR_USART6EN; return; }
	if(port == USART1) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_USART1EN); return; }
	if(port == USART2) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_USART2EN); return; }
	if(port == USART3) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_USART3EN); return; }
	if(port == UART4)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_UART4EN);  return; }
	if(port == UART5)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_UART5EN);  return; }
	if(port == USART6) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_USART6EN); return; }
}

inline void enablePeripheralClock(SYSCFG_TypeDef* port) noexcept
{
	if(port == SYSCFG) RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	if(port == SYSCFG) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
}

} // namespace rcc
