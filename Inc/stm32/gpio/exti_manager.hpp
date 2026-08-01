#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin.hpp"

#include "stm32/common/callback.hpp"

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

struct ExtiEntry
{
    GPIO_TypeDef* port;

    Callback rising = nullptr;
    Callback falling = nullptr;
};

class ExtiManager
{
public:
    static void setRisingCallback(const gpio::Pin&, Callback);
    static void setFallingCallback(const gpio::Pin&, Callback);

    static void handleInterrupt(std::uint8_t line);

private:
    inline static ExtiEntry table[16];
};

} // namespace exti
