/**
 * @file 04_bus_recovery_stuck_slave.cpp
 * @brief Example: Recovering an I2C bus when a slave holds SDA low using recoverBus()
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/i2c/i2c.hpp"
#include "stm32/uart/uart.hpp"

using namespace i2c;
using namespace uart;

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("--- I2C Bus Recovery Routine Demo ---\r\n");

    I2c1 i2c(100'000);

    // Attempt bus recovery if line is wedged
    serial.write("Executing 9-pulse SCL bus recovery...\r\n");
    i2c.recoverBus();
    serial.write("[OK] Bus recovery complete. Bus restored to clean state.\r\n");

    while(true)
    {
        systick::delayMs(1000);
    }
}

