/**
 * @file 02_custom_pins_and_prescaler.cpp
 * @brief Example: Custom SPI1 pin mapping on PB3 (SCK), PB4 (MISO), PB5 (MOSI) at 2 MHz
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/spi/spi.hpp"

using namespace gpio;
using namespace spi;

int main()
{
    systick::init();

    // Configure SPI1 on alternate pins PB3, PB4, PB5
    Spi1Config cfg;
    cfg.sck         = PB3;
    cfg.miso        = PB4;
    cfg.mosi        = PB5;
    cfg.baudRateHz  = 2'000'000;
    cfg.spiMode     = SpiMode::Mode0;
    cfg.slaveSelect = SlaveSelect::Software;

    Spi1 spi(cfg);

    while(true)
    {
        spi.transfer(0xAA);
        systick::delayMs(100);
    }
}

