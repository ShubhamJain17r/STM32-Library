/**
 * @file 04_spi_duplex_dma.cpp
 * @brief Example: High-speed full-duplex SPI buffer exchange using dual DMA streams
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/spi/spi.hpp"

using namespace uart;
using namespace spi;

constexpr std::size_t TransferSize = 8;
static const std::uint8_t txBuffer[TransferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
static std::uint8_t rxBuffer[TransferSize]       = {0};

static volatile bool spiDmaComplete = false;

void onSpiTransferComplete()
{
    spiDmaComplete = true;
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n========================================\r\n");
    serial.write("     STM32F446RE SPI1 Duplex DMA Demo   \r\n");
    serial.write("========================================\r\n");

    // Initialize SPI1 at 5 MHz in Mode 0 on PA5, PA6, PA7
    Spi1 spi(5'000'000, SpiMode::Mode0);

    while(true)
    {
        spiDmaComplete = false;

        // Perform full-duplex DMA exchange
        spi.transferDma(txBuffer, rxBuffer, TransferSize, onSpiTransferComplete);

        while(!spiDmaComplete)
        {
            // Free CPU while hardware clocks shift bits
        }

        serial.write("[OK] SPI1 full-duplex DMA transfer completed.\r\n");
        systick::delayMs(2000);
    }
}
