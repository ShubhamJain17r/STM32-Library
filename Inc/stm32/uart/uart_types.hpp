#pragma once

#include <cstdint>

namespace uart
{

enum class Mode : std::uint8_t
{
    RX,
    TX,
    TX_RX
};

enum class Parity : std::uint8_t
{
    NONE,
    EVEN,
    ODD
};

enum class StopBits : std::uint8_t
{
    ONE,
    HALF,
    TWO,
    ONE_AND_HALF
};

enum class WordLength : std::uint8_t
{
    BITS_8,
    BITS_9
};

enum class Oversampling : std::uint8_t
{
    BY16,
    BY8
};

enum class Interrupt : std::uint8_t
{
    ParityError,
    TxEmpty,
    TxComplete,
    RxNotEmpty
};

} // namespace uart
