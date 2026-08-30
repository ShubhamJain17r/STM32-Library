/**
 * @file 01_blocking_echo_minimal.cpp
 * @brief Example: Minimal 1-line USART2 echo console
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    systick::init();

    // Initialize USART2 at 115200 baud on default PA2 (TX) and PA3 (RX)
    Uart2 serial(115200);
    serial.write("--- Minimal USART2 Echo Console Ready ---\r\n");

    while(true)
    {
        if(serial.available())
        {
            char c = serial.read();
            serial.writeByte(static_cast<std::uint8_t>(c)); // Echo character back
        }
    }
}

