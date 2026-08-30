/**
 * @file 01_falling_edge_button_interrupt.cpp
 * @brief Example: Falling-edge push button interrupt on PC13 waking MCU from __WFI()
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

static DigitalOutput* pLed = nullptr;

static void onButtonPressed()
{
    if(pLed) pLed->toggle();
}

int main()
{
    systick::init();

    DigitalOutput led(PA5);
    pLed = &led;

    // Configure PC13 for falling edge trigger with callback
    InterruptInput button(PC13, exti::Trigger::Falling, onButtonPressed, Pull::NONE);

    while(true)
    {
        // Sleep until next interrupt occurs
        __WFI();
    }
}

