/**
 * @file i2c_config.hpp
 * @brief Configuration structures and default settings for I2C peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/core/pin.hpp"
#include "stm32/i2c/core/i2c_types.hpp"
#include "stm32/i2c/core/i2c_traits.hpp"

namespace i2c
{

template<Instance I>
struct I2cConfig
{
    gpio::Pin scl = Traits<I>::defaultScl;
    gpio::Pin sda = Traits<I>::defaultSda;

    std::uint32_t speedHz = 100'000; // 100 kHz Standard Mode
    DutyCycle dutyCycle = DutyCycle::Duty2_1;
    bool enableInternalPullup = true;
    std::uint8_t ownAddress = 0x00;

    constexpr I2cConfig() = default;

    constexpr explicit I2cConfig(std::uint32_t speed)
        : speedHz(speed)
    {}

    constexpr I2cConfig(gpio::Pin sclPin, gpio::Pin sdaPin, std::uint32_t speed = 100'000)
        : scl(sclPin), sda(sdaPin), speedHz(speed)
    {}
};

} // namespace i2c

