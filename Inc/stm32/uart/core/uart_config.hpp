/**
 * @file uart_config.hpp
 * @brief Configuration structures and defaults for UART / USART peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/uart/core/uart_traits.hpp"
#include "stm32/uart/core/uart_types.hpp"
#include "stm32/gpio/af/af_config.hpp"

namespace uart
{

template<Instance I>
struct uartConfig
{
    union {
        gpio::Pin tx;
        gpio::Pin txPin;
    };

    union {
        gpio::Pin rx;
        gpio::Pin rxPin;
    };

    union {
        std::uint32_t baud;
        std::uint32_t baudRate;
    };

    gpio::af::AlternatePinConfig txPinConfig = {};
    gpio::af::AlternatePinConfig rxPinConfig = {};

    Mode mode = Mode::TX_RX;
    WordLength wordLength = WordLength::BITS_8;
    StopBits stopBits = StopBits::ONE;
    Parity parity = Parity::NONE;
    Oversampling oversampling = Oversampling::BY16;

    constexpr uartConfig()
        : tx(Traits<I>::defaultTx),
          rx(Traits<I>::defaultRx),
          baud(115200)
    {}

    constexpr explicit uartConfig(std::uint32_t baudRateVal)
        : tx(Traits<I>::defaultTx),
          rx(Traits<I>::defaultRx),
          baud(baudRateVal)
    {}

    constexpr uartConfig(gpio::Pin txPinVal, gpio::Pin rxPinVal)
        : tx(txPinVal),
          rx(rxPinVal),
          baud(115200)
    {}

    constexpr uartConfig(gpio::Pin txPinVal,
                         gpio::Pin rxPinVal,
                         std::uint32_t baudRateVal)
        : tx(txPinVal),
          rx(rxPinVal),
          baud(baudRateVal)
    {}
};

template<Instance I>
using UartConfig = uartConfig<I>;

} // namespace uart

