# SPI (Serial Peripheral Interface) Subsystem Documentation

## 1. Peripheral Overview (STM32F446RE)

The STM32F446RE contains **4 hardware Serial Peripheral Interface (SPI)** modules (`SPI1`, `SPI2`, `SPI3`, `SPI4`):
* **Bus Architecture**:
  * `SPI1` and `SPI4` operate on **APB2** (up to 90 MHz bus clock, maximum SPI clock of **45 MHz** with `/2` prescaler).
  * `SPI2` and `SPI3` operate on **APB1** (up to 45 MHz bus clock, maximum SPI clock of **22.5 MHz** with `/2` prescaler).
* **Hardware Capabilities**:
  * Master and Slave modes.
  * 4 standard SPI Clock Modes (Mode 0, Mode 1, Mode 2, Mode 3) controlling Clock Polarity (`CPOL`) and Clock Phase (`CPHA`).
  * 8-bit and 16-bit data frame formats (`DFF`).
  * MSB-first or LSB-first bit serialization (`LSBFIRST`).
  * Full-Duplex (4-wire/3-wire), Simplex TX-only, Simplex RX-only, and Half-Duplex (1-wire Bidirectional).
  * Hardware and Software Slave Select (NSS) management.
  * Hardware CRC calculation and polynomial error checking.

---

## 2. Feature Matrix: Implemented vs STM32F446 Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 4 Instances** | SPI1, SPI2, SPI3, SPI4 | Aliases `Spi1`, `Spi2`, `Spi3`, `Spi4` | 🟢 Implemented |
| **Dynamic Baud Rate Auto-Calculation**| 8 Prescaler dividers (/2 to /256) | Auto-calculated from dynamic `rcc::frequency(bus)` | 🟢 Implemented |
| **Automatic GPIO AF Routing** | Multiplexed AF5 / AF6 pins | `initPins()` auto-configures SCK, MISO, MOSI, NSS | 🟢 Implemented |
| **Full-Duplex Polling Transfers** | Byte / Halfword / Buffer | `transfer(byte)`, `transfer16()`, `transfer(tx, rx, len)` | 🟢 Implemented |
| **Transmit-Only & Receive-Only**| `transmit()`, `receive()` | High-speed blocking transfer loops with busy guard | 🟢 Implemented |
| **All 4 SPI Modes (0, 1, 2, 3)** | CPOL 0/1, CPHA 0/1 | `SpiMode::Mode0` through `Mode3` | 🟢 Implemented |
| **8-Bit & 16-Bit Data Frame** | `CR1_DFF` bit | `DataSize::Bits8` and `Bits16` | 🟢 Implemented |
| **Bit Order (MSB / LSB First)**| `CR1_LSBFIRST` bit | `BitOrder::MsbFirst` and `LsbFirst` | 🟢 Implemented |
| **Software & Hardware NSS** | `SSM`, `SSI`, `SSOE` bits | `SlaveSelect::Software`, `HardwareInput`, `HardwareOutput` | 🟢 Implemented |
| **RAII Chip Select Helper** | Scoped CS line assertion | `spi::ChipSelectGuard(csPin)` | 🟢 Implemented |
| **Asynchronous Non-Blocking Transfer**| TXE / RXNE Interrupts | `transferAsync(tx, rx, len)` + completion callback | 🟢 Implemented |
| **Hardware CRC Generation** | CRC polynomial register | Planned (Phase 14) | ⚪ Future Scope |
| **DMA Direct Memory Access** | DMA1 / DMA2 streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Architecture & Directory Layout

```
Inc/stm32/spi/
├── core/
│   ├── spi_types.hpp          <-- Enums: Instance, Mode, SpiMode, Prescaler, BitOrder, DataSize, Direction, SlaveSelect
│   ├── spi_traits.hpp         <-- Traits<Instance>: peripheral pointer, APB bus, IRQ number, default pins
│   ├── spi_af_traits.hpp      <-- Pin capability matrix & alternate function numbers (AF5/AF6)
│   ├── spi_config.hpp         <-- SpiConfig<Instance> struct with flexible constructors
│   └── spi_helper.hpp         <-- Zero-overhead inline register manipulation functions
├── handler/
│   └── spi_handler.hpp        <-- SpiHandler<Instance> (Master/Slave driver, Polling, Async, ChipSelectGuard)
├── interrupt/
│   ├── spi_interrupt.hpp      <-- Event callback dispatch table
│   ├── spi_interrupt.cpp      <-- Event dispatcher implementation
│   └── spi_irq.cpp            <-- ISR vector handlers (SPI1_IRQHandler..SPI4_IRQHandler)
└── spi.hpp                    <-- Master umbrella header
```

---

## 4. Hardware Pin Mappings (STM32F446RE)

