/**
 * @file main.cpp
 * @brief Example 04: SysTick Timebase, High-Precision Delays & Non-Blocking Timeout
 * 
 * Demonstrates:
 * 1. Initializing SysTick with dynamic HCLK clock tree calculation.
 * 2. Reading monotonic elapsed milliseconds (systick::millis()) and microseconds (systick::micros()).
 * 3. Blocking delays (delayMs, delayUs).
 * 4. Non-blocking periodic task execution using RAII systick::Timeout.
 */

#include "stm32f446xx.h"
#include "stm32/common/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    // 1. Initialize SysTick 1ms monotonic tick
    systick::init();

    // 2. Configure user LED on PA5
    DigitalOutput led(PA5);

    // 3. Set up a non-blocking 250ms periodic timeout
    systick::Timeout blinkTimer(250);

    while(true)
    {
        // Non-blocking check: returns true when 250ms have elapsed and restarts the timer
        if(blinkTimer.expired())
        {
            led.toggle();
            blinkTimer.restart();
        }

        // Other tasks can run continuously here without being blocked by delay loops!
    }
}

