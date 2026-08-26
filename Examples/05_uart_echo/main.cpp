/**
 * @file main.cpp
 * @brief Example 05: Buffered Interrupt-Driven UART & Command Processing
 * 
 * Demonstrates:
 * 1. Configuring USART2 (PA2/PA3) at 115200 baud with automatic AF7 alternate function setup.
 * 2. Non-blocking interrupt-driven transmission and reception with static ring buffers.
 * 3. Command parser for LED control over serial terminal.
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace uart;

int main()
{
    // 1. Initialize SysTick timebase
    systick::init();

    // 2. Initialize PA5 user LED
    DigitalOutput led(PA5);

    // 3. Initialize USART2 with 128-byte TX buffer and 128-byte RX buffer
    UartHandler<Instance::usart2, 128, 128> serial(115200);

    serial.write("========================================\r\n");
    serial.write("  STM32F446RE Interrupt UART Demo Ready \r\n");
    serial.write("========================================\r\n");
    serial.write("Commands: [1]=ON, [0]=OFF, [t]=Toggle\r\n\r\n");

    while(true)
    {
        // Check if any bytes have arrived in the RX ring buffer
        if(serial.available())
        {
            const char c = serial.read();

            // Echo the received character back to the terminal
            const std::uint8_t echoByte = static_cast<std::uint8_t>(c);
            serial.write(&echoByte, 1);

            switch(c)
            {
                case '1':
                case 'H':
                case 'h':
                    led.high();
                    serial.write(" -> LED is now ON\r\n");
                    break;

                case '0':
                case 'L':
                case 'l':
                    led.low();
                    serial.write(" -> LED is now OFF\r\n");
                    break;

                case 't':
                case 'T':
                case 't':
                    led.toggle();
                    serial.write(" -> LED Toggled\r\n");
                    break;

                case '\r':
                case '\n':
                    serial.write("\r\n");
                    break;

                default:
                    serial.write(" -> Unknown command\r\n");
                    break;
            }
        }
    }
}
