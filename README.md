# STM32 C++17 Hardware Abstraction Library

A modern, type-safe, lightweight, and reusable C++17 peripheral library for STM32 microcontrollers (currently targeting **STM32F446RE**), designed from scratch for bare-metal embedded applications in **STM32CubeIDE**.

---

## 1. Design Philosophy & Core Principles

1. **Zero / Minimal Overhead**: Direct register-level access via `stm32f446xx.h` without third-party dependencies or runtime abstraction penalties.
2. **Compile-Time Safety**: Extensive use of C++17 type system, scoped enums, `constexpr`, template metaprogramming, and traits to prevent invalid configurations at compile time.
3. **Simplicity for Common Cases, Power for Advanced Cases**:
   - Common use case: `DigitalOutput led(PA5); led.high();`
   - Advanced use case: Explicit configuration of pull, speed, output type, oversampling, parity, and ring buffer sizing.
4. **Clean Peripheral Ownership**: Peripherals (UART, I2C, SPI, Timers) own and configure their alternate function pins internally; application code interacts with high-level peripheral objects rather than configuring raw GPIO alternate function registers manually.
5. **Deterministic & Embedded-Friendly**: No dynamic memory allocation (`new`/`delete`), no exceptions, no RTTI. Deterministic interrupt handling and memory footprint.

---

## 2. Architecture & Layering

```
+-------------------------------------------------------------+
|                     Application Code                        |
|   (e.g., Uart2 serial(115200); DigitalOutput led(PA5);)     |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
|                 Peripheral Abstractions                     |
|    - gpio::DigitalOutput, DigitalInput, InterruptInput      |
|    - uart::UartHandler<Instance, TxBufSize, RxBufSize>      |
|    - exti::ExtiManager                                      |
|    - systick::millis, systick::micros, systick::Timeout     |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
|                    Helper & Traits Layer                    |
|    - reg::setBits, clearBits, modifyBits, write, read       |
|    - gpio::helper / gpio::af::Traits / af::helper           |
|    - uart::helper / uart::Traits / uart::interrupt          |
|    - exti::detail / exti::configureExticr                   |
|    - rcc::frequency / rcc::getTimerFrequency                |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
|             CMSIS / Hardware Register Definitions           |
|                   (stm32f446xx.h / ARM NVIC)                |
+-------------------------------------------------------------+
```

---

## 3. Implemented Features & Modules

### 3.1 Common Infrastructure (`stm32/common`)
- **`registers.hpp` (`namespace reg`)**: Zero-overhead, type-safe register manipulation inline templates:
  - `reg::setBits`, `reg::clearBits`, `reg::modifyBits`, `reg::writeField`
  - `reg::readBits`, `reg::isBitSet`, `reg::isAnyBitSet`
  - `reg::write`, `reg::read`
- **`buffer::RingBuffer<T, N>`**: Statically allocated circular buffer with `static_assert(N > 0)` and critical-section protection against concurrent ISR race conditions.
- **`stm32::Callback`**: Standardized lightweight function pointer callback type (`void(*)()`) scoped in `namespace stm32`.
- **`rcc`**: Dynamic hardware register decoding (`CFGR`, `PLLCFGR`) for real-time SYSCLK, HCLK, PCLK1, PCLK2, and timer bus frequencies (`getTimerFrequency()`).
- **`systick`**: High-precision monotonic millisecond and sub-millisecond microsecond timebases (`millis()`, `micros()`), blocking delays (`delayMs()`, `delayUs()`), and RAII non-blocking `systick::Timeout`.

### 3.2 GPIO Subsystem (`stm32/gpio`)
- **`Pin`**: Value-type representation of a physical pin combining `GPIO_TypeDef* port` and `uint8_t number`, with `mask()`, `portIndex()`, `enableClock()`, and equality comparison.
- **Predefined Pin Constants**: Constexpr instances `PA0`..`PA15`, `PB0`..`PB15`, `PC0`..`PC15`, `PD0`..`PD15`, `PE0`..`PE15`, `PF0`..`PF15`, `PG0`..`PG15`, `PH0`..`PH15`.
- **`DigitalOutput`**: Glitch-free output initialization, `write(PinState)`, `high()`, `low()`, `toggle()`, and `read()`. Configurable output type (Push-Pull, Open-Drain), speed, and pull resistor.
- **`DigitalInput`**: Digital input pin with configurable pull-up/pull-down resistors, `read()`, `isHigh()`, `isLow()`.
- **`InterruptInput`**: Edge-triggered external interrupt pin integrating GPIO, SYSCFG, and EXTI with trigger-tracking and RAII cleanup.
- **Alternate Function Subsystem (`gpio::af`)**:
  - Strongly typed `Signal` and `AlternateFunction` enums.
  - Pin capability mapping and compile-time signal validation (`af::Traits<Signal>`).
  - Glitch-free alternate function configuration (`af::configure<Signal>()`).

### 3.3 EXTI & External Interrupts (`stm32/exti`)
- **`ExtiManager`**: Centralized EXTI line dispatch table with automated NVIC IRQ routing (`EXTI0`..`EXTI4`, `EXTI9_5`, `EXTI15_10`).
- **Deterministic Trigger Tracking**: Stores configured `Trigger::Rising`, `Trigger::Falling`, or `Trigger::Both`, eliminating false or missed edges caused by reading `IDR` during short pulses.
- **Multiplexing Collision Protection**: Prevents conflicting port assignments on the same EXTI line.
- **Edge-Aware Callbacks**: Supports `stm32::Callback` and `exti::EdgeCallback` (`void(*)(exti::Edge)`).
- **Software Interrupt Triggering**: Trigger interrupts via `exti::softwareTrigger(line)`.

