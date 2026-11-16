/**
 * @file uart_types.hpp
 * @brief Core enums for UART peripherals (Mode, Parity, StopBits, WordLength, Oversampling)
 */

#pragma once

#include <cstdint>

namespace uart
{

enum class Mode : std::uint8_t
{
    RX,
    TX,
    TX_RX,
    Rx   = RX,
    Tx   = TX,
    TxRx = TX_RX
};

enum class Parity : std::uint8_t
{
    NONE,
    EVEN,
    ODD,
    None = NONE,
    Even = EVEN,
    Odd  = ODD
};

enum class StopBits : std::uint8_t
{
    ONE,
    HALF,
    TWO,
    ONE_AND_HALF,
    One        = ONE,
    Half       = HALF,
    Two        = TWO,
    OneAndHalf = ONE_AND_HALF
};

enum class WordLength : std::uint8_t
{
    BITS_8,
    BITS_9,
    Bits8 = BITS_8,
    Bits9 = BITS_9
};

enum class Oversampling : std::uint8_t
{
    BY16,
    BY8,
    By16 = BY16,
    By8  = BY8
};

} // namespace uart

