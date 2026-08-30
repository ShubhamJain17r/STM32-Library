# ADC (Analog-to-Digital Converter) Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The STM32F446RE microcontroller features **3 independent 12-bit successive-approximation ADCs** (`ADC1`, `ADC2`, `ADC3`):
* **Bus Architecture**:
  * Connected to **APB2** (bus clock up to 90 MHz).
  * Prescalers: $/2, /4, /6, /8$ via Common `ADC->CCR` (typical 22.5 MHz at $/4$).
* **Resolutions & Conversion Times**:
  * 12-bit (15 ADCCLK cycles), 10-bit (13 cycles), 8-bit (11 cycles), 6-bit (9 cycles).
* **Channel Multiplexing**:
  * 16 external channels (`IN0` through `IN15` on GPIOA, GPIOB, GPIOC).
  * 3 internal channels on `ADC1`:
    * Internal Reference Voltage ($V_{\text{REFINT}} \approx 1.21\text{V}$ on `IN17`).
    * Internal Temperature Sensor ($T_{\text{SENSE}}$ on `IN18`).
    * Battery Voltage Monitor ($V_{\text{BAT}} / 4$ on `IN18`).
* **Conversion Modes**:
  * Single Conversion, Continuous Conversion, and Scan Mode (Sequencing up to 16 channels).
* **Hardware Analog Watchdog (AWD)**:
  * Programmable High (`HTR`) and Low (`LTR`) voltage threshold boundaries with interrupt alert.

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 3 Instances** | ADC1, ADC2, ADC3 | Aliases `Adc1`, `Adc2`, `Adc3` | 🟢 Implemented |
| **12-bit / 10-bit / 8-bit / 6-bit** | Programmable `RES[1:0]` | Configurable via `Resolution` enum | 🟢 Implemented |
| **Automatic GPIO Analog Routing**| `Mode::ANALOG`, `Pull::NONE` | Auto-configured upon `enableChannel()` | 🟢 Implemented |
| **Single Channel Reading** | Blocking Polling | `readRaw()` and floating-point `readVoltage()` | 🟢 Implemented |
| **Internal Temperature Sensor** | $T_{\text{SENSE}}$ on ADC1 | `readInternalTemperature()` (in $^\circ\text{C}$) | 🟢 Implemented |
| **Internal $V_{\text{REFINT}}$ & $V_{\text{BAT}}$** | Internal channels 17 & 18 | `readVrefInternal()` & `readVbat()` | 🟢 Implemented |
| **Multi-Channel Sequence / Scan** | `SQR1`..`SQR3` (up to 16 ranks) | `configureSequence()` & `readSequence()` | 🟢 Implemented |
| **Hardware Analog Watchdog** | `HTR` / `LTR` thresholds | `enableAnalogWatchdog()` with callback | 🟢 Implemented |
| **Centralized Interrupt Dispatch**| Shared `ADC_IRQn` | `AdcEvent::handleInterrupt()` | 🟢 Implemented |
| **Injected Channels** | `JSQR` injected sequence | Planned (Phase 14) | ⚪ Future Scope |
| **DMA Multi-Channel Circular** | DMA2 Streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Architecture & Directory Layout

```
Inc/stm32/adc/
├── core/
│   ├── adc_types.hpp          <-- Enums: Instance, Channel, Resolution, SamplingTime, Alignment, Trigger
│   ├── adc_traits.hpp         <-- Traits<Instance>: peripheral, APB2 bus, IRQ number, common CCR
│   ├── adc_pin_traits.hpp     <-- Pin-to-Channel & Channel-to-Pin capability mappings
│   ├── adc_config.hpp         <-- AdcConfig<Instance> struct with ergonomic constructors
│   └── adc_helper.hpp         <-- Zero-overhead inline register manipulation functions
├── handler/
│   └── adc_handler.hpp        <-- AdcHandler<Instance> (Single-channel, Scan, Voltage/Temp conversion, AWD)
├── interrupt/
│   ├── adc_interrupt.hpp      <-- Event and Watchdog callback dispatch layer
│   ├── adc_interrupt.cpp      <-- Event dispatcher implementation
│   └── adc_irq.cpp            <-- Centralized ADC_IRQHandler vector
└── adc.hpp                    <-- Master umbrella header
```

---

## 4. Hardware Pin Mappings (STM32F446RE)

