/**
 * @file timer_irq.cpp
 * @brief Standard Cortex-M4 NVIC ISR Vector Handlers for STM32F446 Timers
 * 
 * Routes update, break, trigger, and capture/compare interrupts for all 14 hardware
 * timers (TIM1 through TIM14) to the centralized TimerEvent dispatcher.
 */

#include "stm32/timer/interrupt/timer_interrupt.hpp"

extern "C"
{

void TIM1_UP_TIM10_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim1);
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim10);
}

void TIM1_CC_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim1);
}

void TIM1_BRK_TIM9_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim9);
}

void TIM1_TRG_COM_TIM11_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim11);
}

void TIM2_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim2);
}

void TIM3_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim3);
}

void TIM4_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim4);
}

void TIM5_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim5);
}

void TIM6_DAC_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim6);
}

void TIM7_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim7);
}

void TIM8_UP_TIM13_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim8);
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim13);
}

void TIM8_CC_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim8);
}

void TIM8_BRK_TIM12_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim12);
}

void TIM8_TRG_COM_TIM14_IRQHandler()
{
    timer::interrupt::TimerEvent::handleEvent(timer::Instance::tim14);
}

} // extern "C"
