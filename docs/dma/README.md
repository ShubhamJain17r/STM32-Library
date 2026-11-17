# DMA (Direct Memory Access) Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The STM32F446RE contains **two independent multi-master DMA controllers** (`DMA1` and `DMA2`):
* **Dual Bus Master Architecture**:
  * **`DMA1`**: Connected to AHB1 peripherals (via AHB/APB1 bus bridge) and system memory (SRAM1, SRAM2). Supports Peripheral-to-Memory and Memory-to-Peripheral transfers for APB1 peripherals (USART2/3, UART4/5, SPI2/3, I2C1/2/3, TIM2-TIM7, DAC1/2).
  * **`DMA2`**: Connected to AHB1/AHB2 peripherals, APB1/APB2 bus bridges, system SRAM, and Flash memory. Supports Peripheral-to-Memory, Memory-to-Peripheral, and **Memory-to-Memory (M2M)** transfers.
* **16 Total Streams**: 8 streams per controller (`Stream0` through `Stream7`), each with its own FIFO and dedicated configuration/status register set.
* **8 Multiplexed Channels**: Each stream provides software-selectable routing across 8 hardware peripheral requests (`Channel0` through `Channel7`).
* **Operating Modes**:
  * **Normal (Single-shot)** mode.
  * **Circular Buffer** mode (auto-reloading `NDTR` for continuous ADC/UART/SPI streaming).
  * **Double-Buffer Mode (DBM)** with ping-pong memory targets (`M0AR` and `M1AR`).
  * **Direct Mode vs FIFO Mode** with 1/4, 1/2, 3/4, or full FIFO threshold packing/unpacking.
* **Deterministic Interrupts**: Hardware IRQ handlers for all 16 streams dispatching to static, lock-free callback tables (Transfer Complete, Half Transfer, and Transfer Error).

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Dual Controllers & 16 Streams** | DMA1 & DMA2, Streams 0..7 | `Dma1Stream0`..`Dma1Stream7`, `Dma2Stream0`..`Dma2Stream7` | 🟢 Implemented |
| **Compile-Time Hardware Traits** | RM0390 Tables 27 & 28 | `Traits<Controller, StreamId>`, `RequestTraits<Request>` | 🟢 Implemented |
| **Memory-to-Memory (M2M)** | DMA2 only (system bus matrix) | `startMemoryToMemory()` with compile-time `static_assert` | 🟢 Implemented |
| **Memory-to-Peripheral (M2P)** | DMA1 (APB1) & DMA2 (APB1/2) | `startTransfer()` / `writeDma()` in UART & SPI | 🟢 Implemented |
| **Peripheral-to-Memory (P2M)** | DMA1 (APB1) & DMA2 (APB1/2) | `startTransfer()` / `startReceiveDma()` / `startDma()` | 🟢 Implemented |
| **Circular Streaming Mode** | `CIRC` bit in `SxCR` | `Mode::Circular` with auto-reload | 🟢 Implemented |
| **Double-Buffer Mode (DBM)** | `DBM` bit with `M0AR` / `M1AR` | `setDoubleBuffer()` with active target query | 🟢 Implemented |
| **FIFO Mode & Thresholds** | 4-word FIFO per stream | Configurable `FifoThreshold::Quarter`..`Full` | 🟢 Implemented |
| **Zero-Allocation ISR Dispatch** | 16 hardware vector handlers | Static type-safe `DmaEvent` callback tables | 🟢 Implemented |
| **Peripheral Integration** | Hardware request gating | Seamless non-blocking methods in `Uart2`, `Spi1`, `Adc1` | 🟢 Implemented |

---

## 3. Configuration & Defaults Breakdown

