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
	if(port == GPIOA)      reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
	else if(port == GPIOB) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
	else if(port == GPIOC) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
	else if(port == GPIOD) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
	else if(port == GPIOE) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);
	else if(port == GPIOF) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);
	else if(port == GPIOG) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);
	else if(port == GPIOH) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN);
	__DSB();
}

inline void enablePeripheralClock(USART_TypeDef* port) noexcept
{
	if(port == USART1)      reg::setBits(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
	else if(port == USART2) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
	else if(port == USART3) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_USART3EN);
	else if(port == UART4)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_UART4EN);
	else if(port == UART5)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_UART5EN);
	else if(port == USART6) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_USART6EN);
	__DSB();
}

inline void enablePeripheralClock(SYSCFG_TypeDef* port) noexcept
{
	if(port == SYSCFG) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	__DSB();
}

inline void enablePeripheralClock(TIM_TypeDef* timer) noexcept
{
	// APB1 Timers
	if(timer == TIM2)       reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
	else if(timer == TIM3)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
	else if(timer == TIM4)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);
	else if(timer == TIM5)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);
	else if(timer == TIM6)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);
	else if(timer == TIM7)  reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);
	else if(timer == TIM12) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);
	else if(timer == TIM13) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);
	else if(timer == TIM14) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);
	// APB2 Timers
	else if(timer == TIM1)  reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
	else if(timer == TIM8)  reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);
	else if(timer == TIM9)  reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM9EN);
	else if(timer == TIM10) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM10EN);
	else if(timer == TIM11) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_TIM11EN);
	__DSB();
}

inline void enablePeripheralClock(SPI_TypeDef* spi) noexcept
{
	if(spi == SPI1)      reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
	else if(spi == SPI2) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
	else if(spi == SPI3) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_SPI3EN);
	else if(spi == SPI4) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_SPI4EN);
	__DSB();
}

inline void enablePeripheralClock(I2C_TypeDef* i2c) noexcept
{
	if(i2c == I2C1)      reg::setBits(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
	else if(i2c == I2C2) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_I2C2EN);
	else if(i2c == I2C3) reg::setBits(RCC->APB1ENR, RCC_APB1ENR_I2C3EN);
	__DSB();
}

inline void enablePeripheralClock(ADC_TypeDef* adc) noexcept
{
	if(adc == ADC1)      reg::setBits(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
	else if(adc == ADC2) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);
	else if(adc == ADC3) reg::setBits(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);
	__DSB();
}

inline void enablePeripheralClock(DMA_TypeDef* dma) noexcept
{
	if(dma == DMA1)      reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN);
	else if(dma == DMA2) reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);
	__DSB();
}

} // namespace rcc
