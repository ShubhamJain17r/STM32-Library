/**
 * @file 03_multi_channel_scan_sequence.cpp
 * @brief Example: Multi-channel sequential scanning on PA0 (IN0), PA1 (IN1), and PA4 (IN4)
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

    serial.write("--- ADC1 3-Channel Scan Sequence Demo ---\r\n");

    Adc1 adc;

    // Configure 3-channel sequence: IN0 (PA0), IN1 (PA1), IN4 (PA4)
    constexpr Channel sequence[3] = {Channel::IN0, Channel::IN1, Channel::IN4};
    adc.configureSequence(sequence, 3);

    std::uint16_t scanResults[3];

    while(true)
    {
        adc.readSequence(scanResults, 3);

        serial.write("PA0: ");
        printNumber(serial, scanResults[0]);
        serial.write(" | PA1: ");
        printNumber(serial, scanResults[1]);
        serial.write(" | PA4: ");
        printNumber(serial, scanResults[2]);
        serial.write("\r\n");

        systick::delayMs(500);
    }
}

