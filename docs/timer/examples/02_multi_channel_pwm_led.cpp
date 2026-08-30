/**
 * @file 02_multi_channel_pwm_led.cpp
 * @brief Example: Smooth hardware PWM breathing LED on PA6 (TIM3_CH1)
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

    // Configure Timer3 for 1 kHz PWM
    Pwm3 pwm(1000);
    pwm.enableChannel(Channel::CH1, PA6); // Auto-configures PA6 as TIM3_CH1 AF2
    pwm.start();

    float duty = 0.0f;
    float step = 1.0f;

    while(true)
    {
        pwm.setDutyCycle(Channel::CH1, duty);

        duty += step;
        if(duty >= 100.0f || duty <= 0.0f)
        {
            step = -step;
        }

        systick::delayMs(15);
    }
}