| Channel | Pin | ADC1 | ADC2 | ADC3 |
| :--- | :--- | :---: | :---: | :---: |
| **`IN0`** | `PA0` | ✔ | ✔ | ✔ |
| **`IN1`** | `PA1` | ✔ | ✔ | ✔ |
| **`IN2`** | `PA2` | ✔ | ✔ | ✔ |
| **`IN3`** | `PA3` | ✔ | ✔ | ✔ |
| **`IN4`** | `PA4` | ✔ | ✔ | — |
| **`IN5`** | `PA5` | ✔ | ✔ | — |
| **`IN6`** | `PA6` | ✔ | ✔ | — |
| **`IN7`** | `PA7` | ✔ | ✔ | — |
| **`IN8`** | `PB0` | ✔ | ✔ | — |
| **`IN9`** | `PB1` | ✔ | ✔ | — |
| **`IN10`**| `PC0` | ✔ | ✔ | ✔ |
| **`IN11`**| `PC1` | ✔ | ✔ | ✔ |
| **`IN12`**| `PC2` | ✔ | ✔ | ✔ |
| **`IN13`**| `PC3` | ✔ | ✔ | ✔ |
| **`IN14`**| `PC4` | ✔ | ✔ | — |
| **`IN15`**| `PC5` | ✔ | ✔ | — |

---

## 5. API Reference

### `adc::AdcHandler<Instance>`
```cpp
namespace adc
{
    template<Instance I>
    class AdcHandler
    {
    public:
        explicit AdcHandler(Resolution res = Resolution::Bits12, SamplingTime smp = SamplingTime::Cycles84);
        explicit AdcHandler(const AdcConfig<I>& config);

        // Channel Enable
        bool enableChannel(Channel ch, SamplingTime smp = SamplingTime::Cycles84) noexcept;
        bool enableChannel(gpio::Pin pin, SamplingTime smp = SamplingTime::Cycles84) noexcept;

        // Single Channel Reading
        std::uint16_t readRaw(Channel ch) noexcept;
        std::uint16_t readRaw(gpio::Pin pin) noexcept;
        float readVoltage(Channel ch, float vref = 3.3f) noexcept;
        float readVoltage(gpio::Pin pin, float vref = 3.3f) noexcept;

        // Internal Sensors (ADC1 only)
        float readInternalTemperature(float vref = 3.3f) noexcept;
        float readVrefInternal(float vref = 3.3f) noexcept;
        float readVbat(float vref = 3.3f) noexcept;

        // Multi-Channel Sequence / Scan Mode
        void configureSequence(const Channel* channels, std::size_t count) noexcept;
        void readSequence(std::uint16_t* buffer, std::size_t count) noexcept;

        // Analog Watchdog
        void enableAnalogWatchdog(Channel ch, std::uint16_t lowThreshold, std::uint16_t highThreshold, stm32::Callback callback = nullptr) noexcept;
        void disableAnalogWatchdog() noexcept;

        // Callbacks & Control
        void attachConversionCompleteCallback(stm32::Callback cb) noexcept;
        void attachWatchdogCallback(stm32::Callback cb) noexcept;
        void enable() noexcept;
        void disable() noexcept;
    };

    using Adc1 = AdcHandler<Instance::adc1>;
    using Adc2 = AdcHandler<Instance::adc2>;
    using Adc3 = AdcHandler<Instance::adc3>;
}
```

---

## 6. Usage Examples

### Example 1: Reading Potentiometer Voltage on PA0
```cpp
#include "stm32/adc/adc.hpp"
#include "stm32/uart/uart.hpp"

using namespace adc;
using namespace gpio;
using namespace uart;

int main()
{
    Adc1 adc; // 12-bit ADC1 on APB2
    adc.enableChannel(PA0); // Automatically sets PA0 to Analog Mode

    while(true)
    {
        float volts = adc.readVoltage(PA0, 3.3f);
        // volts is between 0.0f and 3.3f
    }
}
```

### Example 2: Internal Die Temperature Reading
```cpp
#include "stm32/adc/adc.hpp"

using namespace adc;

int main()
{
    Adc1 adc;

    while(true)
    {
        float tempC = adc.readInternalTemperature(3.3f);
        // tempC is in degrees Celsius (e.g. 28.5 °C)
    }
}
```

