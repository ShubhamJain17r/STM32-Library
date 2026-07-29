#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace exti
{

constexpr IRQn_Type extiIRQ(std::uint8_t line)
{
    if(line <= 4)
        return static_cast<IRQn_Type>(EXTI0_IRQn + line);

    if(line <= 9)
        return EXTI9_5_IRQn;

    return EXTI15_10_IRQn;
}

} // namespace exti
