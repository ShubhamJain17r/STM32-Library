# STM32 C++17 Hardware Abstraction Library

A modern, type-safe, lightweight, and reusable C++17 peripheral library for STM32 microcontrollers (currently targeting **STM32F446RE**), designed from scratch for bare-metal embedded applications in **STM32CubeIDE**.

---

## 1. Design Philosophy & Core Principles

1. **Zero / Minimal Overhead**: Direct register-level access via `stm32f446xx.h` without third-party dependencies or runtime abstraction penalties.
2. **Compile-Time Safety**: Extensive use of C++17 type system, scoped enums, `constexpr`, template metaprogramming, and traits to prevent invalid configurations at compile time.
3. **Simplicity for Common Cases, Power for Advanced Cases**:
   - Common use case: `DigitalOutput led(PA5); led.high();`
   - Advanced use case: Explicit configuration of pull, speed, output type, oversampling, parity, and ring buffer sizing.
4. **Clean Peripheral Ownership**: Peripherals (UART, I2C, SPI) own and configure their alternate function pins internally; application code interacts with high-level peripheral objects rather than configuring raw GPIO alternate function registers manually.
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
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
|                    Helper & Traits Layer                    |
|    - gpio::helper / gpio::af::Traits / af::helper           |
|    - uart::helper / uart::Traits / uart::interrupt          |
|    - exti::detail                                           |
|    - rcc::frequency / rcc::enablePeripheralClock           |
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
- **`buffer::RingBuffer<T, N>`**: Fixed-size, statically allocated circular buffer with `push()`, `pop()`, `front()`, `back()`, `empty()`, `full()`, and `size()`.
- **`stm32::Callback`**: Standardized lightweight function pointer callback type (`void(*)()`) scoped in `namespace stm32`.
- **`rcc`**: Peripheral bus clock definitions (`AHB1`, `AHB2`, `APB1`, `APB2`), frequency queries, and `enablePeripheralClock()` overloads for GPIO, USART, and SYSCFG.
- **`registers.hpp`**: Register manipulation utilities (in progress).

### 3.2 GPIO Subsystem (`stm32/gpio`)
- **`Pin`**: Value-type representation of a physical pin combining `GPIO_TypeDef* port` and `uint8_t number`, with `mask()`, `portIndex()`, `enableClock()`, and equality comparison.
- **Predefined Pin Constants**: Constexpr instances `PA0`..`PA15`, `PB0`..`PB15`, `PC0`..`PC15`, `PD0`..`PD15`, `PE0`..`PE15`, `PF0`..`PF15`, `PG0`..`PG15`, `PH0`..`PH15`.
- **`DigitalOutput`**: Glitch-free output initialization, `write(PinState)`, `high()`, `low()`, `toggle()`, and `read()`. Configurable output type (Push-Pull, Open-Drain), speed, and pull resistor.
- **`DigitalInput`**: Digital input pin with configurable pull-up/pull-down resistors, `read()`, `isHigh()`, `isLow()`.
- **`InterruptInput`**: Edge-triggered external interrupt pin integrating GPIO, SYSCFG, and EXTI with separate rising and falling edge callback registration.
- **Alternate Function Subsystem (`gpio::af`)**:
  - Strongly typed `Signal` and `AlternateFunction` enums.
  - Pin capability mapping and compile-time signal validation (`af::Traits<Signal>`).
  - Glitch-free alternate function configuration (`af::configure<Signal>()`).

### 3.3 UART Subsystem (`stm32/uart`)
- **`UartHandler<Instance, TxBufSize, RxBufSize>`**:
  - Full interrupt-driven asynchronous TX and RX with statically allocated ring buffers.
  - Predefined aliases: `Uart1`, `Uart2`, `Uart3`, `Uart4`, `Uart5`, `Uart6`.
  - Configurable baud rate, word length (8/9 bits), parity (None/Even/Odd), stop bits (1/0.5/2/1.5), and oversampling (16x/8x).
  - High-precision baud rate generation with automatic rounding and OVER8 fractional BRR encoding.
  - Critical-section protected ring buffer accesses for thread-safe concurrent producer-consumer operation.
  - Stream methods: `write(const char*)`, `write(const uint8_t*, size_t)`, `read()`, `readByte()`, `available()`.
  - User callbacks: Transfer Complete (`TC`) and Idle Line Detection (`IDLE`).

