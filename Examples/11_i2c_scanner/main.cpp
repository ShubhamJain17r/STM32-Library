/**
 * @file main.cpp
 * @brief Example 11: Active I2C Bus Scanner & Device Detection
 * 
 * Demonstrates:
 * 1. Initializing I2C1 in Standard Mode (100 kHz) on PB8 (SCL) and PB9 (SDA).
 * 2. Automatic Open-Drain Alternate Function (AF4) GPIO configuration with internal pull-ups.
 * 3. Probing all valid 7-bit addresses (0x08 to 0x77) and reporting active devices over USART2.
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/i2c/i2c.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
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
    // 1. Initialize SysTick monotonic timebase
    systick::init();

    // 2. Initialize USART2 (PA2/PA3) for serial telemetry
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("     STM32F446RE I2C1 Bus Scanner       \r\n");
    serial.write("========================================\r\n");
    serial.write("Default Pins: PB8 (SCL), PB9 (SDA)\r\n\r\n");

    // 3. Initialize I2C1 at 100 kHz Standard Mode
    I2c1 i2c(100'000);

    while(true)
    {
        serial.write("Scanning I2C bus (0x08 - 0x77)...\r\n");

        std::uint8_t foundDevices[32];
        const std::size_t count = i2c.scanBus(foundDevices, 32);

        if(count == 0)
        {
            serial.write(" -> No I2C devices found on bus.\r\n");
        }
        else
        {
            serial.write(" -> Found ");
            char numBuf[4];
            numBuf[0] = '0' + static_cast<char>(count);
            numBuf[1] = '\0';
            serial.write(numBuf);
            serial.write(" device(s): ");

            for(std::size_t i = 0; i < count; ++i)
            {
                printHexByte(serial, foundDevices[i]);
                serial.write(" ");
            }
            serial.write("\r\n");
        }

        serial.write("----------------------------------------\r\n");
        systick::delayMs(3000);
    }
}

