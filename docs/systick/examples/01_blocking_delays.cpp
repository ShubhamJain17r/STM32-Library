/**
 * @file 01_blocking_delays.cpp
 * @brief Example: SysTick initialization, millisecond, and microsecond delays
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    // 1. Initialize SysTick monotonic 1 kHz timebase
    systick::init();

    // 2. Configure User LED (PA5)
    DigitalOutput led(PA5);

    while(true)
    {
        led.setHigh();
        systick::delayMs(500); // 500 ms delay

        led.setLow();
        systick::delayMs(500);

        // Precise microsecond burst
        for(int i = 0; i < 5; ++i)
        {
            led.toggle();
            systick::delayUs(50'000); // 50 ms in microseconds
        }
    }
}

