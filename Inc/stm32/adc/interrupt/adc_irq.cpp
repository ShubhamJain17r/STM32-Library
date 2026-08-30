/**
 * @file adc_irq.cpp
 * @brief Centralized NVIC ISR Vector Handler for STM32F446 ADC peripherals (ADC1, ADC2, ADC3)
 */

#include "stm32/adc/interrupt/adc_interrupt.hpp"

extern "C"
{

void ADC_IRQHandler(void)
{
    // Dispatch to all 3 ADC instances if active
    adc::interrupt::AdcEvent::handleInterrupt(adc::Instance::adc1);
    adc::interrupt::AdcEvent::handleInterrupt(adc::Instance::adc2);
    adc::interrupt::AdcEvent::handleInterrupt(adc::Instance::adc3);
}

} // extern "C"

