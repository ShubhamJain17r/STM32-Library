/**
 * @file i2c_irq.cpp
 * @brief Standard Cortex-M4 NVIC ISR Vector Handlers for STM32F446 I2C peripherals
 */

#include "stm32/i2c/interrupt/i2c_interrupt.hpp"

extern "C"
{

void I2C1_EV_IRQHandler(void)
{
    i2c::interrupt::I2cEvent::handleEvent(i2c::Instance::i2c1);
}

void I2C1_ER_IRQHandler(void)
{
    i2c::interrupt::I2cEvent::handleError(i2c::Instance::i2c1);
}

void I2C2_EV_IRQHandler(void)
{
    i2c::interrupt::I2cEvent::handleEvent(i2c::Instance::i2c2);
}

void I2C2_ER_IRQHandler(void)
{
    i2c::interrupt::I2cEvent::handleError(i2c::Instance::i2c2);
}

void I2C3_EV_IRQHandler(void)
{
    i2c::interrupt::I2cEvent::handleEvent(i2c::Instance::i2c3);
}

void I2C3_ER_IRQHandler(void)
{
    i2c::interrupt::I2cEvent::handleError(i2c::Instance::i2c3);
}

} // extern "C"

