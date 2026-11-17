/**
 * @file 03_adc_multi_channel_circular_dma.cpp
 * @brief Example: Continuous multi-channel ADC scan sampling into circular RAM buffer with DMA2 Stream 0
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/adc/adc.hpp"

using namespace gpio;
using namespace uart;
using namespace adc;

constexpr std::size_t ChannelCount = 2; // PA0 (Analog Voltage) + Internal Temperature
static std::uint16_t adcSamples[ChannelCount] = {0};
static volatile std::uint32_t dmaInterruptCount = 0;

void onAdcDmaComplete()
{
    ++dmaInterruptCount;
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n==============================================\r\n");
    serial.write(" STM32F446RE ADC Continuous Circular DMA Demo\r\n");
    serial.write("==============================================\r\n");

    Adc1 adc;
    adc.enableChannel(PA0);

    const Channel seq[ChannelCount] = {Channel::IN0, Channel::Temperature};
    adc.configureSequence(seq, ChannelCount);

    // Start continuous circular DMA sampling
    adc.startDma(adcSamples, ChannelCount, onAdcDmaComplete, /* circular = */ true);

    serial.write("[OK] ADC1 running in continuous circular DMA mode.\r\n");

    while(true)
    {
        systick::delayMs(1000);

        float volts = (static_cast<float>(adcSamples[0]) * 3.3f) / 4095.0f;
        serial.write("PA0 Raw: ");
        // Samples update automatically in background without CPU polling
        serial.write(volts > 1.5f ? "HIGH\r\n" : "LOW\r\n");
    }
}
