# EXTI (Extended Interrupts and Events) Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The **EXTI (Extended Interrupts and Events Controller)** manages up to 23 edge detector lines on the STM32F446RE:
* **Lines 0 to 15**: Connected to physical GPIO pins via the `SYSCFG_EXTICR` multiplexer (e.g. Line 0 can be mapped to PA0, PB0, PC0... PH0).
* **Trigger Selection**: Independent rising-edge and falling-edge detection circuitry per line.
* **NVIC IRQ Routing**:
  * Lines 0..4 have dedicated vectors (`EXTI0_IRQn` through `EXTI4_IRQn`).
  * Lines 5..9 share `EXTI9_5_IRQn`.
  * Lines 10..15 share `EXTI15_10_IRQn`.
* **Internal Event Lines**: Lines 16..22 connect to PVD, RTC alarm, USB OTG wakeup, Ethernet wakeup, etc.

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Lines 0 to 15 (GPIO)** | SYSCFG multiplexed mapping | `ExtiManager` + `InterruptInput` | 🟢 Implemented |
| **Trigger Modes** | Rising, Falling, Both | `exti::Trigger` enum with bitwise operators | 🟢 Implemented |
| **Deterministic Trigger Tracking** | Hardware `PR` flag | Stored per-line in `ExtiManager`, immune to `IDR` race conditions | 🟢 Implemented |
| **Pin Collision Protection** | 1 pin per line limit | Validated in `ExtiManager::registerPin()` | 🟢 Implemented |
| **Software Interrupt Trigger**| `SWIER` register | `exti::softwareTrigger(line)` | 🟢 Implemented |
| **Edge-Aware Callbacks** | Rising / Falling distinction | `exti::EdgeCallback` (`void(*)(Edge)`) | 🟢 Implemented |
| **Internal Lines 16–22** | PVD, RTC, USB | Planned (Phase 13) | ⚪ Future Scope |

---

## 3. Architecture & Design

* **Race-Free Edge Dispatch**: Because physical button contacts bounce and short digital pulses might have already changed when the ISR reads the GPIO `IDR` register, `ExtiManager` dispatches callbacks based on the **configured trigger**, eliminating false or missed edges.
* **Headers & Sources**:
  * [`Inc/stm32/exti/exti_types.hpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/exti/exti_types.hpp)
  * [`Inc/stm32/exti/exti_manager.hpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/exti/exti_manager.hpp)
  * [`Inc/stm32/exti/exti_manager.cpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/exti/exti_manager.cpp)
  * [`Inc/stm32/exti/exti_irq.cpp`](file:///d:/STM32F446RE/STM32_Library/Inc/stm32/exti/exti_irq.cpp)

---

## 4. API Reference

### Enums & Types
```cpp
namespace exti
{
    enum class Trigger : std::uint8_t { None = 0, Rising = 1, Falling = 2, Both = 3 };
    enum class Edge : std::uint8_t { Rising, Falling };
    using EdgeCallback = void(*)(Edge);
}
```

### Low-Level Register Functions
```cpp
namespace exti
{
    void configureExticr(const gpio::Pin& pin) noexcept;
    void enableInterrupt(const gpio::Pin& pin) noexcept;
    void disableInterrupt(const gpio::Pin& pin) noexcept;
    void setTrigger(const gpio::Pin& pin, Trigger trigger) noexcept;
    void clearPending(std::uint8_t line) noexcept;
    bool isPending(std::uint8_t line) noexcept;
    void softwareTrigger(std::uint8_t line) noexcept;
    void enableIRQ(const gpio::Pin& pin) noexcept;
    void disableIRQ(const gpio::Pin& pin) noexcept;
}
```

---

## 5. Usage Examples

### Example 1: Interrupt-Driven User Button (Falling Edge)
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

DigitalOutput led(PA5);

void onButtonPressed()
{
    led.toggle();
}

int main()
{
    // Configure PC13 as an external interrupt on falling edge
    InterruptInput button(PC13, exti::Trigger::Falling, onButtonPressed);

    while(true)
    {
        __WFI(); // Sleep until interrupt
    }
}
```

### Example 2: Dual-Edge Callback with Edge Type
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

void onEdgeDetected(exti::Edge edge)
{
    if(edge == exti::Edge::Rising) {
        // Line transitioned LOW -> HIGH
    } else {
        // Line transitioned HIGH -> LOW
    }
}

int main()
{
    InterruptInputConfig cfg;
    cfg.trigger = exti::Trigger::Both;
    cfg.edgeCallback = onEdgeDetected;

    InterruptInput sensor(PA0, cfg);

    while(true) { __WFI(); }
}
```

