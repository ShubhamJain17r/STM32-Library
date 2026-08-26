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
    gpio::Pin tx = Traits<I>::defaultTx;
    gpio::Pin rx = Traits<I>::defaultRx;

    std::uint32_t baud = 115200;

    gpio::af::AlternatePinConfig txPinConfig = {};
    gpio::af::AlternatePinConfig rxPinConfig = {};

    Mode mode = Mode::TX_RX;
    WordLength wordLength = WordLength::BITS_8;
    StopBits stopBits = StopBits::ONE;
    Parity parity = Parity::NONE;
    Oversampling oversampling = Oversampling::BY16;

    constexpr uartConfig() = default;

    constexpr explicit uartConfig(std::uint32_t baudRate)
        : baud(baudRate)
    {}

    constexpr uartConfig(gpio::Pin txPin, gpio::Pin rxPin)
        : tx(txPin), rx(rxPin)
    {}

    constexpr uartConfig(gpio::Pin txPin,
                         gpio::Pin rxPin,
                         std::uint32_t baudRate)
        : tx(txPin),
          rx(rxPin),
          baud(baudRate)
    {}
};

template<Instance I>
using UartConfig = uartConfig<I>;

} // namespace uart

