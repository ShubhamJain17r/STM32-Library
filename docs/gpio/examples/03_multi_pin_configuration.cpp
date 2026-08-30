/**
 * @file 03_multi_pin_configuration.cpp
 * @brief Example: Custom multi-pin output and input configurations (Open-Drain, Pull-Up, Slew Rate)
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    systick::init();

    // 1. Open-Drain Output on PB0 with internal Pull-Up
    DigitalOutputConfig odConfig;
    odConfig.outputType  = OutputType::OPEN_DRAIN;
    odConfig.outputSpeed = OutputSpeed::HIGH;
    odConfig.pull        = Pull::UP;
    odConfig.initialState= PinState::HIGH;

    DigitalOutput openDrainPin(PB0, odConfig);

    // 2. High-speed Push-Pull Output on PB1
    DigitalOutput pushPullPin(PB1);

    // 3. Input with Internal Pull-Down on PB2
    DigitalInput inputPin(PB2, Pull::DOWN);

    while(true)
    {
        openDrainPin.toggle();
        pushPullPin.toggle();

        if(inputPin.isHigh())
        {
            // Trigger custom event
        }

        systick::delayMs(250);
    }
}

