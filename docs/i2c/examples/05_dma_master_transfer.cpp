/**
 * @file 05_dma_master_transfer.cpp
 * @brief Example: High-speed zero-CPU I2C Master Transmit & Receive using DMA1
 */

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/systick/systick.hpp"
#include "stm32/i2c/i2c.hpp"
#include "stm32/uart/uart.hpp"

using namespace i2c;
using namespace uart;

static volatile bool dmaTransferDone = false;

static void onDmaTransferComplete()
{
    dmaTransferDone = true;
}

int main()
{
    // 1. Initialize SysTick millisecond timebase
    systick::init();

    // 2. Initialize telemetry console over USART2 (PA2/PA3 @ 115200 baud)
    Uart2 console(115200);
    console.write("\r\n=== STM32F446 I2C Master DMA Transfer Demo ===\r\n");

    // 3. Initialize I2C1 (PB8=SCL, PB9=SDA @ 400 kHz Fast Mode)
    // Uses DMA1 Stream 6 Channel 1 for TX, and DMA1 Stream 0 Channel 1 for RX per RM0390 Table 28
    I2c1 i2c(400'000);

    // Target slave device (e.g. 24AA02 / 24LC256 EEPROM or OLED display at 0x50)
    constexpr std::uint8_t SLAVE_ADDR = 0x50;

    // Buffer to transmit via DMA
    static const std::uint8_t txPayload[] = {
        0x00, 0x00, // Memory word address
        'H', 'e', 'l', 'l', 'o', ' ', 'D', 'M', 'A', '!', '\0'
    };

    // Buffer to receive via DMA
    static std::uint8_t rxPayload[16] = {0};

    // Verify slave device connectivity before starting DMA transfer
    if(i2c.isDeviceReady(SLAVE_ADDR))
    {
        console.write("[I2C] Slave detected at address 0x50\r\n");

        // --- Demo 1: Synchronous DMA Master Transmit ---
        console.write("[DMA] Starting blocking master transmit via DMA1...\r\n");
        if(i2c.writeDma(SLAVE_ADDR, txPayload, sizeof(txPayload), true, nullptr, 100))
        {
            console.write("[DMA] Transmit completed successfully.\r\n");
        }
        else
        {
            console.write("[DMA] Transmit failed or timed out!\r\n");
        }

        systick::delayMs(10); // Allow EEPROM write cycle to settle

        // --- Demo 2: Synchronous DMA Master Receive ---
        console.write("[DMA] Starting blocking master receive via DMA1...\r\n");
        if(i2c.readDma(SLAVE_ADDR, rxPayload, sizeof(rxPayload), nullptr, 100))
        {
            console.write("[DMA] Receive completed successfully.\r\n");
        }
        else
        {
            console.write("[DMA] Receive failed or timed out!\r\n");
        }

        // --- Demo 3: Asynchronous DMA Master Transmit with Interrupt Callback ---
        dmaTransferDone = false;
        console.write("[DMA] Starting asynchronous master transmit with ISR callback...\r\n");
        i2c.writeDma(SLAVE_ADDR, txPayload, sizeof(txPayload), true, onDmaTransferComplete, 100);

        while(!dmaTransferDone)
        {
            // Main loop CPU remains free for other real-time tasks
            systick::delayMs(1);
        }
        console.write("[DMA] Asynchronous DMA transfer completed via ISR callback!\r\n");
    }
    else
    {
        console.write("[I2C] No device responded at 0x50. Connect an I2C slave to PB8/PB9.\r\n");
    }

    while(true)
    {
        systick::delayMs(1000);
    }
}
