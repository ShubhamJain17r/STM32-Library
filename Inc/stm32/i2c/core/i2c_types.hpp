/**
 * @file i2c_types.hpp
 * @brief Core enumeration types for I2C peripheral configuration
 */

#pragma once

#include <cstdint>

namespace i2c
{

enum class Instance : std::uint8_t
{
    i2c1,
    i2c2,
    i2c3
};

enum class Speed : std::uint32_t
{
    Standard = 100'000, // 100 kHz Standard Mode (Sm)
    Fast     = 400'000  // 400 kHz Fast Mode (Fm)
};

enum class DutyCycle : std::uint8_t
{
    Duty2_1  = 0, // Tlow/Thigh = 2
    Duty16_9 = 1  // Tlow/Thigh = 16/9
};

enum class AddressingMode : std::uint8_t
{
    Bits7  = 0,
    Bits10 = 1
};

enum class I2cStatus : std::uint8_t
{
    Success         = 0,
    Nack            = 1,
    BusError        = 2,
    ArbitrationLost = 3,
    Timeout         = 4,
    Busy            = 5,
    Overrun         = 6,
    Error           = 7
};

} // namespace i2c

