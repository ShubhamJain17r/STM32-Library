# SPI (Serial Peripheral Interface) Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The STM32F446RE contains **4 hardware SPI controllers**:
* **APB2 High-Speed SPI (up to 45 MHz SCK)**: `SPI1`, `SPI4`.
* **APB1 Standard SPI (up to 22.5 MHz SCK)**: `SPI2`, `SPI3`.
* **Data Frames & Modes**:
  * 8-bit or 16-bit frame formats (`DFF`).
  * SPI Modes 0, 1, 2, 3 (CPOL / CPHA combinations).
  * Full-duplex master/slave, simplex receive-only, and half-duplex bidirectional.
  * Hardware CRC calculation and software/hardware slave select management.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 4 Instances** | SPI1 through SPI4 | Aliases `Spi1`..`Spi4`, `Spi1Config`..`Spi4Config` | 🟢 Implemented |
| **Auto-Routing GPIO** | AF5 / AF6 Alternate Function | Auto-configures SCK, MISO, MOSI pins | 🟢 Implemented |
| **Dynamic Prescaler Calculation**| Target Hz to Power-of-2 /2../256 | `calculatePrescaler()` based on live APB bus clock | 🟢 Implemented |
| **8-bit & 16-bit Data Formats** | `DFF` bit in CR1 | Configurable via `DataFrameFormat` enum | 🟢 Implemented |
| **RAII Chip Select Guard** | Active-low / active-high CS | `spi::ChipSelectGuard` helper | 🟢 Implemented |
| **Asynchronous Transfer** | RXNE / TXE interrupts | `transferAsync()` with completion callback | 🟢 Implemented |
| **DMA Circular Mode** | DMA1/DMA2 Streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Configuration & Defaults Breakdown

* **`SpiConfig<Instance>` Defaults**:
  * `baudRateHz`: `1'000'000` (1 MHz)
  * `spiMode`: `SpiMode::Mode0` (CPOL=0, CPHA=0)
  * `dataFrame`: `DataFrameFormat::Bits8`
  * `bitOrder`: `BitOrder::MsbFirst`
  * `slaveSelect`: `SlaveSelect::Software`

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/spi/spi.hpp"

using namespace spi;
using namespace gpio;

// 1. Default Minimal Initialization (10 MHz Mode 0 on PA5/PA6/PA7)
Spi1 spi(10'000'000, SpiMode::Mode0);

// Single byte full-duplex transfer
std::uint8_t rx = spi.transfer(0x55);

// Buffer transfer
std::uint8_t tx[4] = {0x01, 0x02, 0x03, 0x04};
std::uint8_t rxBuf[4];
spi.transfer(tx, rxBuf, 4);

// RAII Chip Select Guard
DigitalOutput cs(PB6);
{
    ChipSelectGuard guard(cs); // Pulls CS low
    spi.transfer(tx, rxBuf, 4);
} // CS automatically pulled high upon scope exit
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **Dynamic Prescalers**: SPI hardware only supports power-of-two prescalers ($/2, /4, /8, /16, /32, /64, /128, /256$). The library always chooses the closest prescaler that does not exceed your requested `baudRateHz`.
* **Async Transfer Lifetime**: When calling `transferAsync(tx, rx, len)`, ensure the `tx` and `rx` buffers remain in valid memory until the completion callback fires.

---

## 6. Example Catalogue & Links

1. [📖 `01_blocking_loopback_defaults.cpp`](examples/01_blocking_loopback_defaults.cpp): Full-duplex 5 MHz SPI1 loopback exchange.
2. [📖 `02_custom_pins_and_prescaler.cpp`](examples/02_custom_pins_and_prescaler.cpp): Custom alternate pinout on PB3/PB4/PB5 with custom baud rate.
3. [📖 `03_sensor_read_chip_select_guard.cpp`](examples/03_sensor_read_chip_select_guard.cpp): Reading external SPI flash/sensor with RAII `ChipSelectGuard`.
4. [📖 `04_async_interrupt_transfer.cpp`](examples/04_async_interrupt_transfer.cpp): Non-blocking asynchronous SPI transfer with completion callback.

