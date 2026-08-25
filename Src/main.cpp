#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace uart;

// Pointer to user LED for EXTI ISR callback
static DigitalOutput* pLed = nullptr;

// EXTI interrupt callback: fires when the user presses PC13 (blue button)
void onUserButtonPressed()
{
    if(pLed)
    {
        pLed->toggle();
    }
}

int main()
{
    // 1. Initialize SysTick 1ms monotonic timebase
    systick::init();

    // 2. Initialize PA5 (Nucleo green user LED)
    DigitalOutput led(PA5);
    pLed = &led;

    // 3. Initialize PC13 (Nucleo blue user button) as an EXTI interrupt on Falling Edge
    InterruptInput userButton(PC13, exti::Trigger::Falling, onUserButtonPressed, Pull::NONE);

    // 4. Initialize USART2 at 115200 baud (connected to ST-LINK Virtual COM port)
    Uart2 serial(115200);

    serial.write("========================================\r\n");
    serial.write("  STM32F446RE C++17 Library Active\r\n");
    serial.write("========================================\r\n");
    serial.write("Press the Nucleo Blue Button (PC13) to toggle LED (EXTI)\r\n");
    serial.write("Or send commands: [1]=ON, [0]=OFF, [t]=Toggle\r\n\r\n");

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
            else if(c == '\r' || c == '\n')
            {
                serial.write("\r\n");
            }
            else
            {
                serial.write(" -> Unknown command\r\n");
            }
        }
    }
}
