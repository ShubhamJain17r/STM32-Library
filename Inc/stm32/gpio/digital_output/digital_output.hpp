/**
 * @file digital_output.hpp
 * @brief High-level GPIO Digital Output abstraction
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"

namespace gpio
{

struct DigitalOutputConfig
{
    OutputType  outputType   = OutputType::PUSH_PULL;
    OutputSpeed outputSpeed  = OutputSpeed::LOW;
    Pull        pull         = Pull::NONE;
    PinState    initialState = PinState::LOW;
};

class DigitalOutput
{
private:
    Pin pin_;

public:
    DigitalOutput() = delete;
    ~DigitalOutput() = default;

    DigitalOutput(const DigitalOutput&) = delete;
    DigitalOutput& operator=(const DigitalOutput&) = delete;

    DigitalOutput(DigitalOutput&&) = delete;
    DigitalOutput& operator=(DigitalOutput&&) = delete;

    explicit DigitalOutput(Pin pin, DigitalOutputConfig config = {});

    inline void write(PinState state) const noexcept
    {
        state == PinState::HIGH ? high() : low();
    }

    inline void high() const noexcept
    {
        reg::write(pin_.port->BSRR, pin_.mask());
    }

    inline void low() const noexcept
    {
        reg::write(pin_.port->BSRR, pin_.mask() << 16u);
    }

    inline void toggle() const noexcept
    {
        reg::write(pin_.port->ODR, reg::read(pin_.port->ODR) ^ pin_.mask());
    }

    inline PinState read() const noexcept
    {
        return reg::isAnyBitSet(pin_.port->ODR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
    }
};

} // namespace gpio

