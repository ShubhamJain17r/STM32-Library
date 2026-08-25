#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace uart;

int main()
{
    // 1. Initialize SysTick 1ms timebase
    systick::init();

    // 2. Initialize PA5 (Nucleo green user LED)
    DigitalOutput led(PA5);

    // 3. Initialize USART2 at 115200 baud (connected to ST-LINK Virtual COM port)
    Uart2 serial(115200);

    serial.write("STM32F446RE C++17 Library Ready\r\n");
    serial.write("Send '1' to turn ON, '0' to turn OFF, 't' to toggle\r\n");

    while(true)
    {
        if(serial.available())
        {
            const char c = serial.read();
            const std::uint8_t byte = static_cast<std::uint8_t>(c);
            serial.write(&byte, 1);

            if(c == '1' || c == 'H' || c == 'h')
            {
                led.high();
                serial.write(" -> LED ON\r\n");
            }
            else if(c == '0' || c == 'L' || c == 'l')
            {
                led.low();
                serial.write(" -> LED OFF\r\n");
            }
            else if(c == 't' || c == 'T')
            {
                led.toggle();
                serial.write(" -> LED TOGGLED\r\n");
            }
        }
    }
}
