/**
 * @file 01_active_bus_scanner.cpp
 * @brief Example: Scanning all 7-bit addresses (0x08 to 0x77) on I2C1
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/i2c/i2c.hpp"
#include "stm32/uart/uart.hpp"

using namespace i2c;
using namespace uart;

static void printHexByte(Uart2& uart, std::uint8_t val)
{
    const char hexChars[] = "0123456789ABCDEF";
    char buf[5];
    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = hexChars[(val >> 4) & 0x0F];
    buf[3] = hexChars[val & 0x0F];
    buf[4] = '\0';
    uart.write(buf);
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n========================================\r\n");
    serial.write("     STM32F446RE I2C1 Bus Scanner       \r\n");
    serial.write("========================================\r\n\r\n");

    // Initialize I2C1 at 100 kHz Standard Mode on PB8 (SCL) and PB9 (SDA)
    I2c1 i2c(100'000);

    while(true)
    {
        serial.write("Scanning I2C bus...\r\n");

        std::uint8_t devices[32];
        const std::size_t count = i2c.scanBus(devices, 32);

        if(count == 0)
        {
            serial.write(" -> No I2C devices found on bus.\r\n");
        }
        else
        {
            serial.write(" -> Found device(s) at: ");
            for(std::size_t i = 0; i < count; ++i)
            {
                printHexByte(serial, devices[i]);
                serial.write(" ");
            }
            serial.write("\r\n");
        }

        serial.write("----------------------------------------\r\n");
        systick::delayMs(3000);
    }
}

