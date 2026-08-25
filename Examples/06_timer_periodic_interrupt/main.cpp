/**
 * @file main.cpp
 * @brief Example 06: Hardware Timer Periodic Interrupt
 * 
 * Demonstrates:
 * 1. Initializing Timer2 for 2 Hz periodic interrupts (500 ms period).
 * 2. Attaching a static callback to toggle the user LED on PA5.
 * 3. Starting the hardware timebase counter.
 */

#include "stm32f446xx.h"
#include "stm32/gpio/gpio.hpp"
#include "stm32/timer/timer.hpp"

using namespace gpio;
using namespace timer;

static DigitalOutput* pLed = nullptr;

void onTimerTick()
{
    if(pLed)
    {
        pLed->toggle();
    }
}

int main()
{
    // 1. Initialize user LED on PA5
    DigitalOutput led(PA5);
    pLed = &led;

    // 2. Initialize Timer2 at 2 Hz (fires every 500 ms)
    Timer2 timer(2);

    // 3. Attach callback and enable interrupts
    timer.setCallback(onTimerTick);
    timer.enableInterrupt();

    // 4. Start the hardware counter
    timer.start();

    while(true)
    {
        // Low-power sleep; CPU wakes on timer interrupt
        __WFI();
    }
}

