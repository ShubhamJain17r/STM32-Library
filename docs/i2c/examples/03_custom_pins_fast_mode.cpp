/**
 * @file 03_custom_pins_fast_mode.cpp
 * @brief Example: Initializing I2C1 in 400 kHz Fast Mode on PB6 (SCL) and PB7 (SDA)
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/i2c/i2c.hpp"

using namespace gpio;
using namespace i2c;

int main()
{
    systick::init();

    // Custom configuration for I2C1 on alternate pins PB6 and PB7
    I2c1Config cfg;
    cfg.scl       = PB6;
    cfg.sda       = PB7;
    cfg.speedHz   = 400'000;
    cfg.dutyCycle = DutyCycle::Duty2_1;

    I2c1 i2c(cfg);

    while(true)
    {
        // Periodic I2C transaction
        std::uint8_t dummy = 0;
        i2c.read(0x50, &dummy, 1);

        systick::delayMs(500);
    }
}

