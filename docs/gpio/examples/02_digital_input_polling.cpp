/**
 * @file 02_digital_input_polling.cpp
 * @brief Example: Polling active-low user button on PC13 with DigitalInput
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    systick::init();

    DigitalOutput led(PA5);
    DigitalInput  button(PC13, Pull::NONE); // Blue button has external pull-up on Nucleo

    while(true)
    {
        if(button.isLow()) // Active-low pressed state
        {
            led.setHigh();
        }
        else
        {
            led.setLow();
        }

        systick::delayMs(20);
    }
}

