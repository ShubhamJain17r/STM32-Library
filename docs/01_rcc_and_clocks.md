# RCC & Clock Tree Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The **Reset and Clock Control (RCC)** module manages the main system oscillators and distributes clocks across the internal buses of the STM32F446RE microcontroller:
* **Oscillator Sources**: High-Speed Internal (16 MHz HSI), High-Speed External (4–26 MHz HSE, 8 MHz default on Nucleo), Low-Speed Internal (32 kHz LSI), and Low-Speed External (32.768 kHz LSE).
* **Main PLL (Phase-Locked Loop)**: Multiplies and divides input clocks to generate frequencies up to **180 MHz** SYSCLK via `PLL_P` (main system clock) or `PLL_R` (I2S/SAI/DSI clock).
* **Internal Bus Architecture**:
  * **AHB (Advanced High-performance Bus)**: Max 180 MHz, feeds the Cortex-M4 CPU, DMA controllers, and Flash memory.
  * **APB1 (Low-Speed Advanced Peripheral Bus)**: Max 45 MHz, feeds TIM2–TIM7, TIM12–TIM14, USART2, USART3, UART4, UART5, I2C1–I2C3, SPI2, SPI3.
  * **APB2 (High-Speed Advanced Peripheral Bus)**: Max 90 MHz, feeds TIM1, TIM8, TIM9–TIM11, USART1, USART6, SPI1, SPI4, ADC1–ADC3, SYSCFG.
  * **Timer Clock Multipliers**: If APB prescaler is 1, $f_{\text{TIM}} = f_{\text{PCLK}}$; if APB prescaler > 1, $f_{\text{TIM}} = 2 \times f_{\text{PCLK}}$ (yielding up to 90 MHz on APB1 timers and up to 180 MHz on APB2 timers).

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Silicon | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Dynamic SYSCLK Decoding** | HSI, HSE, PLL_P, PLL_R | Decoded at runtime via `rcc::getSystemClock()` | 🟢 Implemented |
| **Dynamic Bus Frequencies** | HCLK, PCLK1, PCLK2 | Decoded at runtime via `rcc::getHCLK()`, `getPCLK1()`, `getPCLK2()` | 🟢 Implemented |
| **Dynamic Timer Frequencies**| APB1/APB2 $\times 1$ / $\times 2$ multiplier | Automatically calculated via `rcc::getTimerFrequency(Bus)` | 🟢 Implemented |
| **Peripheral Clock Enables** | AHB1, AHB2, APB1, APB2 | Type-safe overloads in `rcc::enablePeripheralClock()` | 🟢 Implemented |
| **HSE Frequency Configuration**| External crystal/oscillator | Configurable at runtime via `rcc::setHseFrequency(freq)` | 🟢 Implemented |
| **PLL Configuration API** | Custom PLL multipliers | Planned (Phase 12) | ⚪ Future Scope |
| **RTC / Low Power Clocks** | LSI / LSE / Backup domain | Planned (Phase 13) | ⚪ Future Scope |

---

## 3. Architecture & Design

The RCC subsystem operates in `namespace rcc` with zero RAM overhead. All functions decode hardware status registers (`RCC->CFGR`, `RCC->PLLCFGR`) directly using 64-bit precision integer arithmetic to avoid rounding errors and 32-bit overflow.

### Key Headers
* [`Inc/stm32/common/rcc/rcc.hpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/common/rcc/rcc.hpp): Dynamic frequency getters.
* [`Inc/stm32/common/rcc/rcc_enable.hpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/common/rcc/rcc_enable.hpp): Peripheral clock gating functions.

---

## 4. API Reference

### Constants & Configuration
```cpp
namespace rcc
{
    inline constexpr std::uint32_t HSI_FREQUENCY = 16'000'000U;
    inline constexpr std::uint32_t LSI_FREQUENCY = 32'000U;
    inline constexpr std::uint32_t DEFAULT_HSE_FREQUENCY = 8'000'000U;

    void setHseFrequency(std::uint32_t freq) noexcept;
    std::uint32_t getHseFrequency() noexcept;
}
```

### Dynamic Frequency Decoding
```cpp
namespace rcc
{
    enum class Bus : std::uint8_t { AHB1, AHB2, APB1, APB2 };

    std::uint32_t getSystemClock() noexcept;       // SYSCLK in Hz
    std::uint32_t getHCLK() noexcept;              // AHB HCLK in Hz
    std::uint32_t getPCLK1() noexcept;             // APB1 PCLK1 in Hz
    std::uint32_t getPCLK2() noexcept;             // APB2 PCLK2 in Hz
    std::uint32_t frequency(Bus bus) noexcept;     // Frequency of specified bus in Hz
    std::uint32_t getTimerFrequency(Bus bus) noexcept; // Frequency feeding timers on bus
}
```

### Clock Enable Overloads
```cpp
namespace rcc
{
    void enablePeripheralClock(GPIO_TypeDef* port) noexcept;
    void enablePeripheralClock(USART_TypeDef* port) noexcept;
    void enablePeripheralClock(SYSCFG_TypeDef* port) noexcept;
    void enablePeripheralClock(TIM_TypeDef* timer) noexcept;
}
```

---

## 5. Usage Examples

### Example 1: Reading Live Clock Tree Frequencies
```cpp
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/uart/uart.hpp"

int main()
{
    // If using an external crystal other than 8 MHz:
    // rcc::setHseFrequency(25'000'000U);

    std::uint32_t sysclk   = rcc::getSystemClock();
    std::uint32_t hclk     = rcc::getHCLK();
    std::uint32_t pclk1    = rcc::getPCLK1();
    std::uint32_t pclk2    = rcc::getPCLK2();
    std::uint32_t tim1Freq = rcc::getTimerFrequency(rcc::Bus::APB2);

    while(true) {}
}
```

