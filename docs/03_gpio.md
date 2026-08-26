# GPIO (General-Purpose Input/Output) Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The **GPIO** subsystem on the STM32F446RE controls up to 50 physical I/O pins (on LQFP64 Nucleo) divided across Ports A through H (`GPIOA`–`GPIOH`):
* **Operating Modes**: Input (floating, pull-up, pull-down), Output (push-pull, open-drain), Alternate Function (AF0–AF15), and Analog.
* **Speed Grades**: Low (2 MHz), Medium (25 MHz), Fast (50 MHz), High (100 MHz).
* **Atomic Bit Set/Reset**: Dedicated `BSRR` register for glitch-free set and reset operations without read-modify-write races.
* **5V-Tolerant I/Os**: Most pins tolerate up to 5V inputs when configured in digital input or alternate function mode.

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Digital Output Pin** | Push-Pull, Open-Drain, 4 Speeds | `gpio::DigitalOutput` | 🟢 Implemented |
| **Atomic Bit Operations** | `BSRR` atomic set/reset | `high()`, `low()`, `write(PinState)` | 🟢 Implemented |
| **Digital Input Pin** | Floating, Pull-up, Pull-down | `gpio::DigitalInput` (`read()`, `isHigh()`, `isLow()`) | 🟢 Implemented |
| **External Interrupt Input**| EXTI integration | `gpio::InterruptInput` (RAII trigger tracking & dispatch) | 🟢 Implemented |
| **Alternate Function Routing**| AF0 through AF15 | `gpio::af` Traits & automatic peripheral routing | 🟢 Implemented |
| **Analog Mode (ADC/DAC)** | Pin isolated from digital logic | Planned (Phase 10) | ⚪ Future Scope |
| **Pin Locking (LCKR)** | Hardware configuration lock | Planned (Phase 14) | ⚪ Future Scope |

---

## 3. Architecture & Directory Layout

```
Inc/stm32/gpio/
├── core/
│   ├── gpio_types.hpp        <-- Mode, OutputType, OutputSpeed, Pull, PinState
│   ├── pin.hpp               <-- Pin struct (port pointer + pin index)
│   ├── pin_map.hpp           <-- Predefined PA0..PA15, PB0..PB15, etc.
│   └── gpio_helper.hpp       <-- Low-level reg:: bit manipulation
├── digital_output/
│   ├── digital_output.hpp
│   └── digital_output.cpp
├── digital_input/
│   ├── digital_input.hpp
│   └── digital_input.cpp
├── interrupt_input/
│   ├── interrupt_input.hpp
│   └── interrupt_input.cpp
├── af/
│   ├── af_types.hpp          <-- Signal, AlternateFunction enums
│   ├── af_traits.hpp         <-- Signal-to-pin mapping
│   ├── af_helper.hpp         <-- AFR register setters
│   └── af_config.hpp         <-- AlternatePinConfig
└── gpio.hpp                  <-- Master umbrella include
```

---

## 4. API Reference

### `gpio::DigitalOutput`
```cpp
namespace gpio
{
    struct DigitalOutputConfig
    {
        OutputType  outputType   = OutputType::PUSH_PULL;
        OutputSpeed outputSpeed  = OutputSpeed::LOW;
        Pull        pull         = Pull::NONE;
        PinState    initialState = PinState::LOW;
    };

    class DigitalOutput
    {
    public:
        explicit DigitalOutput(Pin pin, DigitalOutputConfig config = {});
        void write(PinState state) const noexcept;
        void high() const noexcept;
        void low() const noexcept;
        void toggle() const noexcept;
        PinState read() const noexcept;
    };
}
```

### `gpio::DigitalInput`
```cpp
namespace gpio
{
    class DigitalInput
    {
    public:
        explicit DigitalInput(Pin pin, Pull pull = Pull::NONE);
        PinState read() const noexcept;
        bool isHigh() const noexcept;
        bool isLow() const noexcept;
    };
}
```

### `gpio::InterruptInput`
```cpp
namespace gpio
{
    struct InterruptInputConfig
    {
        Pull               pull            = Pull::NONE;
        exti::Trigger      trigger         = exti::Trigger::Falling;
        stm32::Callback    callback        = nullptr;
        exti::EdgeCallback edgeCallback    = nullptr;
        stm32::Callback    risingCallback  = nullptr;
        stm32::Callback    fallingCallback = nullptr;
    };

    class InterruptInput
    {
    public:
        explicit InterruptInput(Pin pin, InterruptInputConfig config = {});
        InterruptInput(Pin pin, exti::Trigger trigger, stm32::Callback cb, Pull pull = Pull::NONE);
        PinState read() const noexcept;
        bool isHigh() const noexcept;
        bool isLow() const noexcept;
        void enable() const noexcept;
        void disable() const noexcept;
        void softwareTrigger() const noexcept;
    };
}
```

---

## 5. Usage Examples

### Example 1: Output Control
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    // Fast push-pull output initialized HIGH
    DigitalOutput led(PA5, DigitalOutputConfig{
        .outputType  = OutputType::PUSH_PULL,
        .outputSpeed = OutputSpeed::HIGH,
        .pull        = Pull::NONE,
        .initialState= PinState::HIGH
    });

    led.low();
    led.high();
    led.toggle();
}
```

### Example 2: Polling Active-Low Button
```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
    DigitalOutput led(PA5);
    DigitalInput button(PC13, Pull::NONE); // Internal pull-up if needed

    while(true)
    {
        if(button.isLow()) {
            led.high(); // Button pressed
        } else {
            led.low();
        }
    }
}
```