### 3.4 EXTI & Interrupts (`stm32/exti`)
- **`ExtiManager`**: Centralized EXTI line dispatch table with automated NVIC IRQ routing (`EXTI0`..`EXTI4`, `EXTI9_5`, `EXTI15_10`).
- Guarded dispatching preventing null-pointer dereferences on uninitialized interrupt lines.

---

## 4. Quickstart Examples

### 4.1 Digital Output (Blinking LED)
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    DigitalOutput led(PA5, {
        .outputType = OutputType::PUSH_PULL,
        .outputSpeed = OutputSpeed::LOW,
        .pull = Pull::NONE,
        .initialState = PinState::LOW
    });

    while(true)
    {
        led.toggle();
        for(volatile uint32_t i = 0; i < 500000; ++i);
    }
}
```

### 4.2 Interrupt-Driven UART
```cpp
#include "stm32/uart/uart.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace uart;
using namespace gpio;

int main()
{
    // Initialize USART2 at 115200 baud with default 64-byte TX/RX ring buffers
    Uart2 serial(115200);
    DigitalOutput led(PA5);

    serial.write("STM32 C++17 Library Ready\r\n");

    while(true)
    {
        if(serial.available())
        {
            char c = serial.read();
            serial.write(&reinterpret_cast<const uint8_t&>(c), 1); // Echo back

            if(c == '1') led.high();
            if(c == '0') led.low();
        }
    }
}
```

### 4.3 External Interrupt Input
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

void onButtonPressed()
{
    // Handle button pressed
}

int main()
{
    InterruptInput button(PC13, {
        .pull = Pull::UP,
        .fallingEdgeCallback = onButtonPressed
    });

    while(true)
    {
        // Main loop
    }
}
```

---

## 5. Development Status & Roadmap

| Phase | Module | Status | Description |
| :--- | :--- | :--- | :--- |
| **Phase 0** | **Architecture & Types** | 🟢 Complete | Namespace structure, types, callback model, static dispatch |
| **Phase 1** | **Core Infrastructure** | 🟢 Complete | `RingBuffer`, `Callback`, `registers.hpp` type-safe bit utilities |
| **Phase 2** | **RCC & Clock Tree** | 🟢 Complete | Dynamic PLL/HSE/HSI decoding, AHB/APB1/APB2 & Timer bus frequencies |
| **Phase 3** | **GPIO & Pin Routing** | 🟢 Complete | `DigitalInput`, `DigitalOutput`, `PinMap`, `AF` traits |
| **Phase 4** | **SysTick & Delays** | 🟢 Complete | Millisecond/microsecond timebase, `delayMs`, `delayUs`, `Timeout` |
| **Phase 5** | **EXTI & Events** | 🟡 Refactoring | Central EXTI manager, edge dispatching improvements |
| **Phase 6** | **Timers & PWM** | ⚪ Planned | Basic, General-purpose, Advanced timers, PWM generation |
| **Phase 7** | **UART / USART** | 🟢 Stable | Full buffered interrupt TX/RX, parity, word length, oversampling |
| **Phase 8** | **SPI** | ⚪ Planned | Master/Slave, polling & interrupt, DMA integration |
| **Phase 9** | **I2C** | ⚪ Planned | Master/Slave 100/400 kHz, trait structures started |
| **Phase 10** | **ADC** | ⚪ Planned | Single channel, scan mode, continuous conversion |
| **Phase 11** | **DMA** | ⚪ Planned | Stream management for UART/SPI/I2C/ADC |