### 3.4 UART Subsystem (`stm32/uart`)
- **`UartHandler<Instance, TxBufSize, RxBufSize>`**:
  - Full interrupt-driven asynchronous TX and RX with statically allocated ring buffers.
  - Predefined aliases: `Uart1`, `Uart2`, `Uart3`, `Uart4`, `Uart5`, `Uart6`.
  - Configurable baud rate, word length (8/9 bits), parity (None/Even/Odd), stop bits (1/0.5/2/1.5), and oversampling (16x/8x).
  - High-precision baud rate generation with automatic rounding and OVER8 fractional BRR encoding.
  - Symmetrical atomic spin-loops protecting against compiler register caching under `-O2`/`-O3`.
  - Stream methods: `write(const char*)`, `write(const uint8_t*, size_t)`, `read()`, `readByte()`, `available()`.
  - User callbacks: Transfer Complete (`TC`) and Idle Line Detection (`IDLE`).

---

## 4. Examples Catalogue (`Examples/`)

The repository includes ready-to-run examples in the `Examples/` directory:

| Example | Directory | Highlights |
| :--- | :--- | :--- |
| **01. GPIO Blink** | [`Examples/01_gpio_blink`](file:///d:/STM32F446RE/STM32_Library/Examples/01_gpio_blink/main.cpp) | Basic `DigitalOutput` on PA5 with `systick::delayMs`. |
| **02. Button Polling** | [`Examples/02_gpio_button_poll`](file:///d:/STM32F446RE/STM32_Library/Examples/02_gpio_button_poll/main.cpp) | Active-low button polling on PC13 with `DigitalInput`. |
| **03. EXTI Interrupt** | [`Examples/03_exti_interrupt`](file:///d:/STM32F446RE/STM32_Library/Examples/03_exti_interrupt/main.cpp) | Interrupt-driven button on PC13 with falling-edge callback and `__WFI()`. |
| **04. SysTick & Timeout** | [`Examples/04_systick_delay`](file:///d:/STM32F446RE/STM32_Library/Examples/04_systick_delay/main.cpp) | High-resolution microsecond reads and non-blocking `systick::Timeout`. |
| **05. Buffered UART** | [`Examples/05_uart_echo`](file:///d:/STM32F446RE/STM32_Library/Examples/05_uart_echo/main.cpp) | Interrupt-driven USART2 echo with ring buffer character processing. |

---

## 5. Quickstart Code Snippets

### 5.1 Blinking LED with SysTick
```cpp
#include "stm32/common/systick.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    systick::init();
    DigitalOutput led(PA5);

    while(true)
    {
        led.toggle();
        systick::delayMs(500);
    }
}
```

### 5.2 External Interrupt Button
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

DigitalOutput led(PA5);

void onButtonPressed()
{
    led.toggle();
}

int main()
{
    // Configure PC13 as an interrupt input on Falling Edge with callback
    InterruptInput button(PC13, exti::Trigger::Falling, onButtonPressed);

    while(true)
    {
        __WFI(); // Sleep until interrupt
    }
}
```

### 5.3 Interrupt-Driven UART
```cpp
#include "stm32/uart/uart.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace uart;
using namespace gpio;

int main()
{
    Uart2 serial(115200); // 115200 baud, 64-byte TX/RX ring buffers
    DigitalOutput led(PA5);

    serial.write("STM32 C++17 Library Ready\r\n");

    while(true)
    {
        if(serial.available())
        {
            char c = serial.read();
            if(c == '1') led.high();
            if(c == '0') led.low();
            if(c == 't') led.toggle();
        }
    }
}
```

---

## 6. Development Status & Roadmap

| Phase | Module | Status | Description |
| :--- | :--- | :--- | :--- |
| **Phase 0** | **Architecture & Types** | 🟢 Complete | Namespace structure, types, callback model, static dispatch |
| **Phase 1** | **Core Infrastructure** | 🟢 Complete | `RingBuffer`, `Callback`, `registers.hpp` type-safe bit utilities |
| **Phase 2** | **RCC & Clock Tree** | 🟢 Complete | Dynamic PLL/HSE/HSI decoding, AHB/APB1/APB2 & Timer bus frequencies |
| **Phase 3** | **GPIO & Pin Routing** | 🟢 Complete | `DigitalInput`, `DigitalOutput`, `PinMap`, `AF` traits |
| **Phase 4** | **SysTick & Delays** | 🟢 Complete | Millisecond/microsecond timebase, `delayMs`, `delayUs`, `Timeout` |
| **Phase 5** | **EXTI & Events** | 🟢 Complete | Trigger tracking, edge-aware callbacks, multiplexing safety, RAII |
| **Phase 6** | **Timers & PWM** | ⚪ Next Up | Basic, General-purpose, Advanced timers, PWM generation |
| **Phase 7** | **UART / USART** | 🟢 Complete | Full buffered interrupt TX/RX, parity, word length, oversampling |
| **Phase 8** | **SPI** | ⚪ Planned | Master/Slave, polling & interrupt, DMA integration |
| **Phase 9** | **I2C** | ⚪ Planned | Master/Slave 100/400 kHz, trait structures started |
| **Phase 10** | **ADC** | ⚪ Planned | Single channel, scan mode, continuous conversion |
| **Phase 11** | **DMA** | ⚪ Planned | Stream management for UART/SPI/I2C/ADC |
