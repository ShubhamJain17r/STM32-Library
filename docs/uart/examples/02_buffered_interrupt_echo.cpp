/**
 * @file 02_buffered_interrupt_echo.cpp
 * @brief Example: Asynchronous ring-buffered full-duplex UART communication
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    systick::init();

    // 256-byte TX and RX ring buffers on USART2
    UartHandler<Instance::uart2, 256, 256> serial(115200);
    serial.write("--- 256-Byte Ring-Buffered USART2 Echo Ready ---\r\n");

    std::uint8_t buffer[64];

    while(true)
    {
        if(serial.available())
        {
            const std::size_t bytesToRead = serial.available();
            const std::size_t chunk = (bytesToRead > sizeof(buffer)) ? sizeof(buffer) : bytesToRead;

            serial.read(buffer, chunk);
            serial.write(buffer, chunk);
        }

        systick::delayMs(5);
    }
}

