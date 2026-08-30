/**
 * @file 04_async_interrupt_transfer.cpp
 * @brief Example: Asynchronous non-blocking SPI transfer with completion callback
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/spi/spi.hpp"
#include "stm32/uart/uart.hpp"

using namespace spi;
using namespace uart;

static volatile bool transferDone = false;

static void onSpiTransferFinished()
{
    transferDone = true;
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    Spi1 spi(2'000'000, SpiMode::Mode0);
    spi.attachTransferCompleteCallback(onSpiTransferFinished);

    serial.write("--- Asynchronous Interrupt-Driven SPI Transfer Ready ---\r\n");

    std::uint8_t txBuf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::uint8_t rxBuf[8];

    while(true)
    {
        transferDone = false;
        spi.transferAsync(txBuf, rxBuf, 8);

        while(!transferDone)
        {
            // Background task runs here while SPI transfers in interrupt
            __WFI();
        }

        serial.write("[OK] Asynchronous transfer complete!\r\n");
        systick::delayMs(1500);
    }
}

