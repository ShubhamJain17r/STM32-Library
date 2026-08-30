/**
 * @file 01_blocking_loopback_defaults.cpp
 * @brief Example: Full-duplex 5 MHz SPI1 loopback exchange on PA5 (SCK), PA6 (MISO), PA7 (MOSI)
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/spi/spi.hpp"
#include "stm32/uart/uart.hpp"

using namespace spi;
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
    serial.write("     STM32F446RE SPI1 Loopback Demo     \r\n");
    serial.write("========================================\r\n");
    serial.write("Connect PA6 (MISO) to PA7 (MOSI) to verify.\r\n\r\n");

    // Initialize SPI1 at 5 MHz in Mode 0 on PA5, PA6, PA7
    Spi1 spi(5'000'000, SpiMode::Mode0);

    std::uint8_t txData[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    std::uint8_t rxData[4] = {0x00, 0x00, 0x00, 0x00};

    while(true)
    {
        spi.transfer(txData, rxData, 4);

        serial.write("Sent: 0xDEADBEEF | Received: ");
        for(int i = 0; i < 4; ++i)
        {
            printHexByte(serial, rxData[i]);
            serial.write(" ");
        }
        serial.write("\r\n");

        systick::delayMs(1500);
    }
}

