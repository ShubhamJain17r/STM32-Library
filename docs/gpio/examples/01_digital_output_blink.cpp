/**
 * @file 01_digital_output_blink.cpp
 * @brief Example: Basic DigitalOutput configuration and LED blinking on PA5
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    systick::init();

    // Configure PA5 (Nucleo Green LED) with default Push-Pull High-Speed output
    DigitalOutput led(PA5);

    while(true)
    {
        led.toggle();
        systick::delayMs(500);
    }
}

