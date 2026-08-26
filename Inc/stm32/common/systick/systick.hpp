/**
 * @file systick.hpp
 * @brief High-precision monotonic millisecond & microsecond timebase driver
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"

namespace systick
{

/**
 * @brief Initializes the Cortex-M SysTick timer for millisecond interrupts.
 * Automatically computes SysTick->LOAD using rcc::getHCLK().
 * @param tickHz Desired tick frequency (defaults to 1000 Hz for 1ms ticks).
 */
inline void init(std::uint32_t tickHz = 1000) noexcept
{
    const std::uint32_t hclk = rcc::getHCLK();
    const std::uint32_t reload = (hclk / tickHz) - 1u;

    reg::write(SysTick->LOAD, reload & SysTick_LOAD_RELOAD_Msk);
    reg::write(SysTick->VAL, 0u);
    reg::write(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk |
                              SysTick_CTRL_TICKINT_Msk   |
                              SysTick_CTRL_ENABLE_Msk);
}

/**
 * @brief Disables the SysTick timer and interrupt.
 */
inline void disable() noexcept
{
    reg::clearBits(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

/**
 * @brief Returns the number of milliseconds elapsed since SysTick was initialized.
 */
std::uint32_t millis() noexcept;

/**
 * @brief Returns high-resolution elapsed microseconds by combining the millisecond counter
 * and the current SysTick hardware countdown value.
 */
std::uint32_t micros() noexcept;

/**
 * @brief Blocking millisecond delay using the SysTick millisecond counter.
 * @param ms Duration in milliseconds.
 */
inline void delayMs(std::uint32_t ms) noexcept
{
    const std::uint32_t start = millis();
    while((millis() - start) < ms)
    {
    }
}

/**
 * @brief Blocking microsecond delay using the SysTick microsecond counter.
 * @param us Duration in microseconds.
 */
inline void delayUs(std::uint32_t us) noexcept
{
    const std::uint32_t start = micros();
    while((micros() - start) < us)
    {
    }
}

/**
 * @brief Monotonic non-blocking timeout tracking class.
 */
class Timeout
{
public:
    explicit Timeout(std::uint32_t durationMs) noexcept
        : start_(millis()), duration_(durationMs)
    {}

    /**
     * @brief Checks whether the specified timeout duration has elapsed.
     */
    bool expired() const noexcept
    {
        return (millis() - start_) >= duration_;
    }

    /**
     * @brief Restarts the timeout with a new duration in milliseconds.
     */
    void restart(std::uint32_t newDurationMs) noexcept
    {
        start_ = millis();
        duration_ = newDurationMs;
    }

    /**
     * @brief Restarts the timeout with the existing duration.
     */
    void restart() noexcept
    {
        start_ = millis();
    }

    /**
     * @brief Returns the elapsed time in milliseconds since the timeout started.
     */
    std::uint32_t elapsed() const noexcept
    {
        return millis() - start_;
    }

    /**
     * @brief Returns the remaining time in milliseconds before expiration (or 0 if expired).
     */
    std::uint32_t remaining() const noexcept
    {
        const std::uint32_t el = elapsed();
        return (el >= duration_) ? 0u : (duration_ - el);
    }

private:
    std::uint32_t start_;
    std::uint32_t duration_;
};

} // namespace systick

