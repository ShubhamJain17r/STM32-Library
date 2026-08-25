/**
 * @file pin.hpp
 * @brief Value-type representation of an STM32 physical GPIO Pin
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc_enable.hpp"

namespace gpio::detail
{

constexpr std::uint8_t gpioPortIndex(const GPIO_TypeDef* port) noexcept
{
    if(port == GPIOA) return 0;
    if(port == GPIOB) return 1;
    if(port == GPIOC) return 2;
    if(port == GPIOD) return 3;
    if(port == GPIOE) return 4;
    if(port == GPIOF) return 5;
    if(port == GPIOG) return 6;
    if(port == GPIOH) return 7;

    return 0xFF;
}

} // namespace gpio::detail

namespace gpio
{

/**
 * @brief Represents a physical GPIO pin by binding its port pointer and pin index [0..15].
 */
struct Pin
{
    GPIO_TypeDef* port;
    std::uint8_t number;

    /**
     * @brief Returns bitmask for the pin (1 << number).
     */
    constexpr std::uint32_t mask() const noexcept
    {
        return 1U << number;
    }

    /**
     * @brief Returns 0-based port index (0 for GPIOA, 1 for GPIOB, etc.).
     */
    constexpr std::uint8_t portIndex() const noexcept
    {
        return detail::gpioPortIndex(port);
    }

    /**
     * @brief Returns unique 8-bit index encoding (portIndex << 4 | number).
     */
    constexpr std::uint8_t index() const noexcept
    {
        return (portIndex() << 4) | number;
    }

    /**
     * @brief Enables the AHB1 peripheral clock for this pin's GPIO port.
     */
    inline void enableClock() const noexcept
    {
        rcc::enablePeripheralClock(port);
    }

    constexpr bool operator==(const Pin& rhs) const noexcept
    {
        return port == rhs.port && number == rhs.number;
    }
};

} // namespace gpio
