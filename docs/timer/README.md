# Timers & PWM Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The STM32F446RE contains **14 hardware timers** split across APB1 and APB2:
* **Advanced-Control Timers (`TIM1`, `TIM8`)**: 16-bit, 4 channels, dead-time generation, complementary outputs, repetition counter.
* **General-Purpose 32-bit Timers (`TIM2`, `TIM5`)**: Full 32-bit resolution for multi-hour periods and precision timestamps.
* **General-Purpose 16-bit Timers (`TIM3`, `TIM4`)**: 16-bit, 4 channels with independent PWM/OC/IC.
* **General-Purpose 1/2 Channel Timers (`TIM9`..`TIM14`)**: 16-bit compact timers on APB1/APB2.
* **Basic Timers (`TIM6`, `TIM7`)**: 16-bit pure timebase and DAC trigger generation.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 14 Timers** | TIM1 through TIM14 | Aliases `Timer1`..`Timer14`, `Pwm1`..`Pwm14`, etc. | 🟢 Implemented |
| **Periodic Timebase** | Update Interrupt (UIE) | `TimerHandler` (`setFrequency()`, `setPeriodMs()`, callbacks) | 🟢 Implemented |
| **Hardware PWM** | Edge/Center-aligned PWM | `PwmHandler` (`enableChannel()`, `setDutyCycle()`, auto-AF routing) | 🟢 Implemented |
| **Input Capture** | Frequency / Period meter | `InputCaptureHandler` (`readPeriod()`, `readFrequency()`, `readDuty()`) | 🟢 Implemented |
| **Output Compare** | Timing toggling/set/reset | `OutputCompareHandler` | 🟢 Implemented |
| **One-Pulse Mode (OPM)** | Single precision pulse | `OnePulseHandler` | 🟢 Implemented |

---

## 3. Configuration & Defaults Breakdown

* **Convenient Type Aliases**:
  * `Timer1`..`Timer14` for periodic timebase interrupts.
  * `Pwm1`..`Pwm14` for PWM output generation.
  * `Capture1`..`Capture14` for Input Capture.
  * `Compare1`..`Compare14` for Output Compare.
  * `Pulse1`..`Pulse14` for One-Pulse Mode.
* **Auto-Routing GPIO**: `enableChannel(Channel::CH1, PA6)` automatically looks up the alternate function (`AF2` for `TIM3_CH1`) and sets the pin mode.

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/timer/timer.hpp"

using namespace timer;
using namespace gpio;

// 1. Periodic Timebase
Timer2 timer(2); // 2 Hz frequency
timer.setCallback([](){
    // Periodic ISR
});
timer.enableInterrupt();
timer.start();

// 2. Hardware PWM
Pwm3 pwm(20'000); // 20 kHz PWM
pwm.enableChannel(Channel::CH1, PA6); // Auto-configures PA6 as TIM3_CH1
pwm.setDutyCycle(Channel::CH1, 50.0f); // 50% duty
pwm.start();

// 3. Input Capture
Capture2 ic;
ic.enableChannel(Channel::CH1, PA0);
float freq = ic.readFrequency(Channel::CH1);
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Spurious Interrupts**: The library automatically clears `TIM_SR_UIF` when generating software update events (`TIM_EGR_UG`), preventing immediate false interrupt ticks on start.
* **Floating-Point Duty Cycle**: `setDutyCycle()` accepts percentage floats from `0.0f` to `100.0f` and handles ARR quantization seamlessly.

---

## 6. Example Catalogue & Links

1. [📖 `01_periodic_update_interrupt.cpp`](examples/01_periodic_update_interrupt.cpp): Periodic 2 Hz timer update interrupt.
2. [📖 `02_multi_channel_pwm_led.cpp`](examples/02_multi_channel_pwm_led.cpp): Multi-channel PWM breathing LED on PA6.
3. [📖 `03_input_capture_frequency_meter.cpp`](examples/03_input_capture_frequency_meter.cpp): High-precision 32-bit frequency measurement on PA0 (`TIM2_CH1`).
4. [📖 `04_one_pulse_mode_delay.cpp`](examples/04_one_pulse_mode_delay.cpp): Single precision hardware pulse generation.

