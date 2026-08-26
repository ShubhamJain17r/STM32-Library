# UART / USART Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The STM32F446RE features **4 USARTs** (`USART1`, `USART2`, `USART3`, `USART6`) and **2 UARTs** (`UART4`, `UART5`):
* **Bus Mapping**:
  * `USART1` and `USART6` operate on **APB2** (up to 90 MHz bus clock, maximum 11.25 MBaud).
  * `USART2`, `USART3`, `UART4`, `UART5` operate on **APB1** (up to 45 MHz bus clock, maximum 5.625 MBaud).
* **Hardware Features**: Fractional baud rate generator, configurable 8-bit or 9-bit word length, parity generation/checking (None, Even, Odd), 1/0.5/2/1.5 stop bits, and $16\times$ / $8\times$ oversampling modes.

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 6 Instances** | USART1..3, UART4..5, USART6 | `Uart1` through `Uart6` aliases | 🟢 Implemented |
| **Asynchronous TX/RX** | Hardware Transceiver | Interrupt-driven with statically allocated `RingBuffer` | 🟢 Implemented |
| **Fractional Baud Rate** | Fractional `BRR` register | Automatic mantissa & fraction encoding via `configureBaudRate()` | 🟢 Implemented |
| **Oversampling Modes** | 16x and 8x (`OVER8`) | Configurable via `Oversampling::BY16` / `BY8` | 🟢 Implemented |
| **Word Length & Parity** | 8/9 bits; None/Even/Odd | Full support in `UartConfig` | 🟢 Implemented |
| **Idle Line Detection** | `IDLE` interrupt | `attachReceiveCompleteCallback()` | 🟢 Implemented |
| **Transmission Complete**| `TC` interrupt | `attachTransferCompleteCallback()` | 🟢 Implemented |
| **Hardware Flow Control**| RTS / CTS lines | Planned (Phase 11 with DMA) | ⚪ Future Scope |
| **DMA Transfers** | DMA1/DMA2 streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Architecture & Directory Layout

```
Inc/stm32/uart/
├── core/
│   ├── uart_types.hpp        <-- Mode, Parity, StopBits, WordLength, Oversampling
│   ├── uart_traits.hpp       <-- Instance mapping, IRQ, APB bus, default pins
│   ├── uart_config.hpp       <-- UartConfig<Instance>
│   └── uart_helper.hpp       <-- Low-level reg:: bit manipulation
├── interrupt/
│   ├── uart_interrupt.hpp    <-- Event dispatch table
│   ├── uart_interrupt.cpp
│   └── uart_irq.cpp          <-- USART1..6 ISR vector handlers
└── uart.hpp                  <-- Master UartHandler<Instance, TxBufSize, RxBufSize>
```

---

## 4. API Reference

### `uart::UartHandler<Instance, TxBufSize, RxBufSize>`
```cpp
namespace uart
{
    template<Instance I, std::size_t TxBufSize = 64, std::size_t RxBufSize = 64>
    class UartHandler
    {
    public:
        explicit UartHandler(std::uint32_t baud = 115200);
        explicit UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud = 115200);
        explicit UartHandler(const UartConfig<I>& config);

        void write(const char* str) noexcept;
        void write(const std::uint8_t* data, std::size_t length) noexcept;

        char read() noexcept;                              // Blocking single char read
        std::uint8_t readByte() noexcept;                  // Blocking single byte read
        void read(std::uint8_t* buf, std::size_t length) noexcept;
        bool available() const noexcept;                   // Returns true if RX buffer has data

        void enable() noexcept;
        void disable() noexcept;

        void attachTransferCompleteCallback(stm32::Callback cb) noexcept;
        void attachReceiveCompleteCallback(stm32::Callback cb) noexcept;
    };

    using Uart1 = UartHandler<Instance::usart1>;
    using Uart2 = UartHandler<Instance::usart2>;
    using Uart3 = UartHandler<Instance::usart3>;
    using Uart4 = UartHandler<Instance::uart4>;
    using Uart5 = UartHandler<Instance::uart5>;
    using Uart6 = UartHandler<Instance::usart6>;
}
```

---

## 5. Usage Examples

### Example 1: Basic Serial Print (USART2 on Nucleo ST-LINK)
```cpp
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    // Initialize USART2 at 115200 baud (connected to virtual COM port)
    Uart2 serial(115200);

    serial.write("Hello STM32F446RE Bare-Metal C++17!\r\n");

    while(true)
    {
        if(serial.available())
        {
            char c = serial.read();
            serial.write("Echo: ");
            serial.write(reinterpret_cast<const uint8_t*>(&c), 1);
            serial.write("\r\n");
        }
    }
}
```

### Example 2: Custom Pin Routing & Buffer Sizing
```cpp
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    // Custom 256-byte RX and 128-byte TX ring buffers on USART1 (PB6 TX, PB7 RX)
    UartConfig<Instance::usart1> cfg;
    cfg.tx = gpio::PB6;
    cfg.rx = gpio::PB7;
    cfg.baud = 921600; // High speed baud rate

    UartHandler<Instance::usart1, 128, 256> highSpeedSerial(cfg);

    highSpeedSerial.write("High-speed link active\r\n");
}
```

