/**
 * @file main.cpp
 * @brief Example 14: ADC Internal Temperature & VREFINT Reading
 * 
 * Demonstrates:
 * 1. Enabling internal TSVREFE channels on ADC1.
 * 2. Reading MCU die temperature using datasheet linear approximation.
 * 3. Reading internal bandgap reference voltage (VREFINT ~1.21V).
 * 4. Streaming telemetry over USART2.
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/adc/adc.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace adc;
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
        while(val > 0)
        {
            tmp[tIdx++] = '0' + (val % 10);
            val /= 10;
        }
        while(tIdx > 0) buf[idx++] = tmp[--tIdx];
    }
    buf[idx] = '\0';
    uart.write(buf);
}

static void printFloat(Uart2& uart, float val, const char* unit)
{
    if(val < 0.0f)
    {
        uart.write("-");
        val = -val;
    }
    std::uint32_t whole = static_cast<std::uint32_t>(val);
    std::uint32_t frac  = static_cast<std::uint32_t>((val - whole) * 100.0f);

    printNumber(uart, whole);
    uart.write(".");
    if(frac < 10) uart.write("0");
    printNumber(uart, frac);
    uart.write(unit);
}

int main()
{
    // 1. Initialize SysTick monotonic timebase
    systick::init();

    // 2. Initialize USART2 for serial output
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("    STM32F446RE ADC1 Internal Sensors   \r\n");
    serial.write("========================================\r\n\r\n");

    // 3. Initialize ADC1
    Adc1 adc;

    // 4. Enable Temperature and VREFINT channels
    adc.enableChannel(Channel::Temperature);
    adc.enableChannel(Channel::VrefInt);

    while(true)
    {
        const float tempC   = adc.readInternalTemperature(3.3f);
        const float vrefInt = adc.readVrefInternal(3.3f);

        serial.write("Die Temp: ");
        printFloat(serial, tempC, " deg C");
        serial.write("  |  VREFINT: ");
        printFloat(serial, vrefInt, " V");
        serial.write("\r\n");

        systick::delayMs(1000);
    }
}

