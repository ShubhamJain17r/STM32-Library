/**
 * @file 01_single_channel_analog_read.cpp
 * @brief Example: 12-bit ADC single-channel reading and voltage conversion on PA0
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

    serial.write("--- ADC1 PA0 Reading Demo ---\r\n");

    Adc1 adc;
    adc.enableChannel(PA0);

    while(true)
    {
        const std::uint16_t raw = adc.readRaw(PA0);
        const float voltage     = adc.readVoltage(PA0, 3.3f);

        serial.write("PA0 Raw: ");
        printNumber(serial, raw);
        serial.write(" / 4095\r\n");

        systick::delayMs(500);
    }
}