* **`DmaStreamConfig` Defaults**:
  * `channel`: `ChannelId::Channel0`
  * `direction`: `Direction::MemoryToPeripheral`
  * `priority`: `Priority::Medium`
  * `mode`: `Mode::Normal`
  * `srcSize`: `DataSize::Byte` (8-bit)
  * `dstSize`: `DataSize::Byte` (8-bit)
  * `srcInc`: `true`
  * `dstInc`: `false`
  * `fifoEnable`: `false` (Direct Mode)
  * `fifoThreshold`: `FifoThreshold::Full`
  * `doubleBuffer`: `DoubleBufferMode::Disabled`
  * `flowControl`: `FlowControl::Dma`

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/dma/dma.hpp"
#include "stm32/uart/uart.hpp"
#include "stm32/spi/spi.hpp"
#include "stm32/adc/adc.hpp"

using namespace dma;

// ===========================================================================
// 1. Memory-to-Memory (M2M) Block Transfer (DMA2 Only)
// ===========================================================================
static const std::uint32_t src[16] = { /* data */ };
static std::uint32_t dst[16];

Dma2Stream0 m2m;
m2m.startMemoryToMemory(src, dst, 16, DataSize::Word);
m2m.waitForCompletion(100); // Wait up to 100ms or attach callback

// ===========================================================================
// 2. High-Speed UART TX via DMA
// ===========================================================================
uart::Uart2 serial(115200);

// Non-blocking DMA transmission with completion callback
serial.writeDma("High-speed DMA transmission over USART2\r\n", []() {
    // Callback executed in DMA1_Stream6 ISR upon completion
});

// ===========================================================================
// 3. Continuous Multi-Channel ADC Scan into Circular Buffer
// ===========================================================================
adc::Adc1 adc;
adc.enableChannel(gpio::PA0);

static std::uint16_t adcSamples[64];
adc.startDma(adcSamples, 64, []() {
    // Callback fired each time the circular buffer fills
}, /* circular = */ true);

// ===========================================================================
// 4. Full-Duplex SPI Exchange via Paired DMA Streams
// ===========================================================================
spi::Spi1 spi(10'000'000);

static const std::uint8_t tx[4] = {0x01, 0x02, 0x03, 0x04};
static std::uint8_t rx[4];

spi.transferDma(tx, rx, 4, []() {
    // Both TX and RX complete
});
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **M2M Supported ONLY on DMA2**: In the STM32F4 architecture, only `DMA2` is wired as a bus master to Flash memory and the internal system bus matrix. Any attempt to initialize M2M on `DMA1` is caught at compile time with a `static_assert`.
* **Disabling Sequence & EN Polling**: Per RM0390 §9.3.16, disabling a stream by clearing `DMA_SxCR_EN` does not take effect immediately if a burst or transfer is in progress. The library automatically polls until `EN == 0` before modifying configuration or address registers.
* **Buffer Memory Lifetime**: When initiating asynchronous DMA transfers (`writeDma`, `startTransfer`), the source and destination buffers must remain in valid scope (static, global, or heap-persistent; not temporary stack variables).
* **Data Width Alignment**: When configuring `DataSize::HalfWord` (16-bit) or `DataSize::Word` (32-bit), buffer pointers must be naturally aligned to 2-byte and 4-byte boundaries respectively to prevent alignment bus faults.

---

## 6. Example Catalogue & Links

1. [📖 `01_memory_to_memory.cpp`](examples/01_memory_to_memory.cpp): Zero-CPU 32-bit SRAM block copy using `Dma2Stream0`.
2. [📖 `02_uart_tx_dma.cpp`](examples/02_uart_tx_dma.cpp): Asynchronous non-blocking UART transmission with DMA completion callback.
3. [📖 `03_adc_multi_channel_circular_dma.cpp`](examples/03_adc_multi_channel_circular_dma.cpp): Continuous ADC scan sampling into circular RAM buffer.
4. [📖 `04_spi_duplex_dma.cpp`](examples/04_spi_duplex_dma.cpp): High-speed full-duplex SPI buffer exchange using dual DMA streams.
