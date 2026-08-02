#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/uart/uart_traits.hpp"
#include "stm32/uart/uart_types.hpp"

#include "stm32/gpio/pin.hpp"

namespace uart
{

template<Instance I>
struct uartConfig
{
    gpio::Pin tx = Traits<I>::defaultTx;

    gpio::Pin rx = Traits<I>::defaultRx;

    std::uint32_t baud = 115200;

    Mode mode = Mode::TX_RX;

    WordLength wordLength = WordLength::BITS_8;

    StopBits stopBits = StopBits::ONE;

    Parity parity = Parity::NONE;

    Oversampling oversampling = Oversampling::BY16;
};

} // namespace uart
