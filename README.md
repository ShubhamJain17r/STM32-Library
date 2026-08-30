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
| **SPI Subsystem** | [📖 `docs/08_spi.md`](docs/08_spi.md) | `Spi1`..`Spi4`, full/half duplex, modes 0..3, 8/16-bit, auto-baud, RAII `ChipSelectGuard`. |
| **I2C Subsystem** | [📖 `docs/09_i2c.md`](docs/09_i2c.md) | `I2c1`..`I2c3`, 100/400 kHz, auto-timing, timeout guarding, bus scanner, recovery. |
| **ADC Subsystem** | [📖 `docs/10_adc.md`](docs/10_adc.md) | `Adc1`..`Adc3`, 12-bit, auto-analog GPIO, scan/continuous, internal Temp/VREF, AWD. |

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
| 09 | **SPI Loopback** | [`Examples/09_spi_loopback_poll/main.cpp`](Examples/09_spi_loopback_poll/main.cpp) | 5 MHz full-duplex loopback packet exchange on `SPI1`. |
| 10 | **SPI Sensor / Flash** | [`Examples/10_spi_sensor_read/main.cpp`](Examples/10_spi_sensor_read/main.cpp) | SPI device register read with RAII `ChipSelectGuard`. |
| 11 | **I2C Bus Scanner** | [`Examples/11_i2c_scanner/main.cpp`](Examples/11_i2c_scanner/main.cpp) | Active 7-bit bus scanner (`0x08`–`0x77`) with serial reporting. |
| 12 | **I2C Sensor Read** | [`Examples/12_i2c_sensor_read/main.cpp`](Examples/12_i2c_sensor_read/main.cpp) | 400 kHz Fast Mode sensor register read / write operations. |
| 13 | **ADC Single Channel** | [`Examples/13_adc_single_channel/main.cpp`](Examples/13_adc_single_channel/main.cpp) | 12-bit analog voltage conversion on PA0 with floating-point calculation. |
| 14 | **ADC Internal Sensors**| [`Examples/14_adc_internal_temperature/main.cpp`](Examples/14_adc_internal_temperature/main.cpp) | MCU die temperature sensor & internal $V_{\text{REFINT}}$ measurement. |
| 15 | **ADC Analog Watchdog** | [`Examples/15_adc_analog_watchdog/main.cpp`](Examples/15_adc_analog_watchdog/main.cpp) | Hardware out-of-bounds voltage monitoring with interrupt alert. |

---

## 4. Code Snippets

### Hardware ADC Voltage & Internal Temperature
```cpp
#include "stm32/adc/adc.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace adc;
using namespace gpio;

int main()
{
    // Initialize 12-bit ADC1 on APB2
    Adc1 adc;

    // Enable analog channel on PA0 (Automatically switches PA0 to Analog Mode)
    adc.enableChannel(PA0);

    // Read voltage on PA0 (0.0V to 3.3V)
    float volts = adc.readVoltage(PA0, 3.3f);

    // Read internal die temperature in degrees Celsius
    float tempC = adc.readInternalTemperature(3.3f);

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
