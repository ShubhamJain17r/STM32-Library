# Timers, Timebase & Hardware PWM Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The STM32F446RE contains **14 hardware timers** split across four distinct functional classes:

| Class | Instances | Counter Width | Bus | Channels | Highlights |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Basic** | `TIM6`, `TIM7` | 16-bit | APB1 (90 MHz) | 0 | Pure periodic timebase, DAC triggers, microsecond delays |
| **General Purpose (32-bit)** | `TIM2`, `TIM5` | 32-bit | APB1 (90 MHz) | 4 | Long-duration timebases, high-resolution PWM, encoder |
| **General Purpose (16-bit)** | `TIM3`, `TIM4` | 16-bit | APB1 (90 MHz) | 4 | General 4-channel PWM, output compare, input capture |
| **General Purpose (2-CH)** | `TIM9`, `TIM12` | 16-bit | APB2 / APB1 | 2 | Dual-channel PWM, independent timebase |
| **General Purpose (1-CH)** | `TIM10`, `TIM11`, `TIM13`, `TIM14` | 16-bit | APB2 / APB1 | 1 | Single-channel PWM (LED dimming, buzzer, servo) |
| **Advanced Control** | `TIM1`, `TIM8` | 16-bit | APB2 (180 MHz)| 4 + 3 comp. | Full motor control, complementary PWM with dead-time, `BDTR->MOE` |

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **Coverage for all 14 Timers**| TIM1 through TIM14 | Aliases `Timer1`..`Timer14`, `Pwm1`..`Pwm14`, `InputCapture1`..`InputCapture14` | 🟢 Implemented |
| **Dynamic Frequency Calculation**| Hardware PSC/ARR | Auto-calculated using `rcc::getTimerFrequency(bus)` | 🟢 Implemented |
| **Periodic Interrupts** | Update event (`UIE`) | Zero-overhead `setCallback(stm32::Callback)` | 🟢 Implemented |
| **One-Pulse Mode (OPM)** | `CR1_OPM` bit | `setOnePulseMode(bool)`, hardware `delayUs()` | 🟢 Implemented |
| **Hardware PWM Generation** | Edge/Center aligned PWM | `PwmHandler` with float duty (`0.0`–`100.0%`) and permille (`0`–`1000`) | 🟢 Implemented |
| **Automatic GPIO AF Routing** | Multiplexed AF pins | `enableChannel(Channel, Pin)` validates and configures AF internally | 🟢 Implemented |
| **Output Compare Match Modes** | Toggle, Active, Inactive | `enableOutputCompare(Channel, Pin, OutputCompareMode)` | 🟢 Implemented |
| **Input Capture (Timestamping)**| Edge capture into CCR | `InputCaptureHandler` with rising/falling/both triggers & callback | 🟢 Implemented |
| **Advanced Main Output Enable**| `BDTR->MOE` register | Automatically armed on `TIM1` and `TIM8` | 🟢 Implemented |
| **Quadrature Encoder Mode** | `SMCR_SMS` (Encoder 1/2/3) | Planned (Phase 10) | ⚪ Future Scope |
| **Dead-Time Generator** | `BDTR_DTG` complementary | Planned (Phase 10) | ⚪ Future Scope |

---

## 3. Architecture & Directory Layout

```
Inc/stm32/timer/
├── core/
│   ├── timer_types.hpp        <-- Instance, Channel, CounterMode, PwmMode, Polarity
│   ├── timer_traits.hpp       <-- Instance to register, bus, IRQ, 32-bit traits
│   ├── timer_af_traits.hpp    <-- Pin capability & AF index mapping
│   └── timer_helper.hpp       <-- Low-level reg:: bit manipulation
├── timebase/
│   └── timer.hpp              <-- TimerHandler<Instance> (Timebase, Delays, OPM)
├── pwm/
│   └── pwm.hpp                <-- PwmHandler<Instance> (Multi-channel PWM & OC)
├── input_capture/
│   └── input_capture.hpp      <-- InputCaptureHandler<Instance> (Frequency/Period)
├── interrupt/
│   ├── timer_interrupt.hpp    <-- Callback dispatch table
│   ├── timer_interrupt.cpp
│   └── timer_irq.cpp          <-- TIM1..14 ISR vector handlers
└── timer.hpp                  <-- Master umbrella include
```

---

## 4. API Reference

