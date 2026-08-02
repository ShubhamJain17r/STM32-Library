#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc.hpp"
#include "stm32/common/rcc_enable.hpp"

#include "stm32/gpio/af/af_traits.hpp"

namespace uart
{

enum class Instance
{
    Usart1,
    Usart2,
    Usart3,
    Uart4,
    Uart5,
    Usart6
};

template<Instance I>
struct Traits;

template<>
struct Traits<Instance::Usart1>
{
    static USART_TypeDef* peripheral() noexcept
    {
        return USART1;
    }

    static constexpr IRQn_Type irq = USART1_IRQn;

    static constexpr rcc::Bus bus = rcc::Bus::APB2;

    static constexpr gpio::af::Signal txSignal = gpio::af::Signal::USART1_TX;
    static constexpr gpio::af::Signal rxSignal = gpio::af::Signal::USART1_RX;

    static constexpr gpio::Pin defaultTx = gpio::af::Traits<txSignal>::pins[0];
    static constexpr gpio::Pin defaultRx = gpio::af::Traits<txSignal>::pins[0];

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(peripheral());
    }
};

template<>
struct Traits<Instance::Usart2>
{
    static USART_TypeDef* peripheral() noexcept
    {
        return USART2;
    }

    static constexpr IRQn_Type irq = USART2_IRQn;

    static constexpr rcc::Bus bus = rcc::Bus::APB1;

    static constexpr gpio::af::Signal txSignal = gpio::af::Signal::USART2_TX;
    static constexpr gpio::af::Signal rxSignal = gpio::af::Signal::USART2_RX;

    static constexpr gpio::Pin defaultTx = gpio::af::Traits<txSignal>::pins[0];
    static constexpr gpio::Pin defaultRx = gpio::af::Traits<txSignal>::pins[0];

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(peripheral());
    }
};

template<>
struct Traits<Instance::Usart3>
{
    static USART_TypeDef* peripheral() noexcept
    {
        return USART3;
    }

    static constexpr IRQn_Type irq = USART3_IRQn;

    static constexpr rcc::Bus bus = rcc::Bus::APB1;

    static constexpr gpio::af::Signal txSignal = gpio::af::Signal::USART3_TX;
    static constexpr gpio::af::Signal rxSignal = gpio::af::Signal::USART3_RX;

    static constexpr gpio::Pin defaultTx = gpio::af::Traits<txSignal>::pins[0];
    static constexpr gpio::Pin defaultRx = gpio::af::Traits<txSignal>::pins[0];

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(peripheral());
    }
};

template<>
struct Traits<Instance::Uart4>
{
    static USART_TypeDef* peripheral() noexcept
    {
        return UART4;
    }

    static constexpr IRQn_Type irq = UART4_IRQn;

    static constexpr rcc::Bus bus = rcc::Bus::APB1;

    static constexpr gpio::af::Signal txSignal = gpio::af::Signal::UART4_TX;
    static constexpr gpio::af::Signal rxSignal = gpio::af::Signal::UART4_RX;

    static constexpr gpio::Pin defaultTx = gpio::af::Traits<txSignal>::pins[0];
    static constexpr gpio::Pin defaultRx = gpio::af::Traits<txSignal>::pins[0];

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(peripheral());
    }
};

template<>
struct Traits<Instance::Uart5>
{
    static USART_TypeDef* peripheral() noexcept
    {
        return UART5;
    }

    static constexpr IRQn_Type irq = UART5_IRQn;

    static constexpr rcc::Bus bus = rcc::Bus::APB1;

    static constexpr gpio::af::Signal txSignal = gpio::af::Signal::UART5_TX;
    static constexpr gpio::af::Signal rxSignal = gpio::af::Signal::UART5_RX;

    static constexpr gpio::Pin defaultTx = gpio::af::Traits<txSignal>::pins[0];
    static constexpr gpio::Pin defaultRx = gpio::af::Traits<txSignal>::pins[0];

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(peripheral());
    }
};

template<>
struct Traits<Instance::Usart6>
{
    static USART_TypeDef* peripheral() noexcept
    {
        return USART6;
    }

    static constexpr IRQn_Type irq = USART6_IRQn;

    static constexpr rcc::Bus bus = rcc::Bus::APB2;

    static constexpr gpio::af::Signal txSignal = gpio::af::Signal::USART6_TX;
    static constexpr gpio::af::Signal rxSignal = gpio::af::Signal::USART6_RX;

    static constexpr gpio::Pin defaultTx = gpio::af::Traits<txSignal>::pins[0];
    static constexpr gpio::Pin defaultRx = gpio::af::Traits<txSignal>::pins[0];

    static void enableClock() noexcept
    {
        rcc::enablePeripheralClock(peripheral());
    }
};

} // namespace uart
