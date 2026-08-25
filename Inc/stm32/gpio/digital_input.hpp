/**
 * @file digital_input.hpp
 * @brief High-level GPIO Digital Input abstraction
 * 
 * Provides:
 * - Direct digital reading of pin state (HIGH / LOW)
 * - Configurable internal pull resistors (NONE, PULL_UP, PULL_DOWN)
 * - Convenient boolean helpers: isHigh(), isLow()
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio
{

class DigitalInput
{
private:
    Pin pin_;

public:
    DigitalInput() = delete;
    ~DigitalInput() = default;

    DigitalInput(const DigitalInput&) = delete;
    DigitalInput& operator=(const DigitalInput&) = delete;

    DigitalInput(DigitalInput&&) = delete;
    DigitalInput& operator=(DigitalInput&&) = delete;

    explicit DigitalInput(Pin pin, Pull pull = Pull::NONE);

    /**
     * @brief Reads the current logic level of the input pin via IDR.
     */
    inline PinState read() const noexcept
    {
        return reg::isAnyBitSet(pin_.port->IDR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
    }

    /**
     * @brief Returns true if the input pin is at logic HIGH.
     */
    inline bool isHigh() const noexcept
    {
        return (read() == PinState::HIGH);
    }

    /**
     * @brief Returns true if the input pin is at logic LOW.
     */
    inline bool isLow() const noexcept
    {
        return !isHigh();
    }
};

} // namespace gpio
