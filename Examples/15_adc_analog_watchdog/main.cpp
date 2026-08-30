/**
 * @file main.cpp
 * @brief Example 15: ADC Hardware Analog Watchdog (AWD) & Threshold Alert
 * 
 * Demonstrates:
 * 1. Initializing ADC1 in continuous conversion mode on PA0.
 * 2. Configuring the hardware Analog Watchdog (AWD) to monitor PA0 between 1.0V and 2.5V.
 * 3. Registering an interrupt callback that toggles the on-board LED (PA5) whenever the input leaves the valid band.
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/adc/adc.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace adc;
using namespace uart;

static DigitalOutput* pLed = nullptr;
static volatile bool watchdogAlert = false;

static void onAnalogWatchdogTrigger()
{
    if(pLed) pLed->toggle();
    watchdogAlert = true;
}

int main()
{
    // 1. Initialize SysTick monotonic timebase
    systick::init();

    // 2. Configure On-Board LED (PA5)
    DigitalOutput led(PA5);
    pLed = &led;

    // 3. Initialize USART2 for serial telemetry
    Uart2 serial(115200);
    serial.write("\r\n========================================\r\n");
    serial.write("   STM32F446RE Analog Watchdog Demo     \r\n");
    serial.write("========================================\r\n");
    serial.write("Thresholds: Low = 1.0 V (~1241 counts), High = 2.5 V (~3102 counts)\r\n\r\n");

    // 4. Configure ADC1 in Continuous Mode
    Adc1Config config;
    config.continuous = true;
    config.resolution = Resolution::Bits12;

    Adc1 adc(config);
    adc.enableChannel(PA0);

    // 5. Configure Analog Watchdog on Channel 0 (PA0)
    // Low: 1.0V -> (1.0 / 3.3) * 4095 = 1241
    // High: 2.5V -> (2.5 / 3.3) * 4095 = 3102
    constexpr std::uint16_t LOW_THRESH  = 1241;
    constexpr std::uint16_t HIGH_THRESH = 3102;

    adc.enableAnalogWatchdog(Channel::IN0, LOW_THRESH, HIGH_THRESH, onAnalogWatchdogTrigger);
    adc.start();

    while(true)
    {
        if(watchdogAlert)
        {
            watchdogAlert = false;
            serial.write("[ALERT] Voltage out of bounds on PA0!\r\n");
        }

        systick::delayMs(100);
    }
}

