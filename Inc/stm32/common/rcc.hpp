#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace rcc
{

inline constexpr std::uint32_t HSI_FREQUENCY = 16'000'000U;

inline constexpr std::uint32_t LSI_FREQUENCY = 32'000U;

inline std::uint32_t systemClock = HSI_FREQUENCY;
inline std::uint32_t ahbClock    = HSI_FREQUENCY;
inline std::uint32_t apb1Clock   = HSI_FREQUENCY;
inline std::uint32_t apb2Clock   = HSI_FREQUENCY;


inline void enableGpioClock(GPIO_TypeDef* port) noexcept
{
	if(port == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	if(port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	if(port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	if(port == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	if(port == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	if(port == GPIOF) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	if(port == GPIOG) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	if(port == GPIOH) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
}

inline void enableSyscfgClock() noexcept
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

inline void enableUartClock(USART_TypeDef* uart)
{
	if(uart == USART1) RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	if(uart == USART2) RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	if(uart == USART3) RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	if(uart == UART4) RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
	if(uart == UART5) RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
	if(uart == USART6) RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

} // namespace rcc
