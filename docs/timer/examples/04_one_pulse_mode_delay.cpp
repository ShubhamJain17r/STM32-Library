/**
 * @file 04_one_pulse_mode_delay.cpp
 * @brief Example: Generating a single deterministic hardware pulse using One-Pulse Mode (OPM)
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/timer/timer.hpp"

using namespace gpio;
using namespace timer;

int main()
{
    systick::init();

    // Configure Timer4 in One-Pulse Mode (OPM)
    Pulse4 opm;
    opm.enableChannel(Channel::CH1, PB6);

    while(true)
    {
        // Trigger a 500 us pulse with 100 us initial delay
        opm.triggerPulse(Channel::CH1, 100, 500);

        systick::delayMs(2000);
    }
}

