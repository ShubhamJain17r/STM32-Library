/**
 * @file 02_rising_edge_sensor_interrupt.cpp
 * @brief Example: Rising edge pulse detection on PB0 with event counter
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace uart;

static volatile std::uint32_t pulseCount = 0;

static void onSensorPulse()
{
    pulseCount++;
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n--- Rising Edge Sensor Pulse Counter ---\r\n");

    // Configure PB0 for rising edge pulse capture with internal pull-down
    InterruptInput sensorPin(PB0, exti::Trigger::Rising, onSensorPulse, Pull::DOWN);

    while(true)
    {
        serial.write("Pulses counted: ");
        char buf[16];
        int idx = 0;
        std::uint32_t val = pulseCount;
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
        serial.write("\r\n");

        systick::delayMs(1000);
    }
}

