#pragma once

#include <cstdint>

namespace timer {

extern volatile std::uint32_t tick_count;

void initSysTick();
void delay_ms(std::uint32_t ms);

inline std::uint32_t getTickCount() {
    return tick_count;
}

} // namespace timer
