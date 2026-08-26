/**
 * @file rcc_enable.hpp
 * @brief Peripheral clock enable overloads for STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include "stm32/common/registers/registers.hpp"

namespace rcc
{

inline void enablePeripheralClock(GPIO_TypeDef* port) noexcept
{
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
	if(port == USART1) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_USART1EN); return; }
	if(port == USART2) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_USART2EN); return; }
	if(port == USART3) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_USART3EN); return; }
	if(port == UART4)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_UART4EN);  return; }
	if(port == UART5)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_UART5EN);  return; }
	if(port == USART6) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_USART6EN); return; }
}

inline void enablePeripheralClock(SYSCFG_TypeDef* port) noexcept
{
	if(port == SYSCFG) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
}

inline void enablePeripheralClock(TIM_TypeDef* timer) noexcept
{
	// APB1 Timers
	if(timer == TIM2)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);  return; }
	if(timer == TIM3)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);  return; }
	if(timer == TIM4)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);  return; }
	if(timer == TIM5)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);  return; }
	if(timer == TIM6)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);  return; }
	if(timer == TIM7)  { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);  return; }
	if(timer == TIM12) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM12EN); return; }
	if(timer == TIM13) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM13EN); return; }
	if(timer == TIM14) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM14EN); return; }
	// APB2 Timers
	if(timer == TIM1)  { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);  return; }
	if(timer == TIM8)  { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);  return; }
	if(timer == TIM9)  { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM9EN);  return; }
	if(timer == TIM10) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM10EN); return; }
	if(timer == TIM11) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM11EN); return; }
}

inline void enablePeripheralClock(SPI_TypeDef* spi) noexcept
{
	if(spi == SPI1) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); return; }
	if(spi == SPI2) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_SPI2EN); return; }
	if(spi == SPI3) { reg::setBits(RCC->APB1ENR, RCC_APB1ENR_SPI3EN); return; }
	if(spi == SPI4) { reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SPI4EN); return; }
}

} // namespace rcc
