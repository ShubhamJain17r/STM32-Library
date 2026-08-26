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

## 2. Dedicated Peripheral Documentation (`docs/`)

Each peripheral subsystem has its own exhaustive guide covering hardware architecture, feature matrix, API reference, pin mappings, and syntax examples:

| Module | Documentation Guide | Key Features |
| :--- | :--- | :--- |
| **RCC & Clock Tree** | [📖 `docs/01_rcc_and_clocks.md`](docs/01_rcc_and_clocks.md) | Dynamic SYSCLK, HCLK, PCLK1/2, and timer multiplier decoding (up to 180 MHz). |
| **SysTick Timebase** | [📖 `docs/02_systick_timebase.md`](docs/02_systick_timebase.md) | Monotonic `millis()`, high-res `micros()`, blocking delays, and RAII `Timeout`. |
| **GPIO Subsystem** | [📖 `docs/03_gpio.md`](docs/03_gpio.md) | `DigitalOutput`, `DigitalInput`, `InterruptInput`, pin maps (`PA0`..`PH15`), AF traits. |
| **EXTI Interrupts** | [📖 `docs/04_exti_interrupts.md`](docs/04_exti_interrupts.md) | Race-free trigger tracking, collision safety, edge callbacks, software triggers. |
| **UART / USART** | [📖 `docs/05_uart.md`](docs/05_uart.md) | `Uart1`..`Uart6`, buffered asynchronous TX/RX, parity, oversampling, IDLE/TC events. |
| **Timers & PWM** | [📖 `docs/06_timers_and_pwm.md`](docs/06_timers_and_pwm.md) | All 14 timers (`TIM1`..`TIM14`), Timebase, OPM, multi-channel PWM, OC, Input Capture. |
| **Common Utilities** | [📖 `docs/07_common_utilities.md`](docs/07_common_utilities.md) | `reg::` helpers, static `RingBuffer<T, N>`, standard `stm32::Callback`. |

---

## 3. Quickstart Examples Catalogue (`Examples/`)

Standalone, copy-pasteable bare-metal example applications located in the `Examples/` folder:

| # | Example Project | Source Code | Highlights |
| :-: | :--- | :--- | :--- |
| 01 | **GPIO Blink** | [`Examples/01_gpio_blink/main.cpp`](Examples/01_gpio_blink/main.cpp) | Basic `DigitalOutput` on PA5 with `systick::delayMs`. |
| 02 | **Button Polling** | [`Examples/02_gpio_button_poll/main.cpp`](Examples/02_gpio_button_poll/main.cpp) | Active-low button polling on PC13 with `DigitalInput`. |
| 03 | **EXTI Interrupt** | [`Examples/03_exti_interrupt/main.cpp`](Examples/03_exti_interrupt/main.cpp) | Falling-edge external button interrupt with `__WFI()`. |
| 04 | **SysTick & Timeout** | [`Examples/04_systick_delay/main.cpp`](Examples/04_systick_delay/main.cpp) | High-res microsecond reads & non-blocking `systick::Timeout`. |
| 05 | **Buffered UART** | [`Examples/05_uart_echo/main.cpp`](Examples/05_uart_echo/main.cpp) | Interrupt-driven USART2 echo with ring buffer character processing. |
| 06 | **Timer Interrupt** | [`Examples/06_timer_periodic_interrupt/main.cpp`](Examples/06_timer_periodic_interrupt/main.cpp) | Periodic 2 Hz hardware interrupt using `Timer2`. |
| 07 | **PWM Breathing LED** | [`Examples/07_pwm_led_breathing/main.cpp`](Examples/07_pwm_led_breathing/main.cpp) | Smooth 1 kHz hardware PWM LED breathing on PA6 (`TIM3_CH1`). |
| 08 | **Input Capture** | [`Examples/08_input_capture/main.cpp`](Examples/08_input_capture/main.cpp) | 32-bit input capture timestamp & period measurement on PA0 (`TIM2_CH1`). |

---

## 4. Code Snippets

### Hardware PWM Generation
```cpp
#include "stm32/timer/timer.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace timer;
using namespace gpio;

int main()
{
    // Initialize Timer3 at 20 kHz PWM
    Pwm3 pwm(20'000);

    // Automatically configures PA6 as TIM3_CH1 (AF2 Push-Pull)
    pwm.enableChannel(Channel::CH1, PA6);
    pwm.setDutyCycle(Channel::CH1, 75.0f); // 75% duty cycle
    pwm.start();

    while(true) {}
}
```

### Buffered Interrupt UART
```cpp
#include "stm32/uart/uart.hpp"

using namespace uart;

int main()
{
    // Initialize USART2 at 115200 baud with 64-byte TX/RX ring buffers
    Uart2 serial(115200);

    serial.write("STM32 Bare-Metal C++17 Ready\r\n");

    while(true)
    {
        if(serial.available())
        {
            char c = serial.read();
            serial.write(&c, 1);
        }
    }
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
| **Phase 8** | **SPI Subsystem** | ⚪ Planned | Master/Slave `SPI1`..`SPI4`, polling, interrupt & DMA |
| **Phase 9** | **I2C Subsystem** | ⚪ Planned | Master/Slave `I2C1`..`I2C3` (100 kHz & 400 kHz) |
| **Phase 10** | **ADC Subsystem** | ⚪ Planned | Single channel, scan mode, continuous conversion |
| **Phase 11** | **DMA Subsystem** | ⚪ Planned | High-speed streams for UART, SPI, I2C, ADC |
