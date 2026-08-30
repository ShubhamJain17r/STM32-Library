# SysTick & Monotonic Timebase Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The ARM Cortex-M4 Core SysTick timer provides high-resolution timekeeping and delay primitives:
* **Clock Source**: System Core Clock ($f_{\text{CPU}}$, up to 180 MHz).
* **Interrupt Rate**: Configured for **1 kHz** (1 ms period) by default.
* **Resolution**:
  * **Millisecond**: 1 ms granularity via `systick::millis()`.
  * **Microsecond**: Sub-microsecond precision via `systick::micros()` using `SysTick->VAL` interpolation.
* **Wrap Compensation**: Monotonic guarantee even during masked interrupts.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | Cortex-M4 SysTick | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Monotonic Millisecond Counter** | 24-bit downcounter + ISR | `systick::millis()` | 🟢 Implemented |
| **High-Res Microsecond Timer** | Sub-tick `VAL` interpolation | `systick::micros()` | 🟢 Implemented |
| **Blocking Delays** | Millisecond & Microsecond | `systick::delayMs()`, `systick::delayUs()` | 🟢 Implemented |
| **RAII Non-Blocking Timeout Guard** | Elapsed time tracking | `systick::Timeout` class | 🟢 Implemented |

---

## 3. Configuration & Defaults Breakdown

* **Initialization**: Single call `systick::init()` configures `SysTick->LOAD = (SystemCoreClock / 1000) - 1`, enables SysTick exception, and starts the counter.
* **RAII `Timeout` Class**:
  ```cpp
  systick::Timeout timer(500); // 500 ms deadline
  if(timer.expired()) { timer.restart(); }
  ```

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/common/systick/systick.hpp"

namespace systick
{
    void init() noexcept;

    std::uint32_t millis() noexcept;
    std::uint32_t micros() noexcept;

    void delayMs(std::uint32_t ms) noexcept;
    void delayUs(std::uint32_t us) noexcept;

    class Timeout
    {
    public:
        explicit Timeout(std::uint32_t durationMs) noexcept;
        bool expired() const noexcept;
        void restart() noexcept;
        void reset(std::uint32_t durationMs) noexcept;
        std::uint32_t elapsed() const noexcept;
    };
}
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Deadlock Protection**: Never write a bare `while(!ready)` loop in drivers. Always wrap polling in `systick::Timeout to(timeoutMs); while(!ready && !to.expired())`.
* **ISR Delays**: Avoid calling `delayMs()` inside high-priority ISRs as it wastes CPU cycles.

---

## 6. Example Catalogue & Links

1. [📖 `01_blocking_delays.cpp`](examples/01_blocking_delays.cpp): Basic LED blinking and microsecond delays.
2. [📖 `02_microsecond_profiling.cpp`](examples/02_microsecond_profiling.cpp): Benchmarking execution time of code routines with `micros()`.
3. [📖 `03_nonblocking_timeout_guard.cpp`](examples/03_nonblocking_timeout_guard.cpp): Guarding communication loops with non-blocking RAII `Timeout`.

