/**
 * @file main.cpp
 * @brief Example 01: GPIO Digital Output & SysTick Delays (LED Blink)
 * 
 * Demonstrates:
 * 1. Initializing the SysTick 1ms monotonic timebase.
 * 2. Configuring PA5 (Nucleo green user LED) with custom speed and pull options.
 * 3. Toggling and setting pin states using high(), low(), and toggle().
 */

#include "stm32f446xx.h"
#include "stm32/common/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    // 1. Initialize the SysTick monotonic timebase (defaults to 1 kHz / 1 ms tick)
    systick::init();

    // 2. Configure PA5 (Nucleo board user LED) as a Push-Pull digital output
    DigitalOutput led(PA5, DigitalOutputConfig{
        .outputType  = OutputType::PUSH_PULL,
        .outputSpeed = OutputSpeed::LOW,
        .pull        = Pull::NONE,
        .initialState= PinState::LOW
    });

    while(true)
    {
        // Toggle the LED every 500 milliseconds
        led.toggle();
        systick::delayMs(500);
    }
}

