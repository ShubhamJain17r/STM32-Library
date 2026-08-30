/**
 * @file 03_custom_baud_parity_config.cpp
 * @brief Example: Custom baud rate, alternate pins (PB6/PB7), and Even Parity
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace uart;

int main()
{
    systick::init();

    // Configure USART1 on PB6 (TX) and PB7 (RX) with Even Parity at 9600 Baud
    Uart1Config cfg;
    cfg.txPin        = PB6;
    cfg.rxPin        = PB7;
    cfg.baudRate     = 9600;
    cfg.parity       = Parity::Even;
    cfg.wordLength   = WordLength::Bits9; // 8 data bits + 1 parity bit requires 9-bit word
    cfg.stopBits     = StopBits::One;
    cfg.oversampling = Oversampling::By16;

    Uart1 serial(cfg);
    serial.write("--- Custom USART1 9600 Baud 8E1 on PB6/PB7 Ready ---\r\n");

    while(true)
    {
        if(serial.available())
        {
            char c = serial.read();
            serial.writeByte(static_cast<std::uint8_t>(c));
        }
    }
}

