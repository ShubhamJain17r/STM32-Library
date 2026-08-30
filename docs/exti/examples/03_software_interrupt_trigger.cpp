/**
 * @file 03_software_interrupt_trigger.cpp
 * @brief Example: Triggering EXTI software interrupt lines on demand
 */

#include "stm32f446xx.h"
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/exti/exti_manager.hpp"

using namespace gpio;
using namespace uart;

static DigitalOutput* pLed = nullptr;

static void onSoftwareTrigger()
{
    if(pLed) pLed->toggle();
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    DigitalOutput led(PA5);
    pLed = &led;

    serial.write("\r\n--- EXTI Software Trigger Demo ---\r\n");

    // Register callback on EXTI Line 4
    exti::ExtiManager::setCallback(4, onSoftwareTrigger);
    exti::ExtiManager::enableLine(4);

    while(true)
    {
        serial.write("Triggering EXTI Line 4 in software...\r\n");
        exti::triggerSoftware(4);

        systick::delayMs(1000);
    }
}

