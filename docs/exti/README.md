# EXTI (Extended Interrupts & Events) Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The EXTI controller manages external and internal event line triggers:
* **Lines 0 to 15**: Connected to GPIO pin indices 0 to 15 via the `SYSCFG->EXTICR` multiplexer.
  * Note: Pin index $N$ across all ports share EXTI line $N$ (e.g. `PA0`, `PB0`, `PC0` all map to line 0; only one port can be mapped to line 0 at a time).
* **NVIC IRQ Vectors**:
  * `EXTI0_IRQn` (Vector 6) -> Line 0
  * `EXTI1_IRQn` (Vector 7) -> Line 1
  * `EXTI2_IRQn` (Vector 8) -> Line 2
  * `EXTI3_IRQn` (Vector 9) -> Line 3
  * `EXTI4_IRQn` (Vector 10) -> Line 4
  * `EXTI9_5_IRQn` (Vector 23) -> Shared Lines 5 through 9
  * `EXTI15_10_IRQn` (Vector 40) -> Shared Lines 10 through 15
* **Edge Triggers**: Rising Edge, Falling Edge, Both Edges.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 16 GPIO Lines** | Lines 0..15 | `exti::ExtiManager` centralized tracker | 🟢 Implemented |
| **Collision Safety** | 1 port per line check | Automatic detection and safe line unmapping | 🟢 Implemented |
| **Edge Triggers** | Rising, Falling, Both | `exti::Trigger` enum (`Rising`, `Falling`, `Both`) | 🟢 Implemented |
| **Software Triggers** | `SWIER` register | `exti::triggerSoftware()` | 🟢 Implemented |
| **High-Level Helper** | RAII Pin + ISR binding | `gpio::InterruptInput` class | 🟢 Implemented |

---

## 3. Configuration & Defaults Breakdown

* **`exti::Trigger` Enum**:
  ```cpp
  enum class Trigger : std::uint8_t { Rising, Falling, Both };
  ```
* **High-Level `InterruptInput`**:
  ```cpp
  InterruptInput btn(PC13, exti::Trigger::Falling, callback, Pull::UP);
  ```

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/exti/exti.hpp"
#include "stm32/gpio/interrupt_input/interrupt_input.hpp"

// 1. High-Level Interrupt Input
gpio::InterruptInput userButton(gpio::PC13, exti::Trigger::Falling, [](){
    // Button pressed callback
});

// 2. Direct EXTI Manager (Low-Level)
exti::ExtiManager::configurePin(gpio::PC13, exti::Trigger::Rising);
exti::ExtiManager::setCallback(13, [](){
    // EXTI Line 13 ISR
});
exti::ExtiManager::enableLine(13);

// Software Trigger
exti::triggerSoftware(13);
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Single Pending Clear**: The library clears the hardware `PR` flag *before* executing the callback, ensuring any newly arrived edges during callback execution are safely latched.
* **Line Sharing**: If you configure `PA5` for EXTI, you cannot concurrently configure `PB5` for EXTI, because both share line 5.

---

## 6. Example Catalogue & Links

1. [📖 `01_falling_edge_button_interrupt.cpp`](examples/01_falling_edge_button_interrupt.cpp): Low-power falling edge push button interrupt waking from `__WFI()`.
2. [📖 `02_rising_edge_sensor_interrupt.cpp`](examples/02_rising_edge_sensor_interrupt.cpp): Rising edge sensor pulse detection.
3. [📖 `03_software_interrupt_trigger.cpp`](examples/03_software_interrupt_trigger.cpp): Manually triggering EXTI lines in software via `SWIER`.

