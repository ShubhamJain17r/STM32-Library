# GPIO (General Purpose Input/Output) Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The GPIO subsystem manages all 8 GPIO ports on the STM32F446RE:
* **Ports Available**: `GPIOA`, `GPIOB`, `GPIOC`, `GPIOD`, `GPIOE`, `GPIOF`, `GPIOG`, `GPIOH`.
* **Port Pins**: Up to 16 pins per port (`PA0`..`PH15`).
* **Hardware Modes**:
  * **Input**: Floating, Pull-Up, Pull-Down.
  * **Output**: Push-Pull or Open-Drain with programmable slew rates (Low, Medium, Fast, High).
  * **Alternate Function**: AF0 through AF15 multiplexing.
  * **Analog**: Disables digital input buffers for zero leakage on ADC/DAC.
* **Atomic Bit Operations**: High-speed single-cycle set and reset via `BSRR` register.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All Ports & Pins** | GPIOA through GPIOH | `gpio::PA0` through `gpio::PH15` constants | 🟢 Implemented |
| **Digital Output Driver** | Push-Pull, Open-Drain | `DigitalOutput` class (atomic `setHigh()`, `setLow()`, `toggle()`) | 🟢 Implemented |
| **Digital Input Driver** | Input with Pull-Up/Down | `DigitalInput` class (`read()`, `isHigh()`, `isLow()`) | 🟢 Implemented |
| **Interrupt Input Driver** | EXTI edge integration | `InterruptInput` class (Falling/Rising/Both edges + callback) | 🟢 Implemented |
| **Alternate Function Engine** | AF0..AF15 multiplexer | `gpio::af::setAlternateFunction()` & auto-routing traits | 🟢 Implemented |

---

## 3. Configuration & Defaults Breakdown

* **`DigitalOutputConfig` Defaults**:
  * `outputType`: `OutputType::PUSH_PULL`
  * `outputSpeed`: `OutputSpeed::HIGH`
  * `pull`: `Pull::NONE`
  * `initialState`: `PinState::LOW`
* **`DigitalInputConfig` Defaults**:
  * `pull`: `Pull::NONE`

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/gpio/gpio.hpp"

using namespace gpio;

// 1. Digital Output
DigitalOutput led(PA5);
led.setHigh();
led.setLow();
led.toggle();
led.write(PinState::HIGH);

// Custom Output Config
DigitalOutput ledCustom(PA5, DigitalOutputConfig{
    .outputType   = OutputType::OPEN_DRAIN,
    .outputSpeed  = OutputSpeed::MEDIUM,
    .pull         = Pull::UP,
    .initialState = PinState::HIGH
});

// 2. Digital Input
DigitalInput button(PC13, Pull::UP);
if(button.isLow()) { /* Button active */ }

// 3. Interrupt Input (EXTI)
InterruptInput btnIrq(PC13, exti::Trigger::Falling, [](){
    // Button pressed ISR
}, Pull::UP);
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Automatic Clock Enable**: Constructing a `DigitalOutput`, `DigitalInput`, or `InterruptInput` automatically turns on the peripheral clock for that GPIO port in `RCC->AHB1ENR`.
* **Atomic Bit Manipulation**: `setHigh()` and `setLow()` write to `BSRR` directly, guaranteeing re-entrancy and thread-safety without disabling interrupts.

---

## 6. Example Catalogue & Links

1. [📖 `01_digital_output_blink.cpp`](examples/01_digital_output_blink.cpp): High-speed LED toggling on PA5.
2. [📖 `02_digital_input_polling.cpp`](examples/02_digital_input_polling.cpp): Active-low push button polling on PC13.
3. [📖 `03_multi_pin_configuration.cpp`](examples/03_multi_pin_configuration.cpp): Comprehensive multi-pin custom configuration (Open-Drain, Pull-Up, speeds).

