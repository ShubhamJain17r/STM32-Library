/**
 * @file 01_periodic_update_interrupt.cpp
 * @brief Example: Periodic 2 Hz hardware timer update interrupt on Timer2
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/timer/timer.hpp"

using namespace gpio;
using namespace timer;

static DigitalOutput* pLed = nullptr;

static void onTimerTick()
{
    if(pLed) pLed->toggle();
}

int main()
{
    systick::init();

    DigitalOutput led(PA5);
    pLed = &led;

    // Configure Timer2 to fire at 2 Hz
    Timer2 timer(2);
    timer.setCallback(onTimerTick);
    timer.enableInterrupt();
    timer.start();

    while(true)
    {
        __WFI();
    }
}

