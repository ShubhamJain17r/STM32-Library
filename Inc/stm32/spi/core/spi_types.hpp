/**
 * @file spi_types.hpp
 * @brief Core enumeration types for SPI peripheral configuration
 */

#pragma once

#include <cstdint>

namespace spi
{

enum class Instance : std::uint8_t
{
    spi1,
    spi2,
    spi3,
    spi4
};

enum class Mode : std::uint8_t
{
    Slave  = 0,
    Master = 1
};

enum class ClockPolarity : std::uint8_t
{
    Low  = 0,
    High = 1
};

enum class ClockPhase : std::uint8_t
{
    Edge1 = 0,
    Edge2 = 1
};

enum class SpiMode : std::uint8_t
{
    Mode0 = 0, // CPOL = 0, CPHA = 0 (Idle Low, Sample 1st edge)
    Mode1 = 1, // CPOL = 0, CPHA = 1 (Idle Low, Sample 2nd edge)
    Mode2 = 2, // CPOL = 1, CPHA = 0 (Idle High, Sample 1st edge)
    Mode3 = 3  // CPOL = 1, CPHA = 1 (Idle High, Sample 2nd edge)
};

enum class BitOrder : std::uint8_t
{
    MsbFirst = 0,
    LsbFirst = 1
};

enum class DataSize : std::uint8_t
{
    Bits8  = 0,
    Bits16 = 1
};

enum class Prescaler : std::uint8_t
{
    Div2   = 0,
    Div4   = 1,
    Div8   = 2,
    Div16  = 3,
    Div32  = 4,
    Div64  = 5,
    Div128 = 6,
    Div256 = 7
};

enum class Direction : std::uint8_t
{
    FullDuplex = 0,
    RxOnly     = 1,
    TxOnly     = 2,
    HalfDuplex = 3
};

enum class SlaveSelect : std::uint8_t
{
    Software       = 0,
    HardwareInput  = 1,
    HardwareOutput = 2
};

} // namespace spi

