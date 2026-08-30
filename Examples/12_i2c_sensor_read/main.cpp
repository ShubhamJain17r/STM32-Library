/**
 * @file main.cpp
 * @brief Example 12: I2C Sensor Interfacing & Register Read / Write Operations
 * 
 * Demonstrates:
 * 1. Initializing I2C1 in Fast Mode (400 kHz) on custom pins PB6 (SCL) and PB7 (SDA).
 * 2. Probing device readiness with isDeviceReady().
 * 3. Performing atomic 8-bit register writes and reads (e.g. MPU6050 WHO_AM_I, BMP280, or AT24C256).
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

    // 2. Initialize USART2 for debug messages
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("     STM32F446RE I2C Sensor Reader      \r\n");
    serial.write("========================================\r\n");

    // 3. Configure I2C1 for 400 kHz on PB6 (SCL) and PB7 (SDA)
    I2c1Config config;
    config.scl       = PB6;
    config.sda       = PB7;
    config.speedHz   = 400'000; // 400 kHz Fast Mode
    config.dutyCycle = DutyCycle::Duty2_1;

    I2c1 i2c(config);

    // Target I2C Slave Address (e.g. MPU6050 = 0x68, BMP280 = 0x76, SSD1306 = 0x3C)
    constexpr std::uint8_t SENSOR_ADDR = 0x68;
    constexpr std::uint8_t WHO_AM_I_REG = 0x75;

    while(true)
    {
        // 4. Check if sensor acknowledges its address
        if(i2c.isDeviceReady(SENSOR_ADDR))
        {
            serial.write("[OK] Sensor responded at address: ");
            printHexByte(serial, SENSOR_ADDR);
            serial.write("\r\n");

            // 5. Read WHO_AM_I Identification Register
            std::uint8_t chipId = 0;
            if(i2c.readRegister(SENSOR_ADDR, WHO_AM_I_REG, chipId))
            {
                serial.write(" -> WHO_AM_I Register: ");
                printHexByte(serial, chipId);
                serial.write("\r\n");
            }
            else
            {
                serial.write(" -> [ERROR] Failed to read WHO_AM_I register\r\n");
            }
        }
        else
        {
            serial.write("[WARN] Sensor not detected at address ");
            printHexByte(serial, SENSOR_ADDR);
            serial.write(". Re-trying...\r\n");
        }

        systick::delayMs(2000);
    }
}