### `timer::TimerHandler<Instance>` (Timebase & Delays)
```cpp
namespace timer
{
    template<Instance I>
    class TimerHandler
    {
    public:
        explicit TimerHandler(std::uint32_t frequencyHz);
        TimerHandler(std::uint32_t prescaler, std::uint32_t autoReload);

        void setFrequency(std::uint32_t frequencyHz) noexcept;
        void setPeriodUs(std::uint32_t periodUs) noexcept;
        void setPeriodMs(std::uint32_t periodMs) noexcept;

        void setPrescaler(std::uint32_t psc) noexcept;
        void setAutoReload(std::uint32_t arr) noexcept;
        void setOnePulseMode(bool enable) noexcept;
        void setCounterMode(CounterMode mode) noexcept;

        void start() noexcept;
        void stop() noexcept;
        void reset() noexcept;
        std::uint32_t counter() const noexcept;

        void setCallback(stm32::Callback callback) noexcept;
        void enableInterrupt() noexcept;
        void disableInterrupt() noexcept;

        void delayUs(std::uint32_t us) noexcept; // Hardware single-pulse delay
        void delayMs(std::uint32_t ms) noexcept;
    };
}
```

### `timer::PwmHandler<Instance>` (Hardware PWM & Output Compare)
```cpp
namespace timer
{
    template<Instance I>
    class PwmHandler
    {
    public:
        explicit PwmHandler(std::uint32_t frequencyHz);
        void setFrequency(std::uint32_t frequencyHz) noexcept;

        // Automatically verifies pin compatibility and configures AF registers
        bool enableChannel(Channel ch, gpio::Pin pin,
                           Polarity polarity = Polarity::ActiveHigh,
                           PwmMode mode = PwmMode::Mode1) noexcept;

        bool enableOutputCompare(Channel ch, gpio::Pin pin,
                                 OutputCompareMode mode,
                                 Polarity polarity = Polarity::ActiveHigh) noexcept;

        void disableChannel(Channel ch) noexcept;

        void setDutyCycle(Channel ch, float percentage) noexcept;       // 0.0f to 100.0f %
        void setDutyPermille(Channel ch, std::uint16_t permille) noexcept; // 0 to 1000
        void setRawCompare(Channel ch, std::uint32_t ccrValue) noexcept;

        std::uint32_t getRawCompare(Channel ch) const noexcept;
        std::uint32_t getAutoReload() const noexcept;

        void start() noexcept;
        void stop() noexcept;
    };
}
```

### `timer::InputCaptureHandler<Instance>` (Input Capture)
```cpp
namespace timer
{
    template<Instance I>
    class InputCaptureHandler
    {
    public:
        explicit InputCaptureHandler(std::uint32_t prescaler = 0u);
        void setPrescaler(std::uint32_t psc) noexcept;

        bool enableChannel(Channel ch, gpio::Pin pin,
                           CaptureEdge edge = CaptureEdge::Rising,
                           InputPrescaler psc = InputPrescaler::Div1) noexcept;

        void disableChannel(Channel ch) noexcept;
        std::uint32_t getCaptureValue(Channel ch) const noexcept;

        void setCallback(Channel ch, interrupt::ChannelCallback callback) noexcept;
        void enableInterrupt(Channel ch) noexcept;
        void disableInterrupt(Channel ch) noexcept;

        void start() noexcept;
        void stop() noexcept;
    };
}
```

---

## 5. Usage Examples

### Example 1: Hardware PWM on PA6 (`TIM3_CH1`)
```cpp
#include "stm32/timer/pwm/pwm.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace timer;
using namespace gpio;

int main()
{
    // Initialize 20 kHz PWM on Timer3
    Pwm3 pwm(20'000);

    // Automatically validates PA6 as TIM3_CH1 and sets AF2 push-pull
    pwm.enableChannel(Channel::CH1, PA6);

    // Set 50% duty cycle and start PWM output
    pwm.setDutyCycle(Channel::CH1, 50.0f);
    pwm.start();

    while(true) {}
}
```

### Example 2: 10 Hz Periodic Timer Interrupt
```cpp
#include "stm32/timer/timebase/timer.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace timer;
using namespace gpio;

DigitalOutput led(PA5);

void onTimerTick()
{
    led.toggle();
}

int main()
{
    Timer2 timer(10); // 10 Hz (fires every 100 ms)
    timer.setCallback(onTimerTick);
    timer.enableInterrupt();
    timer.start();

    while(true) { __WFI(); }
}
```

