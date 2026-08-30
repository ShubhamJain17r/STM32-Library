/**
 * @file 04_nonblocking_tx_stream.cpp
 * @brief Example: High-throughput telemetry streaming using writeNonBlocking()
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    systick::init();

    Uart2 serial(115200);
    serial.write("--- Non-Blocking Stream Demo Ready ---\r\n");

    std::uint8_t packetCounter = 0;

    while(true)
    {
        // Try writing a byte non-blocking (returns false if ring buffer is completely full)
        if(serial.writeNonBlocking(packetCounter))
        {
            packetCounter++;
        }

        systick::delayUs(500);
    }
}

