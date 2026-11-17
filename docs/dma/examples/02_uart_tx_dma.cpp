/**
 * @file 02_uart_tx_dma.cpp
 * @brief Example: Asynchronous non-blocking UART transmission using DMA1 Stream 6
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace uart;

static DigitalOutput* pLed = nullptr;
static volatile bool dmaTxFinished = false;

void onUartTxDmaComplete()
{
    dmaTxFinished = true;
    if(pLed) pLed->toggle();
}

int main()
{
    systick::init();

    // User LED on PA5
    DigitalOutput led(PA5);
    pLed = &led;

    // USART2 on PA2 (TX) and PA3 (RX)
    Uart2 serial(115200);

    const char message[] = "Hello from STM32 bare-metal DMA UART transmitter!\r\n";

    while(true)
    {
        dmaTxFinished = false;

        // Initiate asynchronous DMA transmission
        serial.writeDma(message, onUartTxDmaComplete);

        // CPU is free to perform other computations while DMA transfers bytes
        while(!dmaTxFinished)
        {
            // Background task / sleep
        }

        systick::delayMs(1000);
    }
}
