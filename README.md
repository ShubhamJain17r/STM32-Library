# STM32 Bare-Metal C++17 Hardware Abstraction Library

A modern, type-safe, zero-cost, and modular C++17 peripheral library for STM32 microcontrollers (targeting **STM32F446RE**), engineered from scratch for bare-metal embedded applications in **STM32CubeIDE**.

---

## 1. Design Philosophy

1. **Zero Runtime Overhead**: Direct register-level access via `stm32f446xx.h` with inline `reg::` templates; no virtual dispatch, no runtime penalties.
2. **Compile-Time Safety**: Strong type system, scoped enums, `constexpr`, template metaprogramming, and traits to prevent invalid states at compile time.
3. **Ergonomic Simplicity**: Common use cases in 1–2 lines; full configuration available for advanced cases.
4. **Clean Peripheral Ownership**: Peripherals own and configure their Alternate Function GPIO pins internally.
5. **Deterministic & Embedded-Friendly**: No dynamic memory allocation (`new`/`delete`), no exceptions, no RTTI, static memory footprints.

---

## 2. Dedicated Peripheral Documentation Subtrees (`docs/`)

Each peripheral subsystem has its own dedicated directory containing comprehensive documentation, hardware mapping, configuration breakdowns, and standalone multi-configuration example source files:

| Peripheral Subsystem | Documentation Guide | Key Features | Examples Included |
| :--- | :--- | :--- | :--- |
| **RCC & Clocks** | [📖 `docs/rcc/README.md`](docs/rcc/README.md) | Dynamic SYSCLK, HCLK, PCLK1/2, and timer multipliers. | [`docs/rcc/examples/`](docs/rcc/examples/) |
| **SysTick Timebase** | [📖 `docs/systick/README.md`](docs/systick/README.md) | Monotonic `millis()`, `micros()`, delays, `Timeout`. | [`docs/systick/examples/`](docs/systick/examples/) |
| **GPIO Subsystem** | [📖 `docs/gpio/README.md`](docs/gpio/README.md) | `DigitalOutput`, `DigitalInput`, `InterruptInput`, AF. | [`docs/gpio/examples/`](docs/gpio/examples/) |
| **EXTI Interrupts** | [📖 `docs/exti/README.md`](docs/exti/README.md) | Race-free line tracking, collision safety, edge callbacks. | [`docs/exti/examples/`](docs/exti/examples/) |
| **Timers & PWM** | [📖 `docs/timer/README.md`](docs/timer/README.md) | All 14 timers, Timebase, PWM, OC, Input Capture, OPM. | [`docs/timer/examples/`](docs/timer/examples/) |
| **UART / USART** | [📖 `docs/uart/README.md`](docs/uart/README.md) | `Uart1`..`Uart6`, buffered interrupt TX/RX, parity. | [`docs/uart/examples/`](docs/uart/examples/) |
| **SPI Subsystem** | [📖 `docs/spi/README.md`](docs/spi/README.md) | `Spi1`..`Spi4`, full/half duplex, modes 0..3, RAII CS. | [`docs/spi/examples/`](docs/spi/examples/) |
| **I2C Subsystem** | [📖 `docs/i2c/README.md`](docs/i2c/README.md) | `I2c1`..`I2c3`, 100/400 kHz, bus scanner, recovery. | [`docs/i2c/examples/`](docs/i2c/examples/) |
| **ADC Subsystem** | [📖 `docs/adc/README.md`](docs/adc/README.md) | `Adc1`..`Adc3`, 12-bit, scan mode, Temp/VREF, AWD. | [`docs/adc/examples/`](docs/adc/examples/) |
| **Common Utilities** | [📖 `docs/common/README.md`](docs/common/README.md) | `reg::` templates, static `RingBuffer`, `Callback`. | — |

---

## 3. Directory Layout

```
STM32_Library/
├── docs/                      <-- Modular Peripheral Documentation Subtrees
│   ├── rcc/                   <-- RCC documentation & examples
│   ├── systick/               <-- SysTick monotonic timebase documentation & examples
│   ├── gpio/                  <-- GPIO pins & Alternate Function documentation & examples
│   ├── exti/                  <-- EXTI line interrupts documentation & examples
│   ├── timer/                 <-- Hardware Timers, PWM & Input Capture documentation & examples
│   ├── uart/                  <-- UART/USART buffered serial documentation & examples
│   ├── spi/                   <-- SPI full/half duplex documentation & examples
│   ├── i2c/                   <-- I2C master, scanner & recovery documentation & examples
│   ├── adc/                   <-- ADC analog conversion & watchdog documentation & examples
│   └── common/                <-- Register templates & RingBuffer documentation
├── Inc/stm32/                 <-- C++17 Header Implementations
│   ├── common/                <-- Bitwise register templates, RCC, SysTick, RingBuffer, Callback
│   ├── gpio/                  <-- DigitalOutput, DigitalInput, InterruptInput, AF traits
│   ├── exti/                  <-- ExtiManager, line routing, ISR handlers
│   ├── timer/                 <-- Timebase, PWM, OutputCompare, InputCapture, OnePulse
│   ├── uart/                  <-- UartHandler, RingBuffers, interrupt dispatcher
│   ├── spi/                   <-- SpiHandler, ChipSelectGuard, interrupt dispatcher
│   ├── i2c/                   <-- I2cMaster, bus scanner, recovery, interrupt dispatcher
│   └── adc/                   <-- AdcHandler, internal temperature, watchdog, interrupt dispatcher
└── Src/
    └── main.cpp               <-- Unified integration verification test
```

