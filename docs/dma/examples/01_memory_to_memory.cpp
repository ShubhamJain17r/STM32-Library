/**
 * @file 01_memory_to_memory.cpp
 * @brief Example: High-speed 32-bit SRAM Memory-to-Memory DMA transfer using DMA2 Stream 0
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/dma/dma.hpp"

using namespace dma;
using namespace uart;

constexpr std::size_t BufferSize = 16;
static const std::uint32_t srcData[BufferSize] = {
    0x11111111, 0x22222222, 0x33333333, 0x44444444,
    0x55555555, 0x66666666, 0x77777777, 0x88888888,
    0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC,
    0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x12345678
};
static std::uint32_t dstData[BufferSize] = {0};

static volatile bool transferComplete = false;

void onDmaComplete()
{
    transferComplete = true;
}

int main()
{
    systick::init();
    Uart2 serial(115200);

    serial.write("\r\n========================================\r\n");
    serial.write("     STM32F446RE DMA2 M2M Demo          \r\n");
    serial.write("========================================\r\n");

    // Initialize DMA2 Stream 0 for Memory-to-Memory transfer
    Dma2Stream0 m2m;
    m2m.attachCompleteCallback(onDmaComplete);

    serial.write("Starting 32-bit block copy...\r\n");
    m2m.startMemoryToMemory(srcData, dstData, BufferSize, DataSize::Word);

    // Wait for ISR callback
    while(!transferComplete) {}

    // Verify data integrity
    bool match = true;
    for(std::size_t i = 0; i < BufferSize; ++i)
    {
        if(dstData[i] != srcData[i])
        {
            match = false;
            break;
        }
    }

    if(match)
    {
        serial.write("[SUCCESS] All 16 words copied and verified matching.\r\n");
    }
    else
    {
        serial.write("[ERROR] Destination data mismatch!\r\n");
    }

    while(true)
    {
        systick::delayMs(1000);
    }
}
