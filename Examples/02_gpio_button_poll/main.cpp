/**
 * @file main.cpp
 * @brief Example 02: GPIO Digital Input Polling (Button & LED)
 * 
 * Demonstrates:
 * 1. Configuring PC13 (Nucleo blue user push-button, active-low) as a DigitalInput.
 * 2. Configuring PA5 as a DigitalOutput.
 * 3. Polling the button state in the main loop and updating the LED.
 */

#include "stm32f446xx.h"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    // 1. Configure PA5 as digital output for user LED
    DigitalOutput led(PA5);

    // 2. Configure PC13 as digital input (Nucleo user button has an external pull-up)
    DigitalInput button(PC13, Pull::NONE);

    while(true)
    {
        // On STM32 Nucleo boards, PC13 button is pressed when the line is LOW.
        if(button.isLow())
        {
            led.high(); // Button pressed -> LED ON
        }
        else
        {
            led.low();  // Button released -> LED OFF
        }
    }
}

