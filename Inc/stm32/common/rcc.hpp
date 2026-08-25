/**
 * @file rcc.hpp
 * @brief Dynamic RCC Clock Tree frequency decoding for STM32F446
 * 
 * Computes exact real-time clock frequencies by decoding hardware registers:
 * - SYSCLK (HSI, HSE, Main PLL_P, Main PLL_R)
 * - HCLK (AHB bus clock)
 * - PCLK1 (APB1 peripheral clock) & PCLK2 (APB2 peripheral clock)
 * - Timer clock frequencies on APB1 and APB2 bus multipliers
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"

namespace rcc
{

inline constexpr std::uint32_t HSI_FREQUENCY = 16'000'000U;
inline constexpr std::uint32_t LSI_FREQUENCY =     32'000U;
inline constexpr std::uint32_t DEFAULT_HSE_FREQUENCY = 8'000'000U;

// Configurable HSE frequency (defaults to 8 MHz for STM32 Nucleo boards)
inline std::uint32_t hseFrequency = DEFAULT_HSE_FREQUENCY;

inline void setHseFrequency(std::uint32_t freq) noexcept
{
    hseFrequency = freq;
}

inline std::uint32_t getHseFrequency() noexcept
{
    return hseFrequency;
}

enum class Bus : std::uint8_t
{
    AHB1,
    AHB2,
    APB1,
    APB2
};

/**
 * @brief Computes the current SYSCLK frequency dynamically by decoding RCC registers.
 */
inline std::uint32_t getSystemClock() noexcept
{
    const std::uint32_t sws = reg::readBits(RCC->CFGR, RCC_CFGR_SWS);

    switch(sws)
    {
        case RCC_CFGR_SWS_HSI: // 0x00: HSI
            return HSI_FREQUENCY;

        case RCC_CFGR_SWS_HSE: // 0x04: HSE
            return hseFrequency;

        case RCC_CFGR_SWS_PLL: // 0x08: Main PLL (PLL_P)
        case (RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1): // 0x0C: Main PLL (PLL_R)
        {
            const std::uint32_t pllsrc = reg::isAnyBitSet(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC) ? hseFrequency : HSI_FREQUENCY;
            const std::uint32_t pllm = reg::readBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLM);
            const std::uint32_t plln = reg::readBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;

            if(pllm == 0) return HSI_FREQUENCY;

            // 64-bit intermediate arithmetic prevents both integer division truncation and 32-bit overflow
            const std::uint32_t vco = static_cast<std::uint32_t>((static_cast<std::uint64_t>(pllsrc) * plln) / pllm);

            if(sws == RCC_CFGR_SWS_PLL)
            {
                const std::uint32_t pllp = (((reg::readBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1u) * 2u);
                return vco / pllp;
            }
            else // PLL_R
            {
                const std::uint32_t pllr = reg::readBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos;
                return (pllr > 0) ? (vco / pllr) : vco;
            }
        }

        default:
            return HSI_FREQUENCY;
    }
}

/**
 * @brief Computes the current AHB bus clock (HCLK) frequency.
 */
inline std::uint32_t getHCLK() noexcept
{
    const std::uint32_t sysclk = getSystemClock();
    const std::uint32_t hpre = reg::readBits(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;

    // AHB Prescaler table: 0xxx: 1, 1000: 2, 1001: 4, 1010: 8, 1011: 16, 1100: 64, 1101: 128, 1110: 256, 1111: 512
    constexpr std::uint8_t ahbShiftTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

    return sysclk >> ahbShiftTable[hpre & 0xFu];
}

/**
 * @brief Computes the current APB1 peripheral clock (PCLK1) frequency.
 */
inline std::uint32_t getPCLK1() noexcept
{
    const std::uint32_t hclk = getHCLK();
    const std::uint32_t ppre1 = reg::readBits(RCC->CFGR, RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;

    // APB Prescaler table: 0xx: 1, 100: 2, 101: 4, 110: 8, 111: 16
    constexpr std::uint8_t apbShiftTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

    return hclk >> apbShiftTable[ppre1 & 0x7u];
}

/**
 * @brief Computes the current APB2 peripheral clock (PCLK2) frequency.
 */
inline std::uint32_t getPCLK2() noexcept
{
    const std::uint32_t hclk = getHCLK();
    const std::uint32_t ppre2 = reg::readBits(RCC->CFGR, RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;

    constexpr std::uint8_t apbShiftTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

    return hclk >> apbShiftTable[ppre2 & 0x7u];
}

/**
 * @brief Computes the clock frequency for a given peripheral bus.
 */
inline std::uint32_t frequency(Bus bus) noexcept
{
    switch(bus)
    {
        case Bus::AHB1:
        case Bus::AHB2:
            return getHCLK();

        case Bus::APB1:
            return getPCLK1();

        case Bus::APB2:
            return getPCLK2();
    }

    return 0;
}

/**
 * @brief Computes the clock frequency feeding timers on a given APB bus.
 * If APB prescaler is 1, TIM clock = PCLK. Else, TIM clock = 2 * PCLK.
 */
inline std::uint32_t getTimerFrequency(Bus bus) noexcept
{
    if(bus == Bus::APB1)
    {
        const std::uint32_t pclk1 = getPCLK1();
        const bool prescaled = reg::readBits(RCC->CFGR, RCC_CFGR_PPRE1) >= RCC_CFGR_PPRE1_DIV2;
        return prescaled ? (pclk1 * 2u) : pclk1;
    }
    else if(bus == Bus::APB2)
    {
        const std::uint32_t pclk2 = getPCLK2();
        const bool prescaled = reg::readBits(RCC->CFGR, RCC_CFGR_PPRE2) >= RCC_CFGR_PPRE2_DIV2;
        return prescaled ? (pclk2 * 2u) : pclk2;
    }

    return getHCLK();
}

} // namespace rcc
