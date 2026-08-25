#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/timer/timer.hpp"
#include "stm32/timer/pwm.hpp"

using namespace gpio;
using namespace uart;
using namespace timer;

// Pointers for ISR callbacks
static DigitalOutput* pLed = nullptr;
static Pwm3* pPwm = nullptr;
static float pwmDuty = 25.0f;

// EXTI callback: Button press toggles LED and cycles PWM duty
void onUserButtonPressed()
{
    if(pLed) pLed->toggle();

    pwmDuty += 25.0f;
    if(pwmDuty > 100.0f) pwmDuty = 0.0f;

    if(pPwm) pPwm->setDutyCycle(Channel::CH1, pwmDuty);
}

// Timer2 periodic callback (fires at 2 Hz)
void onTimerTick()
{
    // Periodic heartbeat action
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

    // 4. Initialize Hardware PWM on Timer3 (1 kHz) on PA6 (TIM3_CH1)
    Pwm3 pwm(1000);
    pwm.enableChannel(Channel::CH1, PA6);
    pwm.setDutyCycle(Channel::CH1, pwmDuty);
    pwm.start();
    pPwm = &pwm;

    // 5. Initialize Timer2 at 2 Hz periodic interrupts
    Timer2 timer(2);
    timer.setCallback(onTimerTick);
    timer.enableInterrupt();
    timer.start();

    // 6. Initialize USART2 at 115200 baud (connected to ST-LINK Virtual COM port)
    Uart2 serial(115200);

    serial.write("========================================\r\n");
    serial.write("  STM32F446RE C++17 Library Full Demo   \r\n");
    serial.write("========================================\r\n");
    serial.write("Peripherals Active:\r\n");
    serial.write("  * SysTick Timebase (1 ms)\r\n");
    serial.write("  * EXTI Interrupt (PC13 User Button)\r\n");
    serial.write("  * Hardware PWM (Timer3 CH1 @ PA6, 1 kHz)\r\n");
    serial.write("  * Hardware Timebase (Timer2 @ 2 Hz)\r\n");
    serial.write("  * Interrupt-Driven UART2 @ 115200 baud\r\n\r\n");
    serial.write("Commands: [1]=LED ON, [0]=LED OFF, [t]=Toggle, [+]=PWM Up, [-]=PWM Down\r\n\r\n");

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
            else if(c == '+' || c == '=')
            {
                pwmDuty = (pwmDuty <= 90.0f) ? (pwmDuty + 10.0f) : 100.0f;
                pwm.setDutyCycle(Channel::CH1, pwmDuty);
                serial.write(" -> PWM Duty Increased\r\n");
            }
            else if(c == '-' || c == '_')
            {
                pwmDuty = (pwmDuty >= 10.0f) ? (pwmDuty - 10.0f) : 0.0f;
                pwm.setDutyCycle(Channel::CH1, pwmDuty);
                serial.write(" -> PWM Duty Decreased\r\n");
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
