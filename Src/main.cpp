#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/timer/timer.hpp"

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

// Timer2 periodic callback: Fires at 2 Hz
void onTimerTick()
{
    if(pLed) pLed->toggle();
}

int main()
{
    // 1. Initialize SysTick monotonic timebase
    systick::init();

    // 2. Configure Green User LED (PA5)
    DigitalOutput led(PA5, DigitalOutputConfig{
        .outputType  = OutputType::PUSH_PULL,
        .outputSpeed = OutputSpeed::HIGH,
        .pull        = Pull::NONE,
        .initialState= PinState::LOW
    });
    pLed = &led;

    // 3. Configure Blue User Button (PC13) with falling-edge EXTI interrupt
    InterruptInput userButton(PC13, exti::Trigger::Falling, onUserButtonPressed, Pull::NONE);

    // 4. Configure USART2 (PA2 TX, PA3 RX) at 115200 baud
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write(" STM32F446RE Bare-Metal C++17 Library   \r\n");
    serial.write(" Complete Peripheral Demonstration      \r\n");
    serial.write("========================================\r\n");

    // 5. Configure Timer2 for 2 Hz periodic update interrupt
    Timer2 timer(2); // 2 Hz frequency
    timer.setCallback(onTimerTick);
    timer.enableInterrupt();
    timer.start();

    // 6. Configure Timer3 for 1 kHz PWM on PA6 (TIM3_CH1)
    Pwm3 pwm(1000); // 1 kHz PWM
    pwm.enableChannel(Channel::CH1, PA6);
    pwm.setDutyCycle(Channel::CH1, pwmDuty);
    pwm.start();
    pPwm = &pwm;

    serial.write("[OK] SysTick, GPIO, EXTI, UART, Timer, and PWM initialized.\r\n");

    systick::Timeout printTimer(1000); // Non-blocking 1s status report

    while(true)
    {
        // Periodic non-blocking telemetry via UART
        if(printTimer.expired())
        {
            printTimer.restart();

            serial.write("Uptime: ");
            char buf[16];
            uint32_t sec = systick::millis() / 1000;
            // Simple integer to ascii
            int idx = 0;
            if(sec == 0) buf[idx++] = '0';
            else
            {
                char tmp[16];
                int tIdx = 0;
                while(sec > 0)
                {
                    tmp[tIdx++] = '0' + (sec % 10);
                    sec /= 10;
                }
                while(tIdx > 0) buf[idx++] = tmp[--tIdx];
            }
            buf[idx++] = 's';
            buf[idx++] = '\r';
            buf[idx++] = '\n';
            buf[idx] = '\0';
            serial.write(buf);
        }

        // Echo UART input
        if(serial.available())
        {
            char c = serial.read();
            serial.write("Echo: ");
            serial.write(reinterpret_cast<const uint8_t*>(&c), 1);
            serial.write("\r\n");
        }
    }
}
