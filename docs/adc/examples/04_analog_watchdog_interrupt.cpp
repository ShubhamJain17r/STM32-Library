/**
 * @file 04_analog_watchdog_interrupt.cpp
 * @brief Example: Hardware Analog Watchdog (AWD) with out-of-bounds voltage alert
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/adc/adc.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace adc;
using namespace uart;

static DigitalOutput* pLed = nullptr;
static volatile bool awdFired = false;

static void onWatchdogAlert()
{
    if(pLed) pLed->toggle();
    awdFired = true;
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    DigitalOutput led(PA5);
    pLed = &led;

    serial.write("--- ADC1 Hardware Analog Watchdog Demo ---\r\n");

    Adc1Config cfg;
    cfg.continuous = true; // Run continuously in background
    Adc1 adc(cfg);

    adc.enableChannel(PA0);

    // Watchdog window: 1.0V (~1241 counts) to 2.5V (~3102 counts)
    adc.enableAnalogWatchdog(Channel::IN0, 1241, 3102, onWatchdogAlert);
    adc.start();

    while(true)
    {
        if(awdFired)
        {
            awdFired = false;
            serial.write("[ALERT] PA0 Voltage Out of Bounds (Window 1.0V - 2.5V)!\r\n");
        }

        systick::delayMs(100);
    }
}

