# RCC & Clock Tree Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The Reset and Clock Control (RCC) subsystem manages all oscillator sources, PLL multipliers/dividers, and peripheral clock distributions across the STM32F446RE:
* **Core Frequency**: Up to **180 MHz** SYSCLK / HCLK.
* **Oscillator Sources**:
  * **HSI** (High-Speed Internal): 16 MHz RC oscillator.
  * **HSE** (High-Speed External): 4 to 26 MHz crystal oscillator (typical 8 MHz on Nucleo-F446RE).
  * **LSI** (Low-Speed Internal): 32 kHz RC oscillator (for Watchdog / RTC).
  * **LSE** (Low-Speed External): 32.768 kHz crystal oscillator.
* **Bus Architecture**:
  * **AHB1 / AHB2 / AHB3**: System buses (up to 180 MHz).
  * **APB1 (Low-Speed Peripheral Bus)**: Max frequency **45 MHz**. Connected to TIM2..TIM7, TIM12..TIM14, USART2, USART3, UART4, UART5, SPI2, SPI3, I2C1..I2C3, CAN1..CAN2, DAC, PWR.
  * **APB2 (High-Speed Peripheral Bus)**: Max frequency **90 MHz**. Connected to TIM1, TIM8, TIM9..TIM11, USART1, USART6, SPI1, SPI4, ADC1..ADC3, SDIO, SYSCFG.
* **Timer Clock Multipliers**:
  * When APB prescaler is 1, Timer Clock = PCLK.
  * When APB prescaler is > 1 (e.g., /2 or /4), Timer Clock = $2 \times \text{PCLK}$.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Dynamic Clock Decoding** | Reading `RCC->CFGR`, `RCC->PLLCFGR` | `rcc::getSYSCLK()`, `rcc::getHCLK()`, `rcc::getPCLK1()`, `rcc::getPCLK2()` | 🟢 Implemented |
| **Timer Clock Multipliers** | APB1/APB2 Timer Multipliers | `rcc::getTimerClock(Bus)` | 🟢 Implemented |
| **Dynamic Peripheral Clock Gating** | AHB1, APB1, APB2 ENR registers | Overloaded `rcc::enablePeripheralClock()` templates | 🟢 Implemented |
| **Oscillator Source Detection** | HSI, HSE, PLL_P, PLL_R | Auto-detected inside clock calculation engine | 🟢 Implemented |
| **Runtime PLL Reconfiguration** | Overdrive & PLL switching | Planned (Phase 13) | ⚪ Future Scope |

---

## 3. Configuration & Defaults Breakdown

The RCC driver operates as a zero-cost compile-time and runtime utility:
* **Default Out-of-Reset Clock**: 16 MHz HSI with APB1 = 16 MHz, APB2 = 16 MHz.
* **Nucleo / SystemInit Clock**: Typically 180 MHz PLL with APB1 = 45 MHz, APB2 = 90 MHz.
* **Bus Enum (`rcc::Bus`)**:
  ```cpp
  enum class Bus : std::uint8_t { AHB1, AHB2, AHB3, APB1, APB2 };
  ```

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"

namespace rcc
{
    // Returns system core clock in Hz (e.g. 180'000'000)
    std::uint32_t getSYSCLK() noexcept;

    // Returns Advanced High-Performance Bus clock in Hz
    std::uint32_t getHCLK() noexcept;

    // Returns APB1 peripheral bus clock in Hz (max 45 MHz)
    std::uint32_t getPCLK1() noexcept;

    // Returns APB2 peripheral bus clock in Hz (max 90 MHz)
    std::uint32_t getPCLK2() noexcept;

    // Returns timer clock for given bus (accounts for x2 APB multiplier)
    std::uint32_t getTimerClock(Bus bus) noexcept;

    // Peripheral Clock Gating Overloads
    void enablePeripheralClock(GPIO_TypeDef* port) noexcept;
    void enablePeripheralClock(USART_TypeDef* uart) noexcept;
    void enablePeripheralClock(TIM_TypeDef* timer) noexcept;
    void enablePeripheralClock(SPI_TypeDef* spi) noexcept;
    void enablePeripheralClock(I2C_TypeDef* i2c) noexcept;
    void enablePeripheralClock(ADC_TypeDef* adc) noexcept;
}
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Automatic Enable**: You rarely need to call `rcc::enablePeripheralClock()` manually. High-level peripheral drivers (`UartHandler`, `PwmHandler`, `SpiHandler`, `I2cMaster`, `AdcHandler`, `DigitalOutput`) automatically invoke the appropriate clock gate upon construction.
* **Timer Frequency Gotcha**: When APB1 prescaler is $/4$ (yielding 45 MHz PCLK1 from 180 MHz HCLK), the timer clock is $2 \times 45\text{ MHz} = 90\text{ MHz}$. Always use `rcc::getTimerClock(rcc::Bus::APB1)` when calculating timer prescalers!

---

## 6. Example Catalogue & Links

1. [📖 `01_system_clock_status.cpp`](examples/01_system_clock_status.cpp): Inspects and prints all active system and bus clock frequencies over UART.
2. [📖 `02_custom_pll_frequency_check.cpp`](examples/02_custom_pll_frequency_check.cpp): Demonstrates calculating timer prescalers dynamically from decoded APB1/APB2 bus clocks.

