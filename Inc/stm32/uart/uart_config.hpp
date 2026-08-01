#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/uart/uart_traits.hpp"
#include "stm32/uart/uart_types.hpp"

#include "stm32/gpio/pin.hpp"

namespace uart
{

template<USART_TypeDef* Instance>
struct Config
{
    gpio::Pin tx = Traits<Instance>::defaultTx;

    gpio::Pin rx = Traits<Instance>::defaultRx;

    std::uint32_t baud = 115200;

    Mode mode = Mode::TX_RX;

    WordLength wordLength = WordLength::BITS_8;

    StopBits stopBits = StopBits::ONE;

    Parity parity = Parity::NONE;

    Oversampling oversampling = Oversampling::BY16;
};

} // namespace uart
