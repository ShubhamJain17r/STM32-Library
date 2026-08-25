# STM32 C++17 Hardware Abstraction Library

A modern, type-safe, lightweight, and reusable C++17 peripheral library for STM32 microcontrollers (currently targeting **STM32F446RE**), designed from scratch for bare-metal embedded applications in **STM32CubeIDE**.

---

## 1. Design Philosophy & Core Principles

1. **Zero / Minimal Overhead**: Direct register-level access via `stm32f446xx.h` without third-party dependencies or runtime abstraction penalties.
2. **Compile-Time Safety**: Extensive use of C++17 type system, scoped enums, `constexpr`, template metaprogramming, and traits to prevent invalid configurations at compile time.
3. **Simplicity for Common Cases, Power for Advanced Cases**:
   - Common Timebase: `Timer2 timer(1000); timer.setCallback(onTick); timer.start();`
   - Common PWM: `Pwm3 pwm(20000); pwm.enableChannel(Channel::CH1, PA6); pwm.setDutyCycle(Channel::CH1, 75.0f); pwm.start();`
4. **Clean Peripheral Ownership**: Peripherals (UART, Timers, PWM, I2C, SPI) own and configure their alternate function pins internally; application code interacts with high-level peripheral objects rather than configuring raw GPIO alternate function registers manually.
5. **Deterministic & Embedded-Friendly**: No dynamic memory allocation (`new`/`delete`), no exceptions, no RTTI. Deterministic interrupt handling and memory footprint.

---

## 2. Architecture & Layering

```
+-------------------------------------------------------------+
|                     Application Code                        |
|   (e.g., Pwm3 pwm(20000); Timer2 timer(1000); Uart2 serial;) |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
|                 Peripheral Abstractions                     |
|    - timer::TimerHandler<Instance> (Timer1..Timer14)        |
|    - timer::PwmHandler<Instance> (Pwm1..Pwm14)              |
|    - timer::InputCaptureHandler<Instance>                   |
|    - gpio::DigitalOutput, DigitalInput, InterruptInput      |
|    - uart::UartHandler<Instance, TxBufSize, RxBufSize>      |
|    - exti::ExtiManager, systick::millis / systick::micros   |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
|                    Helper & Traits Layer                    |
|    - reg::setBits, clearBits, modifyBits, write, read       |
|    - timer::helper / timer::Traits / timer::af::Traits      |
|    - gpio::helper / gpio::af::Traits / af::helper           |
|    - uart::helper / uart::Traits / uart::interrupt          |
|    - exti::detail / rcc::getTimerFrequency                  |
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

### 3.2 Timers & Hardware PWM Subsystem (`stm32/timer`)
- **Support for all 14 Timers on STM32F446**:
  - **Basic Timers (`TIM6`, `TIM7`)**: 16-bit timebases, DAC triggers, microsecond delays.
  - **General Purpose 32-bit (`TIM2`, `TIM5`)**: High-resolution timebases, 4-channel PWM.
  - **General Purpose 16-bit (`TIM3`, `TIM4`, `TIM9`–`TIM14`)**: 1, 2, or 4 channel PWM, timebase.
  - **Advanced Control (`TIM1`, `TIM8`)**: 4-channel PWM with complementary outputs and automatic `BDTR->MOE` output arming.
- **`TimerHandler<Instance>` (Aliases: `Timer1`..`Timer14`, `BasicTimer6`, `BasicTimer7`)**:
  - Exact frequency configuration in Hz, period in microseconds (`setPeriodUs`), or period in milliseconds (`setPeriodMs`).
  - One-Pulse Mode (`OPM`) for single-pulse delays.
  - Up, Down, and Center-Aligned counting modes.
  - Hardware periodic interrupts with zero-overhead `stm32::Callback` dispatching.
- **`PwmHandler<Instance>` (Aliases: `Pwm1`..`Pwm5`, `Pwm8`..`Pwm14`)**:
  - Multi-channel PWM generation with frequency and duty cycle control (`0.0f` to `100.0f %` or permille `0` to `1000`).
  - Output Compare modes (`ToggleOnMatch`, `ActiveOnMatch`, `InactiveOnMatch`, `Pwm1`, `Pwm2`).
  - **Automatic GPIO Alternate Function configuration**: `pwm.enableChannel(Channel::CH1, PA6)` validates pin compatibility and configures AF registers internally.
- **`InputCaptureHandler<Instance>` (Aliases: `InputCapture1`..`InputCapture5`, `InputCapture8`..`InputCapture14`)**:
  - Frequency and pulse width measurement with automatic AF input configuration.
  - Configurable edge triggers (`Rising`, `Falling`, `Both`) and input prescalers.

### 3.3 GPIO Subsystem (`stm32/gpio`)
- **`Pin`**: Value-type representation of a physical pin combining `GPIO_TypeDef* port` and `uint8_t number`, with `mask()`, `portIndex()`, `enableClock()`, and equality comparison.
- **Predefined Pin Constants**: Constexpr instances `PA0`..`PA15`, `PB0`..`PB15`, `PC0`..`PC15`, `PD0`..`PD15`, `PE0`..`PE15`, `PF0`..`PF15`, `PG0`..`PG15`, `PH0`..`PH15`.
- **`DigitalOutput`**: Glitch-free output initialization, `write(PinState)`, `high()`, `low()`, `toggle()`, and `read()`. Configurable output type (Push-Pull, Open-Drain), speed, and pull resistor.
- **`DigitalInput`**: Digital input pin with configurable pull-up/pull-down resistors, `read()`, `isHigh()`, `isLow()`.
- **`InterruptInput`**: Edge-triggered external interrupt pin integrating GPIO, SYSCFG, and EXTI with trigger-tracking and RAII cleanup.

### 3.4 EXTI & External Interrupts (`stm32/exti`)
- **`ExtiManager`**: Centralized EXTI line dispatch table with automated NVIC IRQ routing (`EXTI0`..`EXTI4`, `EXTI9_5`, `EXTI15_10`).
- **Deterministic Trigger Tracking**: Stores configured `Trigger::Rising`, `Trigger::Falling`, or `Trigger::Both`, eliminating false or missed edges caused by reading `IDR` during short pulses.
- **Multiplexing Collision Protection**: Prevents conflicting port assignments on the same EXTI line.

### 3.5 UART Subsystem (`stm32/uart`)
- **`UartHandler<Instance, TxBufSize, RxBufSize>`**:
  - Full interrupt-driven asynchronous TX and RX with statically allocated ring buffers.
  - Predefined aliases: `Uart1`, `Uart2`, `Uart3`, `Uart4`, `Uart5`, `Uart6`.
  - Configurable baud rate, word length (8/9 bits), parity (None/Even/Odd), stop bits (1/0.5/2/1.5), and oversampling (16x/8x).
  - High-precision baud rate generation with automatic rounding and OVER8 fractional BRR encoding.

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
| **06. Timer Interrupt** | [`Examples/06_timer_periodic_interrupt`](file:///d:/STM32F446RE/STM32_Library/Examples/06_timer_periodic_interrupt/main.cpp) | Periodic 2 Hz hardware interrupt using `Timer2` to toggle user LED. |
| **07. PWM Breathing LED** | [`Examples/07_pwm_led_breathing`](file:///d:/STM32F446RE/STM32_Library/Examples/07_pwm_led_breathing/main.cpp) | Smooth 1 kHz PWM LED breathing on PA6 (`TIM3_CH1`) using `Pwm3`. |
| **08. Input Capture** | [`Examples/08_input_capture`](file:///d:/STM32F446RE/STM32_Library/Examples/08_input_capture/main.cpp) | 32-bit input capture timestamp and frequency measurement on PA0 (`TIM2_CH1`). |

---

## 5. Quickstart Code Snippets

### 5.1 Hardware PWM Generation
```cpp
#include "stm32/timer/pwm.hpp"

