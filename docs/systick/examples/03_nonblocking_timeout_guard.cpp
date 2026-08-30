/**
 * @file 03_nonblocking_timeout_guard.cpp
 * @brief Example: Guarding asynchronous loops against deadlocks using RAII systick::Timeout
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n--- Non-Blocking Timeout Guard Demo ---\r\n");

    systick::Timeout loopTimer(1000); // 1-second recurring interval

    while(true)
    {
        if(loopTimer.expired())
        {
            loopTimer.restart();
            serial.write("1 second elapsed (non-blocking)\r\n");
        }

        // Other background tasks run unblocked here
    }
}

