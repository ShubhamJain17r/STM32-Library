#pragma once

#include "stm32f446xx.h"
#include <cstdint>

struct Pin
{
    GPIO_TypeDef *port;
    std::uint8_t pinNumber;
    IRQn_Type irqn;

    void (*enableClock)();
};

void enableGPIOA()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

constexpr Pin PA0
{
    GPIOA,
    0,
    EXTI0_IRQn,
    enableGPIOA
};
