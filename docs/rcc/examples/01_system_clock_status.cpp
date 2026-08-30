/**
 * @file 01_system_clock_status.cpp
 * @brief Example: Reading and displaying active system and bus clock frequencies
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

static void printFrequency(Uart2& uart, const char* name, std::uint32_t freqHz)
{
    uart.write(name);
    uart.write(": ");
    
    char buf[16];
    int idx = 0;
    std::uint32_t mhz = freqHz / 1'000'000U;
    std::uint32_t khz = (freqHz % 1'000'000U) / 1'000U;

    // Convert MHz
    if(mhz == 0) buf[idx++] = '0';
    else
    {
        char tmp[16];
        int tIdx = 0;
        while(mhz > 0) { tmp[tIdx++] = '0' + (mhz % 10); mhz /= 10; }
        while(tIdx > 0) buf[idx++] = tmp[--tIdx];
    }
    buf[idx++] = '.';

    // Convert kHz (3 digits)
    buf[idx++] = '0' + ((khz / 100) % 10);
    buf[idx++] = '0' + ((khz / 10) % 10);
    buf[idx++] = '0' + (khz % 10);
    buf[idx++] = '\0';

    uart.write(buf);
    uart.write(" MHz (");

    // Raw integer
    idx = 0;
    std::uint32_t val = freqHz;
    if(val == 0) buf[idx++] = '0';
    else
    {
        char tmp[16];
        int tIdx = 0;
        while(val > 0) { tmp[tIdx++] = '0' + (val % 10); val /= 10; }
        while(tIdx > 0) buf[idx++] = tmp[--tIdx];
    }
    buf[idx++] = '\0';

    uart.write(buf);
    uart.write(" Hz)\r\n");
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n========================================\r\n");
    serial.write("     STM32F446RE Clock Tree Status      \r\n");
    serial.write("========================================\r\n");

    printFrequency(serial, "SYSCLK", rcc::getSYSCLK());
    printFrequency(serial, "HCLK  ", rcc::getHCLK());
    printFrequency(serial, "PCLK1 ", rcc::getPCLK1());
    printFrequency(serial, "PCLK2 ", rcc::getPCLK2());
    printFrequency(serial, "TIM1_CLK", rcc::getTimerClock(rcc::Bus::APB1));
    printFrequency(serial, "TIM2_CLK", rcc::getTimerClock(rcc::Bus::APB2));

    while(true)
    {
        systick::delayMs(2000);
    }
}

