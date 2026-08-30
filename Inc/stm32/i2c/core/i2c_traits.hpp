/**
 * @file i2c_traits.hpp
 * @brief Hardware traits and IRQ mappings for all 3 I2C peripherals on STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/pin_map.hpp"
#include "stm32/i2c/core/i2c_types.hpp"

namespace i2c
{

template<Instance I>
struct Traits;

template<>
struct Traits<Instance::i2c1>
{
    static constexpr I2C_TypeDef* peripheral() noexcept { return I2C1; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr IRQn_Type eventIrq = I2C1_EV_IRQn;
    static constexpr IRQn_Type errorIrq = I2C1_ER_IRQn;

    static constexpr gpio::Pin defaultScl = gpio::PB8;
    static constexpr gpio::Pin defaultSda = gpio::PB9;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(I2C1);
    }
};

template<>
struct Traits<Instance::i2c2>
{
    static constexpr I2C_TypeDef* peripheral() noexcept { return I2C2; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr IRQn_Type eventIrq = I2C2_EV_IRQn;
    static constexpr IRQn_Type errorIrq = I2C2_ER_IRQn;

    static constexpr gpio::Pin defaultScl = gpio::PB10;
    static constexpr gpio::Pin defaultSda = gpio::PB11;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(I2C2);
    }
};

template<>
struct Traits<Instance::i2c3>
{
    static constexpr I2C_TypeDef* peripheral() noexcept { return I2C3; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr IRQn_Type eventIrq = I2C3_EV_IRQn;
    static constexpr IRQn_Type errorIrq = I2C3_ER_IRQn;

    static constexpr gpio::Pin defaultScl = gpio::PA8;
    static constexpr gpio::Pin defaultSda = gpio::PB4;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(I2C3);
    }
};

} // namespace i2c

