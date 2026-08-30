# I2C (Inter-Integrated Circuit) Subsystem

## 1. Overview & Hardware Mapping (STM32F446RE)

The STM32F446RE contains **3 hardware I2C peripherals** on APB1 (up to 45 MHz bus clock):
* **`I2C1`**: Default SCL on `PB8`, SDA on `PB9` (Alternate `PB6`/`PB7`).
* **`I2C2`**: Default SCL on `PB10`, SDA on `PB11` (Alternate `PC12`).
* **`I2C3`**: Default SCL on `PA8`, SDA on `PB4` (Alternate `PC9`).
* **Speed Profiles**:
  * **Standard Mode (Sm)**: Up to **100 kHz**.
  * **Fast Mode (Fm)**: Up to **400 kHz** (Duty Cycle 2:1 or 16:9).
* **Hardware Robustness**:
  * Automatic Open-Drain Alternate Function (`AF4`) pin routing with optional internal pull-ups.
  * Monotonic non-blocking timeout guarding on every hardware loop.
  * Dedicated bus recovery routine to unstick slaves holding SDA low.

---

## 2. Feature Matrix: Implemented vs Hardware

| Feature | STM32F446 Hardware | Library Implementation | Status |
| :--- | :--- | :--- | :--- |
| **All 3 Instances** | I2C1, I2C2, I2C3 | Aliases `I2c1`..`I2c3`, `I2c1Config`..`I2c3Config` | 🟢 Implemented |
| **Standard Mode (100 kHz)**| Sm Timing | Auto-calculated `CCR` & `TRISE` from `rcc::getPCLK1()` | 🟢 Implemented |
| **Fast Mode (400 kHz)** | Fm Timing (Duty 2:1 & 16:9) | Fully configurable via `DutyCycle` enum | 🟢 Implemented |
| **Automatic GPIO AF Routing** | Alternate Function AF4 | Auto-configures Open-Drain, High-Speed, Pull-Ups | 🟢 Implemented |
| **Non-Locking Timeout Protection** | User polling loops | Every loop guarded with monotonic `systick::Timeout` | 🟢 Implemented |
| **ST Errata-Compliant Receiver**| 1-Byte, 2-Byte, N-Byte | Precise `POS`, `ACK`, `ADDR`, `STOP` sequencing | 🟢 Implemented |
| **Bus Recovery Helper** | SCL Clock Pulsing | `recoverBus()` (9 clock pulses to free stuck SDA) | 🟢 Implemented |
| **I2C Bus Scanner** | Address probing 0x08..0x77 | `scanBus()` & `isDeviceReady()` | 🟢 Implemented |
| **8-Bit & 16-Bit Register Helpers**| Sub-addressing | `readRegister()`, `writeRegister()`, 16-bit overloads | 🟢 Implemented |
| **DMA Direct Memory Access**| DMA1 Streams | Planned (Phase 11) | ⚪ Future Scope |

---

## 3. Configuration & Defaults Breakdown

* **`I2cConfig<Instance>` Defaults**:
  * `speedHz`: `100'000` (100 kHz Standard Mode)
  * `dutyCycle`: `DutyCycle::Duty2_1`
  * `enableInternalPullup`: `true`
  * `ownAddress`: `0x00`
  * `scl` & `sda`: Peripheral defaults (`PB8`/`PB9` for `I2C1`).

---

## 4. Complete API Reference & Syntax

```cpp
#include "stm32/i2c/i2c.hpp"

using namespace i2c;

// 1. Default Minimal Initialization
I2c1 i2c(100'000); // 100 kHz Standard Mode on PB8/PB9

// 2. Bus Scanner
std::uint8_t devices[16];
std::size_t count = i2c.scanBus(devices, 16);

// 3. Register Read & Write
std::uint8_t chipId = 0;
i2c.readRegister(0x68, 0x75, chipId); // MPU6050 WHO_AM_I
i2c.writeRegister(0x68, 0x6B, 0x00);  // Wake up MPU6050

// 4. Raw Buffer Read & Write
std::uint8_t tx[2] = {0x01, 0x02};
std::uint8_t rx[4];
i2c.write(0x50, tx, 2);
i2c.read(0x50, rx, 4);

// 5. Bus Recovery
if(!i2c.isDeviceReady(0x68)) {
    i2c.recoverBus();
}
```

---

## 5. Common Use Cases, Best Practices & Pitfalls

* **External vs Internal Pull-Ups**: While the library can enable internal pull-up resistors, standard $4.7\text{ k}\Omega$ external pull-up resistors are strongly recommended for Fast Mode 400 kHz buses.
* **Repeated START in Register Read**: `readRegister()` automatically transmits the sub-register address with a repeated START (leaving the bus owned), ensuring thread-safe reads on multi-master buses.

---

## 6. Example Catalogue & Links

1. [📖 `01_active_bus_scanner.cpp`](examples/01_active_bus_scanner.cpp): Active 7-bit bus scanner (`0x08`–`0x77`) with UART reporting.
2. [📖 `02_sensor_register_read_write.cpp`](examples/02_sensor_register_read_write.cpp): Register write/read operations with an MPU6050 sensor.
3. [📖 `03_custom_pins_fast_mode.cpp`](examples/03_custom_pins_fast_mode.cpp): 400 kHz Fast Mode on alternate pins PB6 (SCL) and PB7 (SDA).
4. [📖 `04_bus_recovery_stuck_slave.cpp`](examples/04_bus_recovery_stuck_slave.cpp): Unsticking a bus where SDA was held low by an interrupted transaction.

