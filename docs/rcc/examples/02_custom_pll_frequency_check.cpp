/**
 * @file 02_custom_pll_frequency_check.cpp
 * @brief Example: Dynamic timer prescaler calculation using RCC bus frequency inspection
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n--- Dynamic Timer Calculation ---\r\n");

    // Calculate prescaler for 1 MHz timer tick on APB1
    const std::uint32_t apb1TimerClock = rcc::getTimerClock(rcc::Bus::APB1);
    const std::uint32_t pscFor1Mhz = (apb1TimerClock / 1'000'000U) - 1U;

    serial.write("APB1 Timer Input Clock: ");
    char buf[16];
    int idx = 0;
    std::uint32_t val = apb1TimerClock / 1'000'000U;
    if(val == 0) buf[idx++] = '0';
    else
    {
        char tmp[16];
        int tIdx = 0;
        while(val > 0) { tmp[tIdx++] = '0' + (val % 10); val /= 10; }
        while(tIdx > 0) buf[idx++] = tmp[--tIdx];
    }
    buf[idx] = '\0';
    serial.write(buf);
    serial.write(" MHz\r\n");

    while(true)
    {
        systick::delayMs(3000);
    }
}

