/**
 * @file main.cpp
 * @brief Example 08: Input Capture Pulse Width / Frequency Measurement
 * 
 * Demonstrates:
 * 1. Initializing InputCapture2 on PA0 (TIM2_CH1, 32-bit counter).
 * 2. Automatic Alternate Function (AF1) GPIO input configuration.
 * 3. Capturing timer counter timestamps on rising edges.
 */

#include "stm32f446xx.h"
#include "stm32/gpio/gpio.hpp"
#include "stm32/timer/input_capture.hpp"
#include "stm32/uart/uart.hpp"

using namespace gpio;
using namespace timer;
using namespace uart;

static volatile std::uint32_t lastCapture = 0;
static volatile std::uint32_t periodTicks = 0;

void onInputCaptured(std::uint32_t captureVal)
{
    periodTicks = captureVal - lastCapture;
    lastCapture = captureVal;
}

int main()
{
    // 1. Initialize InputCapture2 with prescaler = 0 (full timer clock resolution)
    InputCapture2 ic(0);

    // 2. Enable Channel 1 on PA0 for Rising edge capture
    ic.enableChannel(Channel::CH1, PA0, CaptureEdge::Rising);

    // 3. Attach capture callback and enable interrupt
    ic.setCallback(Channel::CH1, onInputCaptured);
    ic.enableInterrupt(Channel::CH1);

    // 4. Start timer capture
    ic.start();

    while(true)
    {
        __WFI();
    }
}

