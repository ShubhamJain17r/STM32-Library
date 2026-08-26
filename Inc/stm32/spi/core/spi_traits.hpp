/**
 * @file spi_traits.hpp
 * @brief Hardware traits and IRQ mappings for all 4 SPI peripherals on STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/pin_map.hpp"
#include "stm32/spi/core/spi_types.hpp"

namespace spi
{

template<Instance I>
struct Traits;

template<>
struct Traits<Instance::spi1>
{
    static constexpr SPI_TypeDef* peripheral() noexcept { return SPI1; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr IRQn_Type irq = SPI1_IRQn;

    static constexpr gpio::Pin defaultSck  = gpio::PA5;
    static constexpr gpio::Pin defaultMiso = gpio::PA6;
    static constexpr gpio::Pin defaultMosi = gpio::PA7;
    static constexpr gpio::Pin defaultNss  = gpio::PA4;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(SPI1);
    }
};

template<>
struct Traits<Instance::spi2>
{
    static constexpr SPI_TypeDef* peripheral() noexcept { return SPI2; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr IRQn_Type irq = SPI2_IRQn;

    static constexpr gpio::Pin defaultSck  = gpio::PB13;
    static constexpr gpio::Pin defaultMiso = gpio::PB14;
    static constexpr gpio::Pin defaultMosi = gpio::PB15;
    static constexpr gpio::Pin defaultNss  = gpio::PB12;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(SPI2);
    }
};

template<>
struct Traits<Instance::spi3>
{
    static constexpr SPI_TypeDef* peripheral() noexcept { return SPI3; }
    static constexpr rcc::Bus bus = rcc::Bus::APB1;
    static constexpr IRQn_Type irq = SPI3_IRQn;

    static constexpr gpio::Pin defaultSck  = gpio::PB3;
    static constexpr gpio::Pin defaultMiso = gpio::PB4;
    static constexpr gpio::Pin defaultMosi = gpio::PB5;
    static constexpr gpio::Pin defaultNss  = gpio::PA15;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(SPI3);
    }
};

template<>
struct Traits<Instance::spi4>
{
    static constexpr SPI_TypeDef* peripheral() noexcept { return SPI4; }
    static constexpr rcc::Bus bus = rcc::Bus::APB2;
    static constexpr IRQn_Type irq = SPI4_IRQn;

    static constexpr gpio::Pin defaultSck  = gpio::PE2;
    static constexpr gpio::Pin defaultMiso = gpio::PE5;
    static constexpr gpio::Pin defaultMosi = gpio::PE6;
    static constexpr gpio::Pin defaultNss  = gpio::PE4;

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(SPI4);
    }
};

} // namespace spi

