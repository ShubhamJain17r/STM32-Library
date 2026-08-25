#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin.hpp"

#include "stm32/common/callback.hpp"

namespace
{

constexpr IRQn_Type extiIRQ(std::uint8_t line)
{
    if(line <= 4)
        return static_cast<IRQn_Type>(EXTI0_IRQn + line);

    if(line <= 9)
        return EXTI9_5_IRQn;

    return EXTI15_10_IRQn;
}

} // namespace

namespace exti
{

inline void configureExticr(const gpio::Pin& pin) noexcept
{
	std::uint8_t index = pin.number / 4;
	std::uint16_t pos = (pin.number % 4) * 4;

	SYSCFG->EXTICR[index] &= ~(0xF << pos);
	SYSCFG->EXTICR[index] |=  (pin.portIndex() << pos);
}

inline void enableInterrupt(const gpio::Pin& pin) noexcept
{
	EXTI->IMR |= pin.mask();
}

inline void disableInterrupt(const gpio::Pin& pin) noexcept
{
	EXTI->IMR &= ~pin.mask();
}

inline void enableRisingTrigger(const gpio::Pin& pin) noexcept
{
	EXTI->RTSR |= pin.mask();
}

inline void enableFallingTrigger(const gpio::Pin& pin) noexcept
{
	EXTI->FTSR |= pin.mask();
}

inline void clearPending(std::uint8_t line) noexcept
{
	EXTI->PR = (1u << line);
}

inline void enableIRQ(const gpio::Pin& pin) noexcept
{
	NVIC_EnableIRQ(extiIRQ(pin.number));
}

struct ExtiEntry
{
    GPIO_TypeDef* port;

    stm32::Callback rising  = nullptr;
    stm32::Callback falling = nullptr;
};

class ExtiManager
{
public:
    static void setRisingCallback(const gpio::Pin&, stm32::Callback);
    static void setFallingCallback(const gpio::Pin&, stm32::Callback);

    static void handleInterrupt(std::uint8_t line);

private:
    inline static ExtiEntry table[16];
};

} // namespace exti
