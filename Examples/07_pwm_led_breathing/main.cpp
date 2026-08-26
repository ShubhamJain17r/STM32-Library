/**
 * @file main.cpp
 * @brief Example 07: Hardware PWM LED Breathing (Multi-Level Brightness)
 * 
 * Demonstrates:
 * 1. Initializing Timer3 for 1 kHz PWM frequency on PA6 (TIM3_CH1).
 * 2. Automatic Alternate Function (AF2) GPIO configuration.
 * 3. Smooth duty cycle ramping from 0% to 100% and back using Pwm3.
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/timer/timer.hpp"

using namespace gpio;
using namespace timer;

int main()
{
    // 1. Initialize SysTick timebase for delays
    systick::init();

    // 2. Initialize Pwm3 at 1 kHz frequency
    Pwm3 pwm(1000);

    // 3. Enable Channel 1 on PA6 (automatically configured as AF2 push-pull)
    pwm.enableChannel(Channel::CH1, PA6);

    // 4. Start the PWM counter
    pwm.start();

    float duty = 0.0f;
    float step = 1.0f;

    while(true)
    {
        // Update duty cycle (0.0% to 100.0%)
        pwm.setDutyCycle(Channel::CH1, duty);

        duty += step;
        if(duty >= 100.0f || duty <= 0.0f)
        {
            step = -step; // Reverse direction
        }

        systick::delayMs(15);
    }
}
