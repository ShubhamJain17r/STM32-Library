/**
 * @file 02_internal_temperature_and_vref.cpp
 * @brief Example: Reading internal die temperature and bandgap VREFINT reference on ADC1
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/adc/adc.hpp"
#include "stm32/uart/uart.hpp"

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

    serial.write("--- ADC1 Internal Die Temperature & VREFINT Demo ---\r\n");

    Adc1 adc;
    adc.enableChannel(Channel::Temperature);
    adc.enableChannel(Channel::VrefInt);

    while(true)
    {
        const float tempC   = adc.readInternalTemperature(3.3f);
        const float vrefInt = adc.readVrefInternal(3.3f);

        serial.write("Die Temp: ");
        printNumber(serial, static_cast<std::uint32_t>(tempC));
        serial.write(" deg C\r\n");

        systick::delayMs(1000);
    }
}

