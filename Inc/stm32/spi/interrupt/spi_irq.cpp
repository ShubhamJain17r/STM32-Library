/**
 * @file spi_irq.cpp
 * @brief Standard Cortex-M4 NVIC ISR Vector Handlers for STM32F446 SPI peripherals
 */

#include "stm32/spi/interrupt/spi_interrupt.hpp"

extern "C"
{

void SPI1_IRQHandler(void)
{
    spi::interrupt::SpiEvent::handleEvent(spi::Instance::spi1);
}

void SPI2_IRQHandler(void)
{
    spi::interrupt::SpiEvent::handleEvent(spi::Instance::spi2);
}

void SPI3_IRQHandler(void)
{
    spi::interrupt::SpiEvent::handleEvent(spi::Instance::spi3);
}

void SPI4_IRQHandler(void)
{
    spi::interrupt::SpiEvent::handleEvent(spi::Instance::spi4);
}

} // extern "C"

