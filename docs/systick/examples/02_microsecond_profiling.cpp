/**
 * @file 02_microsecond_profiling.cpp
 * @brief Example: Benchmarking execution duration of functions using systick::micros()
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

static void printNumber(Uart2& uart, std::uint32_t val)
{
    char buf[16];
    int idx = 0;
    if(val == 0) buf[idx++] = '0';
    else
    {
        char tmp[16];
        int tIdx = 0;
        while(val > 0) { tmp[tIdx++] = '0' + (val % 10); val /= 10; }
        while(tIdx > 0) buf[idx++] = tmp[--tIdx];
    }
    buf[idx] = '\0';
    uart.write(buf);
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n--- Execution Time Profiling ---\r\n");

    while(true)
    {
        const std::uint32_t start = systick::micros();

        // Workload to benchmark (e.g. math loop)
        volatile float x = 1.0f;
        for(int i = 0; i < 1000; ++i)
        {
            x = x * 1.001f + 0.5f;
        }

        const std::uint32_t duration = systick::micros() - start;

        serial.write("Loop Execution Time: ");
        printNumber(serial, duration);
        serial.write(" microseconds\r\n");

        systick::delayMs(2000);
    }
}