| Peripheral | Bus | Signal | Available Pins | Alternate Function |
| :--- | :--- | :--- | :--- | :--- |
| **`SPI1`** | **APB2** | **SCK** | `PA5`, `PB3` | **AF5** |
| | | **MISO** | `PA6`, `PB4` | **AF5** |
| | | **MOSI** | `PA7`, `PB5` | **AF5** |
| | | **NSS** | `PA4`, `PA15` | **AF5** |
| **`SPI2`** | **APB1** | **SCK** | `PB10`, `PB13`, `PC7`, `PD3` | **AF5** |
| | | **MISO** | `PB14`, `PC2` | **AF5** |
| | | **MOSI** | `PB15`, `PC1`, `PC3` | **AF5** |
| | | **NSS** | `PB9`, `PB12`, `PD0` | **AF5** |
| **`SPI3`** | **APB1** | **SCK** | `PB3`, `PC10` | **AF6** |
| | | **MISO** | `PB4`, `PC11` | **AF6** |
| | | **MOSI** | `PB5`, `PC12` | **AF6** |
| | | **NSS** | `PA4`, `PA15` | **AF6** |
| **`SPI4`** | **APB2** | **SCK** | `PE2`, `PE12` | **AF5** |
| | | **MISO** | `PE5`, `PE13` | **AF5** |
| | | **MOSI** | `PE6`, `PE14` | **AF5** |
| | | **NSS** | `PE4`, `PE11` | **AF5** |

---

## 5. API Reference

### `spi::SpiHandler<Instance>`
```cpp
namespace spi
{
    template<Instance I>
    class SpiHandler
    {
    public:
        explicit SpiHandler(std::uint32_t baudRateHz = 1'000'000, SpiMode mode = SpiMode::Mode0);
        explicit SpiHandler(gpio::Pin sck, gpio::Pin miso, gpio::Pin mosi, std::uint32_t baudRateHz = 1'000'000);
        explicit SpiHandler(const SpiConfig<I>& config);

        // Synchronous / Polling Transfers
        std::uint8_t  transfer(std::uint8_t data) noexcept;
        std::uint16_t transfer16(std::uint16_t data) noexcept;

        void transmit(const std::uint8_t* data, std::size_t length) noexcept;
        void transmit16(const std::uint16_t* data, std::size_t length) noexcept;

        void receive(std::uint8_t* data, std::size_t length, std::uint8_t dummy = 0xFF) noexcept;
        void receive16(std::uint16_t* data, std::size_t length, std::uint16_t dummy = 0xFFFF) noexcept;

        void transfer(const std::uint8_t* txBuf, std::uint8_t* rxBuf, std::size_t length) noexcept;
        void transfer16(const std::uint16_t* txBuf, std::uint16_t* rxBuf, std::size_t length) noexcept;

        // Asynchronous / Non-blocking Transfers
        bool transferAsync(const std::uint8_t* txBuf, std::uint8_t* rxBuf, std::size_t length) noexcept;
        bool isAsyncBusy() const noexcept;
        void attachTransferCompleteCallback(stm32::Callback cb) noexcept;

        void enable() noexcept;
        void disable() noexcept;
        bool isBusy() const noexcept;

        void setBaudRate(std::uint32_t baudRateHz) noexcept;
        void setPrescaler(Prescaler psc) noexcept;
        void setSpiMode(SpiMode mode) noexcept;
        void setDataSize(DataSize size) noexcept;
        void setBitOrder(BitOrder order) noexcept;
    };

    using Spi1 = SpiHandler<Instance::spi1>;
    using Spi2 = SpiHandler<Instance::spi2>;
    using Spi3 = SpiHandler<Instance::spi3>;
    using Spi4 = SpiHandler<Instance::spi4>;
}
```

---

## 6. Usage Examples

### Example 1: Full-Duplex SPI1 Master (10 MHz, Mode 0)
```cpp
#include "stm32/spi/spi.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace spi;
using namespace gpio;

int main()
{
    // Initialize SPI1 at 10 MHz in Mode 0 (SCK=PA5, MISO=PA6, MOSI=PA7)
    Spi1 spi(10'000'000, SpiMode::Mode0);

    // Send single byte and read response simultaneously
    std::uint8_t rx = spi.transfer(0xAA);

    // Full-duplex buffer exchange
    std::uint8_t txBuf[4] = {0x01, 0x02, 0x03, 0x04};
    std::uint8_t rxBuf[4];
    spi.transfer(txBuf, rxBuf, 4);

    while(true) {}
}
```

### Example 2: External Sensor / Flash Read with RAII `ChipSelectGuard`
```cpp
#include "stm32/spi/spi.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace spi;
using namespace gpio;

int main()
{
    Spi1 spi(5'000'000);
    DigitalOutput cs(PA4, DigitalOutputConfig{.initialState = PinState::HIGH});

    std::uint8_t chipId = 0;

    {
        // Assert CS (LOW) for the duration of this scope block
        ChipSelectGuard guard(cs);

        spi.transfer(0x9F); // Read JEDEC ID Command
        chipId = spi.transfer(0xFF); // Read response byte
    } // CS automatically deasserted (HIGH) here

    while(true) {}
}
```

