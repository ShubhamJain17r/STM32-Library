/**
 * @file 02_sensor_register_read_write.cpp
 * @brief Example: Reading and writing sensor registers (e.g. MPU6050) over I2C1
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

    I2c1 i2c(400'000); // 400 kHz Fast Mode

    constexpr std::uint8_t SENSOR_ADDR = 0x68;
    constexpr std::uint8_t WHO_AM_I_REG = 0x75;

    serial.write("--- I2C Sensor Register Read Demo ---\r\n");

    while(true)
    {
        if(i2c.isDeviceReady(SENSOR_ADDR))
        {
            std::uint8_t chipId = 0;
            if(i2c.readRegister(SENSOR_ADDR, WHO_AM_I_REG, chipId))
            {
                serial.write("Sensor Responded! WHO_AM_I = ");
                printHexByte(serial, chipId);
                serial.write("\r\n");
            }
        }
        else
        {
            serial.write("Sensor not responding. Retrying...\r\n");
        }

        systick::delayMs(2000);
    }
}