---

## 4. Quickstart Code Snippets

### Hardware ADC Voltage & Internal Temperature
```cpp
#include "stm32/adc/adc.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace adc;
using namespace gpio;

int main()
{
    Adc1 adc;               // Initialize 12-bit ADC1 on APB2
    adc.enableChannel(PA0); // Automatically sets PA0 to Analog Mode

    float volts = adc.readVoltage(PA0, 3.3f);        // 0.0V to 3.3V
    float tempC = adc.readInternalTemperature(3.3f); // Die temp in °C

    while(true) {}
}
```

### Hardware I2C Master & Sensor Read
```cpp
#include "stm32/i2c/i2c.hpp"

using namespace i2c;

int main()
{
    // Initialize I2C1 at 400 kHz Fast Mode (Auto-configures PB8 SCL, PB9 SDA in Open-Drain AF4)
    I2c1 i2c(400'000);

    // Read WHO_AM_I register (0x75) from MPU6050 (0x68)
    std::uint8_t chipId = 0;
    if(i2c.readRegister(0x68, 0x75, chipId))
    {
        // chipId == 0x68
    }

    while(true) {}
}
```

### Hardware SPI Master
```cpp
#include "stm32/spi/spi.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace spi;
using namespace gpio;

int main()
{
    // Initialize SPI1 at 10 MHz in Mode 0 (Auto-configures PA5 SCK, PA6 MISO, PA7 MOSI)
    Spi1 spi(10'000'000, SpiMode::Mode0);

    // Full-duplex single byte exchange
    std::uint8_t rx = spi.transfer(0x55);

    // Buffer transfer
    std::uint8_t tx[4] = {0x01, 0x02, 0x03, 0x04};
    std::uint8_t rxBuf[4];
    spi.transfer(tx, rxBuf, 4);

    while(true) {}
}
```

---

## 5. Development Roadmap

| Phase | Module | Status | Highlights |
| :--- | :--- | :--- | :--- |
| **Phase 0** | **Architecture & Types** | 🟢 Complete | Modular namespaces, type-safe traits, static callbacks |
| **Phase 1** | **Core Infrastructure** | 🟢 Complete | `reg::` helpers, `RingBuffer`, `Callback` |
| **Phase 2** | **RCC & Clock Tree** | 🟢 Complete | Dynamic PLL/HSE/HSI decoding, bus & timer multipliers |
| **Phase 3** | **GPIO Subsystem** | 🟢 Complete | `DigitalOutput`, `DigitalInput`, `PinMap`, `AF` traits |
| **Phase 4** | **SysTick & Delays** | 🟢 Complete | Monotonic timebase, `delayMs`, `delayUs`, `Timeout` |
| **Phase 5** | **EXTI & Events** | 🟢 Complete | Trigger tracking, edge-aware callbacks, collision safety |
| **Phase 6** | **Timers & PWM** | 🟢 Complete | All 14 timers, Timebase, OPM, multi-channel PWM, OC, IC |
| **Phase 7** | **UART / USART** | 🟢 Complete | Full buffered interrupt TX/RX, parity, oversampling |
| **Phase 8** | **SPI Subsystem** | 🟢 Complete | `SPI1`..`SPI4`, modes 0..3, 8/16-bit, polling/async, RAII CS |
| **Phase 9** | **I2C Subsystem** | 🟢 Complete | `I2C1`..`I2C3`, 100/400 kHz, auto-timing, timeout guard, scanner |
| **Phase 10** | **ADC Subsystem** | 🟢 Complete | `ADC1`..`ADC3`, 12-bit, auto-analog GPIO, scan, Temp/VREF, AWD |
| **Phase 11** | **DMA Subsystem** | ⚪ Planned | High-speed streams for UART, SPI, I2C, ADC |
| **Phase 12** | **DAC Subsystem** | ⚪ Planned | Dual 12-bit DAC channels on APB1 |
