# UART / USART Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The STM32F446RE contains **6 hardware serial peripherals**:
* **APB2 High-Speed USARTs (up to 90 MHz)**: `USART1`, `USART6` (Max baud up to 11.25 MBaud).
* **APB1 Standard USARTs/UARTs (up to 45 MHz)**: `USART2`, `USART3`, `UART4`, `UART5` (Max baud up to 5.625 MBaud).
* **Interrupts & Ring Buffers**:
  * Fully asynchronous interrupt-driven TX and RX with static compile-time ring buffers (`RingBuffer<uint8_t, N>`).
  * Dedicated NVIC IRQ vectors for `USART1` through `UART5`.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 6 Instances** | USART1..USART3, UART4..5, USART6 | Aliases `Uart1`..`Uart6`, `Uart1Config`..`Uart6Config` | 🟢 Implemented |
| **Auto-Routing GPIO** | AF7 / AF8 Alternate Function | Auto-configures TX and RX pins | 🟢 Implemented |
| **Ring-Buffered TX/RX** | TXE, RXNE, TC Interrupts | Static `RingBuffer` with non-blocking APIs | 🟢 Implemented |
| **Baud Rate Calculation** | Fractional Baud Generator | Exact `BRR` mantissa/fraction based on live bus clock | 🟢 Implemented |
| **Parity & Data Bits** | 8/9 bits, None/Even/Odd | Scoped enums `WordLength`, `Parity`, `StopBits` | 🟢 Implemented |
| **Non-Blocking TX** | `writeNonBlocking()` | Zero-wait buffer push with immediate return | 🟢 Implemented |
| **Hardware Flow Control** | RTS / CTS Hardware Flow | Planned (Phase 13) | ⚪ Future Scope |
| **DMA Circular Mode** | DMA1/DMA2 Streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Configuration & Defaults Breakdown

* **`uartConfig<Instance>` Defaults**:
  * `baudRate`: `115200`
  * `wordLength`: `WordLength::Bits8`
  * `parity`: `Parity::None`
  * `stopBits`: `StopBits::One`
  * `oversampling`: `Oversampling::By16`
  * `txPin` & `rxPin`: Peripheral defaults (e.g. `PA2`/`PA3` for `USART2`).

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/uart/uart.hpp"

using namespace uart;

// 1. Default Minimal Initialization
Uart2 serial(115200);

// Writing
serial.write("Hello World!\r\n");
serial.writeByte(0x55);
serial.writeNonBlocking(0xAA); // Non-blocking

// Reading
if(serial.available())
{
    char c = serial.read();
    uint8_t b = serial.readByte();
}

// 2. Custom Configuration
Uart1Config cfg;
cfg.txPin = gpio::PB6;
cfg.rxPin = gpio::PB7;
cfg.baudRate = 9600;
cfg.parity = Parity::Even;
cfg.wordLength = WordLength::Bits9;

Uart1 customSerial(cfg);
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Transmission Complete (`TC`) Clearing**: The library automatically clears `USART_SR_TC` in the interrupt dispatcher, preventing CPU lockup.
* **Buffer Overflow**: Check `writeNonBlocking()` return value or ensure ring buffer sizes (`TxBufSize`, `RxBufSize`) match your throughput requirements.

---

## 6. Example Catalogue & Links

1. [📖 `01_blocking_echo_minimal.cpp`](examples/01_blocking_echo_minimal.cpp): Minimal 1-line USART2 echo console.
2. [📖 `02_buffered_interrupt_echo.cpp`](examples/02_buffered_interrupt_echo.cpp): Ring-buffered interrupt-driven full-duplex communication.
3. [📖 `03_custom_baud_parity_config.cpp`](examples/03_custom_baud_parity_config.cpp): Custom baud rate, alternate pins, and even parity configuration.
4. [📖 `04_nonblocking_tx_stream.cpp`](examples/04_nonblocking_tx_stream.cpp): High-speed non-blocking telemetry stream using `writeNonBlocking()`.

