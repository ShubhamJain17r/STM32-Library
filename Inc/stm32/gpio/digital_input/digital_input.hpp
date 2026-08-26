/**
 * @file digital_input.hpp
 * @brief High-level GPIO Digital Input abstraction
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"

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

    inline PinState read() const noexcept
    {
        return reg::isAnyBitSet(pin_.port->IDR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
    }

    inline bool isHigh() const noexcept
    {
        return (read() == PinState::HIGH);
    }

    inline bool isLow() const noexcept
    {
        return !isHigh();
    }
};

} // namespace gpio

