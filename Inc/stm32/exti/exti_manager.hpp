#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/callback/callback.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/exti/exti_types.hpp"

namespace exti::detail
{

constexpr IRQn_Type extiIRQ(std::uint8_t line) noexcept
{
    if(line <= 4)
        return static_cast<IRQn_Type>(EXTI0_IRQn + line);

    if(line <= 9)
        return EXTI9_5_IRQn;

    return EXTI15_10_IRQn;
}

} // namespace exti::detail

namespace exti
{

inline void configureExticr(const gpio::Pin& pin) noexcept
{
	const std::uint8_t index = pin.number / 4u;
	const std::uint8_t pos = (pin.number % 4u) * 4u;

	reg::modifyBits(SYSCFG->EXTICR[index], 0xFU << pos, static_cast<std::uint32_t>(pin.portIndex()) << pos);
}

inline void enableInterrupt(const gpio::Pin& pin) noexcept
{
	reg::setBits(EXTI->IMR, pin.mask());
}

inline void disableInterrupt(const gpio::Pin& pin) noexcept
{
	reg::clearBits(EXTI->IMR, pin.mask());
}

inline void enableEvent(const gpio::Pin& pin) noexcept
{
	reg::setBits(EXTI->EMR, pin.mask());
}

inline void disableEvent(const gpio::Pin& pin) noexcept
{
	reg::clearBits(EXTI->EMR, pin.mask());
}

inline void enableRisingTrigger(const gpio::Pin& pin) noexcept
{
	reg::setBits(EXTI->RTSR, pin.mask());
}

inline void disableRisingTrigger(const gpio::Pin& pin) noexcept
{
	reg::clearBits(EXTI->RTSR, pin.mask());
}

inline void enableFallingTrigger(const gpio::Pin& pin) noexcept
{
	reg::setBits(EXTI->FTSR, pin.mask());
}

inline void disableFallingTrigger(const gpio::Pin& pin) noexcept
{
	reg::clearBits(EXTI->FTSR, pin.mask());
}

inline void setTrigger(const gpio::Pin& pin, Trigger trigger) noexcept
{
    switch(trigger)
    {
        case Trigger::None:
            disableRisingTrigger(pin);
            disableFallingTrigger(pin);
            break;
        case Trigger::Rising:
            enableRisingTrigger(pin);
            disableFallingTrigger(pin);
            break;
        case Trigger::Falling:
            disableRisingTrigger(pin);
            enableFallingTrigger(pin);
            break;
        case Trigger::Both:
            enableRisingTrigger(pin);
            enableFallingTrigger(pin);
            break;
    }
}

inline void clearPending(std::uint8_t line) noexcept
{
	reg::write(EXTI->PR, 1U << line);
}

inline bool isPending(std::uint8_t line) noexcept
{
    return reg::isAnyBitSet(EXTI->PR, 1U << line);
}

inline void softwareTrigger(std::uint8_t line) noexcept
{
    reg::setBits(EXTI->SWIER, 1U << line);
}

inline void enableIRQ(const gpio::Pin& pin) noexcept
{
	NVIC_EnableIRQ(detail::extiIRQ(pin.number));
}

inline void disableIRQ(const gpio::Pin& pin) noexcept
{
	NVIC_DisableIRQ(detail::extiIRQ(pin.number));
}

struct ExtiEntry
{
    GPIO_TypeDef*   port            = nullptr;
    Trigger         trigger         = Trigger::None;
    stm32::Callback callback        = nullptr;
    EdgeCallback    edgeCallback    = nullptr;
    stm32::Callback risingCallback  = nullptr;
    stm32::Callback fallingCallback = nullptr;
};

class ExtiManager
{
public:
    static bool registerPin(const gpio::Pin& pin, Trigger trigger, stm32::Callback cb = nullptr, EdgeCallback edgeCb = nullptr) noexcept;
    static void unregisterPin(const gpio::Pin& pin) noexcept;

    static void setCallback(const gpio::Pin& pin, stm32::Callback cb) noexcept;
    static void setEdgeCallback(const gpio::Pin& pin, EdgeCallback cb) noexcept;
    static void setRisingCallback(const gpio::Pin& pin, stm32::Callback cb) noexcept;
    static void setFallingCallback(const gpio::Pin& pin, stm32::Callback cb) noexcept;

    static void handleInterrupt(std::uint8_t line) noexcept;

private:
    inline static ExtiEntry table[16];
};

} // namespace exti
