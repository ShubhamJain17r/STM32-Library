#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace rcc
{

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

} // namespace rcc
