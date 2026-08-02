#pragma once

#include <cstdint>

namespace rcc
{

inline constexpr std::uint32_t HSI_FREQUENCY = 16'000'000U;
inline constexpr std::uint32_t LSI_FREQUENCY =     32'000U;

inline constexpr std::uint32_t SYSTEM_CLOCK = HSI_FREQUENCY;
inline constexpr std::uint32_t AHB_CLOCK    = SYSTEM_CLOCK;
inline constexpr std::uint32_t APB1_CLOCK   = SYSTEM_CLOCK;
inline constexpr std::uint32_t APB2_CLOCK   = SYSTEM_CLOCK;

enum class Bus : std::uint8_t
{
    AHB1,
    AHB2,

    APB1,
    APB2
};

constexpr std::uint32_t frequency(Bus bus) noexcept
{
    switch(bus)
    {
        case Bus::AHB1:
        case Bus::AHB2:
            return AHB_CLOCK;

        case Bus::APB1:
            return APB1_CLOCK;

        case Bus::APB2:
            return APB2_CLOCK;
    }

    return 0;
}

} // namespace rcc