using namespace timer;
using namespace gpio;

int main()
{
    // Initialize Timer3 for 20 kHz PWM
    Pwm3 pwm(20'000);

    // Enable Channel 1 on PA6 (automatically configured as AF2 push-pull)
    pwm.enableChannel(Channel::CH1, PA6);
    pwm.setDutyCycle(Channel::CH1, 75.0f); // 75% duty cycle
    pwm.start();

    while(true)
    {
    }
}
```

### 5.2 Hardware Timer Periodic Callback
```cpp
#include "stm32/timer/timer.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace timer;
using namespace gpio;

DigitalOutput led(PA5);

void onHeartbeat()
{
    led.toggle();
}

int main()
{
    // Initialize Timer2 at 5 Hz (fires every 200 ms)
    Timer2 timer(5);
    timer.setCallback(onHeartbeat);
    timer.enableInterrupt();
    timer.start();

    while(true)
    {
        __WFI();
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
| **Phase 6** | **Timers & PWM** | 🟢 Complete | Basic, General-purpose, Advanced timers, PWM, OPM, OC, IC |
| **Phase 7** | **UART / USART** | 🟢 Complete | Full buffered interrupt TX/RX, parity, word length, oversampling |
| **Phase 8** | **SPI** | ⚪ Next Up | Master/Slave, polling & interrupt, DMA integration |
| **Phase 9** | **I2C** | ⚪ Planned | Master/Slave 100/400 kHz, trait structures started |
| **Phase 10** | **ADC** | ⚪ Planned | Single channel, scan mode, continuous conversion |
| **Phase 11** | **DMA** | ⚪ Planned | Stream management for UART/SPI/I2C/ADC |
