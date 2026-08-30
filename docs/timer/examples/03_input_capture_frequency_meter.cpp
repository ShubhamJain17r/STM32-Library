/**
 * @file 03_input_capture_frequency_meter.cpp
 * @brief Example: 32-bit input capture frequency & period measurement on PA0 (TIM2_CH1)
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/timer/timer.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace timer;
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

    serial.write("\r\n--- 32-bit Timer2 Input Capture Frequency Meter ---\r\n");

    // Initialize Timer2 in Input Capture Mode
    Capture2 ic;
    ic.enableChannel(Channel::CH1, PA0);

    while(true)
    {
        const float freq = ic.readFrequency(Channel::CH1);

        serial.write("Input Frequency on PA0: ");
        printNumber(serial, static_cast<std::uint32_t>(freq));
        serial.write(" Hz\r\n");

        systick::delayMs(1000);
    }
}

