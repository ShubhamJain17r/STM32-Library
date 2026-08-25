/**
 * @file interrupt_input.hpp
 * @brief High-level GPIO External Interrupt Input pin abstraction
 * 
 * Integrates GPIO, SYSCFG, and EXTI subsystems into a single RAII object:
 * - Deterministic trigger selection (Rising, Falling, Both)
 * - Automatic callback registration and clean RAII unregistration on destruction
 * - Edge-aware callbacks (void(*)(exti::Edge)) and software interrupt triggering
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/common/callback.hpp"
#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"
#include "stm32/exti/exti_types.hpp"

namespace gpio
{

struct InterruptInputConfig
{
    Pull               pull            = Pull::NONE;
    exti::Trigger      trigger         = exti::Trigger::Falling;
    stm32::Callback    callback        = nullptr;
    exti::EdgeCallback edgeCallback    = nullptr;
    stm32::Callback    risingCallback  = nullptr;
    stm32::Callback    fallingCallback = nullptr;
};

class InterruptInput
{
private:
    Pin pin_;

public:
    InterruptInput() = delete;
    ~InterruptInput();

    InterruptInput(const InterruptInput&) = delete;
    InterruptInput& operator=(const InterruptInput&) = delete;

    InterruptInput(InterruptInput&&) = delete;
    InterruptInput& operator=(InterruptInput&&) = delete;

    explicit InterruptInput(Pin pin, InterruptInputConfig config = {});
    InterruptInput(Pin pin, exti::Trigger trigger, stm32::Callback cb, Pull pull = Pull::NONE);

    /**
     * @brief Reads the immediate logic level of the pin via IDR.
     */
    inline PinState read() const noexcept
    {
        return reg::isAnyBitSet(pin_.port->IDR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
    }

    /**
     * @brief Returns true if the pin is currently logic HIGH.
     */
    inline bool isHigh() const noexcept
    {
        return (read() == PinState::HIGH);
    }

    /**
     * @brief Returns true if the pin is currently logic LOW.
     */
    inline bool isLow() const noexcept
    {
        return !isHigh();
    }

    /**
     * @brief Unmasks and enables the EXTI interrupt line for this pin.
     */
    void enable() const noexcept;

    /**
     * @brief Masks and disables the EXTI interrupt line for this pin.
     */
    void disable() const noexcept;

    /**
     * @brief Triggers a software interrupt on this EXTI line.
     */
    void softwareTrigger() const noexcept;
};

} // namespace gpio
