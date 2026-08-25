/**
 * @file digital_output.hpp
 * @brief High-level GPIO Digital Output abstraction
 * 
 * Provides atomic, type-safe digital output pin control:
 * - Glitch-free initialization with configurable initial state, speed, pull, and type
 * - high(), low(), toggle(), write(PinState), and read() operations
 * - Zero-cost inline implementation using GPIO BSRR and ODR registers
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/gpio/pin.hpp"
#include "stm32/gpio/gpio_types.hpp"

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

    /**
     * @brief Writes a logic state to the pin (HIGH or LOW).
     */
    inline void write(PinState state) const noexcept
    {
        state == PinState::HIGH ? high() : low();
    }

    /**
     * @brief Sets the pin to logic HIGH atomically via BSRR.
     */
    inline void high() const noexcept
    {
        reg::write(pin_.port->BSRR, pin_.mask());
    }

    /**
     * @brief Clears the pin to logic LOW atomically via BSRR.
     */
    inline void low() const noexcept
    {
        reg::write(pin_.port->BSRR, pin_.mask() << 16u);
    }

    /**
     * @brief Toggles the current output state of the pin.
     */
    inline void toggle() const noexcept
    {
        reg::write(pin_.port->ODR, reg::read(pin_.port->ODR) ^ pin_.mask());
    }

    /**
     * @brief Reads the current output latch state from the ODR register.
     */
    inline PinState read() const noexcept
    {
        return reg::isAnyBitSet(pin_.port->ODR, pin_.mask()) ? PinState::HIGH : PinState::LOW;
    }
};

} // namespace gpio
