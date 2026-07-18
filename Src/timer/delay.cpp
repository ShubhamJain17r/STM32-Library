#include "timer/delay.hpp"
#include "common/registers.hpp"
#include "rcc/rcc.hpp"

namespace timer {

volatile std::uint32_t tick_count = 0;

void initSysTick() {
    SysTick->LOAD = (rcc::SYSCLK_MHz * 1000UL) - 1UL;
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(std::uint32_t ms) {
    const std::uint32_t last_tick = getTickCount();
    while ((getTickCount() - last_tick) < ms);
}

} // namespace timer

extern "C" {
void SysTick_Handler(void) {
    timer::tick_count++;
}
} // extern "C"
