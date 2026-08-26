/**
 * @file pin.hpp
 * @brief Value-type representation of an STM32 physical GPIO Pin
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc_enable.hpp"

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

struct Pin
{
    GPIO_TypeDef* port;
    std::uint8_t number;

    constexpr std::uint32_t mask() const noexcept
    {
        return 1U << number;
    }

    constexpr std::uint8_t portIndex() const noexcept
    {
        return detail::gpioPortIndex(port);
    }

    constexpr std::uint8_t index() const noexcept
    {
        return (portIndex() << 4) | number;
    }

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

