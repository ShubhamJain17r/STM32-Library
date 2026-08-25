/**
 * @file main.cpp
 * @brief Example 03: EXTI External Interrupt with Edge Detection
 * 
 * Demonstrates:
 * 1. Configuring an interrupt-driven input on PC13 (user button).
 * 2. Registering a static callback function for falling edges (button press).
 * 3. Immediate response without polling overhead.
 */

#include "stm32f446xx.h"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

// Global LED reference accessible from the ISR callback
static DigitalOutput* pLed = nullptr;

// Static callback invoked deterministically on button press (falling edge)
void onButtonPressed()
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

    // 2. Configure PC13 as an interrupt input on Falling Edge with callback
    InterruptInput button(PC13, InterruptInputConfig{
        .pull            = Pull::NONE,
        .trigger         = exti::Trigger::Falling,
        .callback        = onButtonPressed,
        .edgeCallback    = nullptr,
        .risingCallback  = nullptr,
        .fallingCallback = nullptr
    });

    while(true)
    {
        // Low power wait-for-interrupt loop; all action happens in the EXTI ISR
        __WFI();
    }
}

