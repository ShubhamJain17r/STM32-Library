# ADC (Analog-to-Digital Converter) Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The STM32F446RE features **3 independent 12-bit successive-approximation ADCs** (`ADC1`, `ADC2`, `ADC3`) on APB2:
* **Resolutions**: 12-bit (0..4095), 10-bit (0..1023), 8-bit (0..255), 6-bit (0..63).
* **Channel Multiplexing**:
  * 16 external channels (`IN0` through `IN15` on GPIOA, GPIOB, GPIOC).
  * 3 internal channels on `ADC1`:
    * Temperature Sensor ($T_{\text{SENSE}}$ on `Channel::Temperature`).
    * Internal Reference Voltage ($V_{\text{REFINT}} \approx 1.21\text{V}$ on `Channel::VrefInt`).
    * Battery Monitor ($V_{\text{BAT}} / 4$ on `Channel::Vbat`).
* **Conversion Modes**:
  * Single conversion, Continuous conversion, and Multi-channel Scan sequence.
* **Hardware Analog Watchdog (AWD)**:
  * Out-of-bounds voltage monitoring with high/low thresholds and interrupt callback.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 3 Instances** | ADC1, ADC2, ADC3 | Aliases `Adc1`, `Adc2`, `Adc3`, `Adc1Config`..`Adc3Config` | 🟢 Implemented |
| **12-bit / 10-bit / 8-bit / 6-bit** | Programmable `RES[1:0]` | Configurable via `Resolution` enum | 🟢 Implemented |
| **Automatic GPIO Analog Routing**| `Mode::ANALOG`, `Pull::NONE` | Auto-configured upon `enableChannel()` | 🟢 Implemented |
| **Single Channel Reading** | Blocking Polling | `readRaw()` and floating-point `readVoltage()` | 🟢 Implemented |
| **Internal Temperature Sensor** | $T_{\text{SENSE}}$ on ADC1 | `readInternalTemperature()` (in $^\circ\text{C}$) | 🟢 Implemented |
| **Internal $V_{\text{REFINT}}$ & $V_{\text{BAT}}$** | Internal channels 17 & 18 | `readVrefInternal()` & `readVbat()` | 🟢 Implemented |
| **Multi-Channel Sequence / Scan** | `SQR1`..`SQR3` (up to 16 ranks) | `configureSequence()` & `readSequence()` | 🟢 Implemented |
| **Hardware Analog Watchdog** | `HTR` / `LTR` thresholds | `enableAnalogWatchdog()` with callback | 🟢 Implemented |
| **Centralized Interrupt Dispatch**| Shared `ADC_IRQn` | `AdcEvent::handleInterrupt()` | 🟢 Implemented |
| **DMA Multi-Channel Circular** | DMA2 Streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Configuration & Defaults Breakdown

* **`AdcConfig<Instance>` Defaults**:
  * `resolution`: `Resolution::Bits12`
  * `defaultSamplingTime`: `SamplingTime::Cycles84`
  * `alignment`: `Alignment::Right`
  * `prescaler`: `Prescaler::Div4` (22.5 MHz at 90 MHz APB2)
  * `continuous`: `false`
  * `scanMode`: `false`

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/adc/adc.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace adc;
using namespace gpio;

// 1. Minimal Initialization (12-bit on PA0)
Adc1 adc;
adc.enableChannel(PA0); // Automatically sets PA0 to Analog Mode

uint16_t raw = adc.readRaw(PA0);          // 0..4095
float volts  = adc.readVoltage(PA0, 3.3f); // 0.0V..3.3V

// 2. Internal Sensors
float tempC = adc.readInternalTemperature(3.3f); // Die temp in °C
float vref  = adc.readVrefInternal(3.3f);        // ~1.21 V

// 3. Multi-Channel Scan Sequence
Channel seq[3] = {Channel::IN0, Channel::IN1, Channel::IN2};
adc.configureSequence(seq, 3);
uint16_t readings[3];
adc.readSequence(readings, 3);

// 4. Analog Watchdog
adc.enableAnalogWatchdog(Channel::IN0, 1000, 3000, [](){
    // Voltage left 1000..3000 count range!
});
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Temperature Sensor Warmup**: Internal temperature reading requires a minimum sampling time of $10\mu\text{s}$. The library automatically uses `SamplingTime::Cycles480` and applies the warmup delay.
* **Pin Conflicts**: If a pin is enabled for ADC, never configure digital pull-up/down resistors on it as this will distort analog measurements.

---

## 6. Example Catalogue & Links

1. [📖 `01_single_channel_analog_read.cpp`](examples/01_single_channel_analog_read.cpp): 12-bit voltage conversion on PA0 with UART telemetry.
2. [📖 `02_internal_temperature_and_vref.cpp`](examples/02_internal_temperature_and_vref.cpp): Reading MCU die temperature and internal $V_{\text{REFINT}}$ bandgap reference.
3. [📖 `03_multi_channel_scan_sequence.cpp`](examples/03_multi_channel_scan_sequence.cpp): Multi-channel sequential scanning on PA0, PA1, and PA2.
4. [📖 `04_analog_watchdog_interrupt.cpp`](examples/04_analog_watchdog_interrupt.cpp): Out-of-bounds voltage monitoring using the hardware Analog Watchdog.

