#include "stm32/common/systick.hpp"

namespace
{
volatile std::uint32_t systickMillis_ = 0;
}

namespace systick
{

std::uint32_t millis() noexcept
{
    return systickMillis_;
}

std::uint32_t micros() noexcept
{
    std::uint32_t ms;
    std::uint32_t val;
    std::uint32_t load;

    // Guard against SysTick count wrap-around while reading
    do
    {
        ms = systickMillis_;
        val = SysTick->VAL;
    } while(ms != systickMillis_);

    load = SysTick->LOAD;
    if(load == 0) return ms * 1000u;

    const std::uint32_t elapsedSubTicks = load - val;
    const std::uint32_t usFraction = (elapsedSubTicks * 1000u) / (load + 1u);

    return (ms * 1000u) + usFraction;
}

} // namespace systick

extern "C"
{

void SysTick_Handler()
{
    systickMillis_++;
}

} // extern "C"
