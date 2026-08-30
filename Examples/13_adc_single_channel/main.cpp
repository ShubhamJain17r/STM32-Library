/**
 * @file main.cpp
 * @brief Example 13: ADC Single Channel Analog Voltage Reading
 * 
 * Demonstrates:
 * 1. Initializing 12-bit ADC1 on APB2.
 * 2. Enabling analog input on PA0 (ADC1_IN0) with automatic GPIO Analog mode routing.
 * 3. Polling raw 12-bit counts (0..4095) and converting to floating-point Volts.
 * 4. Streaming telemetry over USART2 at 115200 baud.
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

static void printVoltage(Uart2& uart, float v)
{
    std::uint32_t whole = static_cast<std::uint32_t>(v);
    std::uint32_t milli = static_cast<std::uint32_t>((v - whole) * 1000.0f);

    printNumber(uart, whole);
    uart.write(".");
    if(milli < 100) uart.write("0");
    if(milli < 10)  uart.write("0");
    printNumber(uart, milli);
    uart.write(" V");
}

int main()
{
    // 1. Initialize SysTick monotonic timebase
    systick::init();

    // 2. Initialize USART2 for serial output
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("       STM32F446RE ADC1 Demo (PA0)      \r\n");
    serial.write("========================================\r\n\r\n");

    // 3. Initialize ADC1 (12-bit, 84 cycles sampling time)
    Adc1 adc;

    // 4. Enable PA0 (ADC1_IN0) - automatically configures PA0 as Analog Mode
    adc.enableChannel(PA0);

    while(true)
    {
        const std::uint16_t raw = adc.readRaw(PA0);
        const float voltage     = adc.readVoltage(PA0, 3.3f);

        serial.write("PA0 Raw: ");
        printNumber(serial, raw);
        serial.write(" / 4095  |  Voltage: ");
        printVoltage(serial, voltage);
        serial.write("\r\n");

        systick::delayMs(500);
    }
}

