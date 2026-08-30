/**
 * @file 03_sensor_read_chip_select_guard.cpp
 * @brief Example: SPI device register read using RAII ChipSelectGuard
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/spi/spi.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace spi;
using namespace uart;

int main()
{
    systick::init();
    Uart2 serial(115200);

    // Initialize SPI1 at 1 MHz
    Spi1 spi(1'000'000, SpiMode::Mode3);

    // Chip Select Pin on PB6
    DigitalOutput csPin(PB6, DigitalOutputConfig{
        .outputType   = OutputType::PUSH_PULL,
        .outputSpeed  = OutputSpeed::HIGH,
        .pull         = Pull::UP,
        .initialState = PinState::HIGH
    });

    serial.write("--- SPI Sensor Reading with RAII CS Guard ---\r\n");

    while(true)
    {
        std::uint8_t regAddr = 0x80 | 0x0F; // Read WHO_AM_I register (MSB=1 for read)
        std::uint8_t chipId = 0;

        {
            ChipSelectGuard guard(csPin); // Automatically drives CS LOW
            spi.transfer(regAddr);
            chipId = spi.transfer(0x00);
        } // Automatically drives CS HIGH

        serial.write("Sensor ID Read: ");
        char buf[5];
        const char hex[] = "0123456789ABCDEF";
        buf[0] = '0'; buf[1] = 'x';
        buf[2] = hex[(chipId >> 4) & 0xF];
        buf[3] = hex[chipId & 0xF];
        buf[4] = '\0';
        serial.write(buf);
        serial.write("\r\n");

        systick::delayMs(2000);
    }
}

