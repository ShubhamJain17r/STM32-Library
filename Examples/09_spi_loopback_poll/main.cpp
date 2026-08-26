/**
 * @file main.cpp
 * @brief Example 09: Full-Duplex SPI Loopback (MOSI connected to MISO)
 * 
 * Demonstrates:
 * 1. Initializing SPI1 at 5 MHz in Mode 0 (PA5 SCK, PA6 MISO, PA7 MOSI).
 * 2. Transmitting a data packet and simultaneously reading it back.
 * 3. Verifying data integrity and indicating status via the PA5 green LED (or serial).
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
    // 1. Initialize SysTick monotonic timebase
    systick::init();

    // 2. Initialize USART2 for serial telemetry
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("  STM32F446RE SPI1 Full-Duplex Loopback  \r\n");
    serial.write("========================================\r\n");
    serial.write("Connect PA7 (MOSI) directly to PA6 (MISO)\r\n\r\n");

    // 3. Initialize SPI1 at 5 MHz, Mode 0 (SCK=PA5, MISO=PA6, MOSI=PA7)
    // Note: If PA5 is used for SCK, use an external LED pin or serial output
    Spi1 spi(5'000'000, SpiMode::Mode0);

    const std::uint8_t txData[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    std::uint8_t rxData[8] = {0};

    while(true)
    {
        // 4. Perform full-duplex transfer
        spi.transfer(txData, rxData, 8);

        // 5. Verify received bytes
        bool match = true;
        for(int i = 0; i < 8; ++i)
        {
            if(rxData[i] != txData[i])
            {
                match = false;
                break;
            }
        }

        if(match)
        {
            serial.write("[SUCCESS] Loopback data matched perfectly!\r\n");
        }
        else
        {
            serial.write("[ERROR] Loopback mismatch (check PA7 to PA6 wire)\r\n");
        }

        systick::delayMs(1000);
    }
}

