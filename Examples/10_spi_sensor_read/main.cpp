/**
 * @file main.cpp
 * @brief Example 10: SPI Master Interfacing with External Device & Chip Select Guard
 * 
 * Demonstrates:
 * 1. Initializing SPI1 at 1 MHz in Mode 0 (Custom pins: PB3 SCK, PB4 MISO, PB5 MOSI).
 * 2. Using software Chip Select (PA4 CS) with RAII ChipSelectGuard.
 * 3. Reading device ID and performing multi-byte register transactions.
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
    // 1. Initialize SysTick
    systick::init();

    // 2. Initialize USART2 for debug prints
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("  STM32F446RE SPI Master Sensor Read    \r\n");
    serial.write("========================================\r\n");

    // 3. Configure Chip Select pin (PA4) as a high-speed Push-Pull output (Default HIGH)
    DigitalOutput cs(PA4, DigitalOutputConfig{
        .outputType  = OutputType::PUSH_PULL,
        .outputSpeed = OutputSpeed::HIGH,
        .pull        = Pull::NONE,
        .initialState= PinState::HIGH
    });

    // 4. Configure SPI1 using PB3 (SCK), PB4 (MISO), PB5 (MOSI) at 2 MHz
    Spi1Config config;
    config.sck         = PB3;
    config.miso        = PB4;
    config.mosi        = PB5;
    config.baudRateHz  = 2'000'000; // 2 MHz
    config.spiMode     = SpiMode::Mode0;
    config.slaveSelect = SlaveSelect::Software;

    Spi1 spi(config);

    while(true)
    {
        std::uint8_t manufacturerId = 0;
        std::uint8_t memoryType     = 0;
        std::uint8_t capacity       = 0;

        // 5. Read JEDEC ID command (0x9F) using RAII ChipSelectGuard
        {
            ChipSelectGuard guard(cs); // CS driven LOW automatically

            spi.transfer(0x9F); // Opcode: Read JEDEC Identification
            manufacturerId = spi.transfer(0xFF);
            memoryType     = spi.transfer(0xFF);
            capacity       = spi.transfer(0xFF);
        } // CS driven HIGH automatically when guard goes out of scope

        serial.write("Device Read Completed.\r\n");

        systick::delayMs(2000);
    }
}

