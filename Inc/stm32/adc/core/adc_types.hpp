/**
 * @file adc_types.hpp
 * @brief Core enumeration types for ADC peripheral configuration
 */

#pragma once

#include <cstdint>

namespace adc
{

enum class Instance : std::uint8_t
{
    adc1,
    adc2,
    adc3
};

enum class Channel : std::uint8_t
{
    IN0         = 0,
    IN1         = 1,
    IN2         = 2,
    IN3         = 3,
    IN4         = 4,
    IN5         = 5,
    IN6         = 6,
    IN7         = 7,
    IN8         = 8,
    IN9         = 9,
    IN10        = 10,
    IN11        = 11,
    IN12        = 12,
    IN13        = 13,
    IN14        = 14,
    IN15        = 15,
    VrefInt     = 17,
    Temperature = 18,
    Vbat        = 18
};

enum class Resolution : std::uint8_t
{
    Bits12 = 0, // 12-bit (0..4095)
    Bits10 = 1, // 10-bit (0..1023)
    Bits8  = 2, // 8-bit  (0..255)
    Bits6  = 3  // 6-bit  (0..63)
};

enum class SamplingTime : std::uint8_t
{
    Cycles3   = 0, // 3 ADC clock cycles
    Cycles15  = 1, // 15 ADC clock cycles
    Cycles28  = 2, // 28 ADC clock cycles
    Cycles56  = 3, // 56 ADC clock cycles
    Cycles84  = 4, // 84 ADC clock cycles
    Cycles112 = 5, // 112 ADC clock cycles
    Cycles144 = 6, // 144 ADC clock cycles
    Cycles480 = 7  // 480 ADC clock cycles
};

enum class Alignment : std::uint8_t
{
    Right = 0,
    Left  = 1
};

enum class Prescaler : std::uint8_t
{
    Div2 = 0, // ADCCLK = PCLK2 / 2
    Div4 = 1, // ADCCLK = PCLK2 / 4
    Div6 = 2, // ADCCLK = PCLK2 / 6
    Div8 = 3  // ADCCLK = PCLK2 / 8
};

enum class Trigger : std::uint8_t
{
    Software = 0,
    Timer1_CC1,
    Timer1_CC2,
    Timer1_CC3,
    Timer2_CC2,
    Timer3_TRGO,
    Timer4_CC4,
    Timer5_CC1,
    Timer8_TRGO,
    Exti11
};

} // namespace adc

