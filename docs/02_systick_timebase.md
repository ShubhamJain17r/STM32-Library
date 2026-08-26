# SysTick Timebase & Delays Subsystem Documentation

## 1. Peripheral Overview (Cortex-M4 SysTick)

The **SysTick (System Timer)** is a 24-bit downcounter integrated into the ARM Cortex-M4 core:
* **Clock Source**: Driven directly by the AHB bus clock (`HCLK`), unaffected by APB peripheral clock prescalers.
* **Resolution**: Dynamic calculation generates precise 1 ms ticks (`tickHz = 1000`) across all system clock configurations.
* **Sub-Millisecond Resolution**: By reading the live countdown register (`SysTick->VAL`) alongside the millisecond tick accumulator, the library provides sub-microsecond precision monotonic time readings without dedicated hardware timer allocation.

---

## 2. Feature Matrix: Implemented vs Hardware Capabilities

| Feature | Cortex-M4 SysTick | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Monotonic Millisecond Counter** | 24-bit counter + ISR | `systick::millis()` (overflows after ~49.7 days) | 🟢 Implemented |
| **Microsecond High-Res Time** | Fractional `VAL` countdown | `systick::micros()` | 🟢 Implemented |
| **Blocking Millisecond Delay** | Polling loop | `systick::delayMs(uint32_t)` | 🟢 Implemented |
| **Blocking Microsecond Delay** | Polling loop | `systick::delayUs(uint32_t)` | 🟢 Implemented |
| **Non-Blocking Timeout Object** | Software abstraction | `systick::Timeout` (RAII tracker) | 🟢 Implemented |
| **Dynamic HCLK Adaptation** | Register reload calculation | Automatic via `rcc::getHCLK()` in `systick::init()` | 🟢 Implemented |

---

## 3. Architecture & Design

* **Zero Memory Footprint**: Single `volatile std::uint32_t systickMillis_` variable in `.bss`.
* **Atomic Read Protection**: `micros()` guards against race conditions where the counter rolls over during the register read.
* **Headers & Sources**:
  * [`Inc/stm32/common/systick/systick.hpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/common/systick/systick.hpp)
  * [`Inc/stm32/common/systick/systick.cpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/common/systick/systick.cpp)

---

## 4. API Reference

### Core Functions
```cpp
namespace systick
{
    void init(std::uint32_t tickHz = 1000) noexcept; // Initializes SysTick (default 1ms)
    void disable() noexcept;                          // Stops SysTick counter and interrupt
    std::uint32_t millis() noexcept;                 // Returns elapsed milliseconds
    std::uint32_t micros() noexcept;                 // Returns elapsed microseconds
    void delayMs(std::uint32_t ms) noexcept;         // Blocking millisecond delay
    void delayUs(std::uint32_t us) noexcept;         // Blocking microsecond delay
}
```

### `systick::Timeout` Class
```cpp
namespace systick
{
    class Timeout
    {
    public:
        explicit Timeout(std::uint32_t durationMs) noexcept;
        bool expired() const noexcept;                  // Returns true if duration has elapsed
        void restart(std::uint32_t newDurationMs) noexcept; // Restarts with a new duration
        void restart() noexcept;                        // Restarts with previous duration
        std::uint32_t elapsed() const noexcept;         // Elapsed ms since start
        std::uint32_t remaining() const noexcept;       // Remaining ms before expiration
    };
}
```

---

## 5. Usage Examples

### Example 1: Basic Delay & Microsecond Profiling
```cpp
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

int main()
{
    systick::init();
    gpio::DigitalOutput led(gpio::PA5);

    while(true)
    {
        led.toggle();
        systick::delayMs(500); // 500 ms delay
    }
}
```

### Example 2: Non-Blocking Task Scheduling with `systick::Timeout`
```cpp
#include "stm32/common/systick/systick.hpp"
#include "stm32/gpio/gpio.hpp"

int main()
{
    systick::init();
    gpio::DigitalOutput led(gpio::PA5);

    systick::Timeout heartBeat(250); // 250ms periodic task

    while(true)
    {
        if(heartBeat.expired())
        {
            led.toggle();
            heartBeat.restart();
        }

        // Other tasks continue to execute with zero blocking
    }
}
```

