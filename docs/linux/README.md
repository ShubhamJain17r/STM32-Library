# Linux Portability, Build, Flash & Debugging Guide

This guide details the complete toolchain setup, build configuration, on-target flashing, and hardware debugging workflow for the **STM32F446RE Bare-Metal C++17 Library** on Linux (Ubuntu, Debian, Fedora, Arch).

---

## 1. Prerequisites & System Packages

### Ubuntu / Debian
```bash
sudo apt update
sudo apt install -y \
    gcc-arm-none-eabi \
    gdb-multiarch \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    openocd \
    stlink-tools \
    cmake \
    make \
    ninja-build \
    picocom
```

### Fedora
```bash
sudo dnf install -y \
    arm-none-eabi-gcc-cs-c++ \
    arm-none-eabi-newlib \
    gdb \
    openocd \
    stlink \
    cmake \
    make
```

### USB Permissions (udev Rules for ST-LINK / Nucleo)
Ensure non-root users can communicate with the onboard ST-LINK/V2-1 debugger on the Nucleo-F446RE:
```bash
sudo cp /usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d/ 2>/dev/null || true
# Or add ST-LINK rules:
sudo tee /etc/udev/rules.d/49-stlinkv2-1.rules << 'EOF'
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE:="0666"
EOF
sudo udevadm control --reload-rules
sudo udevadm trigger
```

---

## 2. Repository Layout & Translation Units on Linux

The repository contains:
* **`Inc/stm32/`**: C++17 Peripheral Headers and **Translation Units** (`.cpp` files).
* **`Src/`**: Application entry point (`main.cpp`), Newlib syscall stubs (`syscalls.c`), and heap allocator (`sysmem.c`).
* **`Startup/`**: Cortex-M4 vector table and assembly reset handler (`startup_stm32f446retx.s`).
* **`Drivers/CMSIS/`**: Standard ARM Cortex-M4 Core and STMicroelectronics device headers (`stm32f446xx.h`, `core_cm4.h`, etc.) and system clock initialization (`system_stm32f4xx.c`).
* **`stm32_sources.mk`**: Exported GNU Makefile include fragment defining HAL source paths, includes, and linker script for downstreams.

> [!NOTE]
> **Tier 1 Bare-Metal Silicon HAL Engine Architecture**:
> In a multi-tier robotics software ecosystem, `STM32-Library` serves as Tier 1. It encapsulates all bare-metal silicon drivers and exports them cleanly as a modern CMake static library target (`stm32_hal` / `stm32::hal`) and a GNU Make fragment (`stm32_sources.mk`), eliminating the need for downstream consumers to re-enumerate the 20+ peripheral translation units.

---

## 3. Building with CMake (Recommended)

The primary build system uses modern CMake with a dedicated ARM toolchain file (`cmake/arm-none-eabi-gcc.cmake`).

### Configuration
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### Compilation
```bash
cmake --build build -j$(nproc)
```

The build produces the following target artifacts inside `build/`:
* `libstm32_hal.a`: Static HAL engine library archive exporting all silicon peripheral drivers.
* `stm32_library.elf`: Standalone executable and linkable format binary with full symbols for GDB.
* `stm32_library.bin`: Raw flash binary for direct programmer flashing.
* `stm32_library.hex`: Intel HEX format binary.
* `stm32_library.map`: Linker symbol map and memory distribution report.

---

## 4. Building with GNU Make

A standalone `Makefile` is also provided for direct command-line builds:

```bash
# Build ELF, BIN, and HEX
make -j$(nproc)

# Clean build artifacts
make clean
```

If your toolchain is in a non-standard path, pass `ARM_TOOLCHAIN_DIR`:
```bash
make ARM_TOOLCHAIN_DIR=/opt/arm-gnu-toolchain/bin
```

---

## 5. Downstream Integration (Tier 1 Architecture)

### A. Consuming via Modern CMake (`stm32::hal`)
In downstream components (Tier 2, such as sensor/actuator drivers) or product repositories (Tier 4):
```cmake
add_subdirectory(path/to/STM32-Library)

# Downstream target automatically inherits includes, defines, standards, and flags:
target_link_libraries(my_robot_app PRIVATE stm32::hal)
```

Target properties automatically propagated:
* **Include Directories**: `${STM32_LIB_DIR}/Inc`, CMSIS Core, CMSIS STM32F4xx headers (`PUBLIC`).
* **Compiler Definitions**: `STM32F446xx` (`PUBLIC`).
* **Target Architecture**: `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard` (`PUBLIC`).
* **Standards**: `cxx_std_17`, `c_std_11` (`PUBLIC`).

### B. Consuming via GNU Make Fragment (`stm32_sources.mk`)
Downstream Makefiles can simply include the source fragment without duplicating source lists:
```makefile
LIB_DIR = path/to/STM32-Library
include $(LIB_DIR)/stm32_sources.mk

# Automatically provides:
#   $(STM32_LIB_SOURCES)      - All 22 HAL C, C++, and ASM source files
#   $(STM32_LIB_INCLUDES)     - Compiler include flags (-I...)
#   $(STM32_LINKER_SCRIPT)    - STM32F446RETX_FLASH.ld path
#   $(STM32_MCU_FLAGS)        - Target architecture flags (-mcpu=cortex-m4 ...)
#   $(STM32_DEFS)             - Target hardware defines (-DSTM32F446xx)
```

---

## 6. Flashing to STM32 Nucleo-F446RE

Connect the Nucleo-64 board to your Linux host via Mini-USB (CN1).

### Method A: Via OpenOCD (Built-in CMake/Make Target)
```bash
# Using CMake
cmake --build build --target flash

# Using Make
make flash

# Manual OpenOCD command
openocd -f board/st_nucleo_f4.cfg -c "program build/stm32_library.elf verify reset exit"
```

### Method B: Via `st-flash` (ST-Link Tools)
```bash
# Using CMake
cmake --build build --target flash-stlink

# Using Make
make flash-stlink

# Manual st-flash command
st-flash write build/stm32_library.bin 0x08000000
```

### Method C: USB Mass Storage Drag-and-Drop
When plugged in, the Nucleo board presents a USB Mass Storage drive labeled `NODE_F446RE`:
```bash
cp build/stm32_library.bin /media/$USER/NODE_F446RE/
sync
```
The ST-LINK LED will flash red/green during programming and turn solid green upon completion.

---

## 7. On-Target Hardware Debugging with GDB

### Terminal 1: Launch OpenOCD GDB Server
```bash
openocd -f board/st_nucleo_f4.cfg
```
OpenOCD will listen on localhost port `3333` for GDB connections.

### Terminal 2: Connect GDB
```bash
gdb-multiarch build/stm32_library.elf \
    -ex "target extended-remote localhost:3333" \
    -ex "monitor reset halt" \
    -ex "load" \
    -ex "break main" \
    -ex "continue"
```

### Essential GDB Commands
* `info registers`: View Cortex-M4 CPU core registers (`r0`–`r12`, `sp`, `lr`, `pc`, `xpsr`).
* `x/16wx 0x40020000`: Inspect GPIOA peripheral registers directly in memory.
* `p/x *RCC`: Inspect entire RCC hardware register structure.
* `bt`: Backtrace execution stack across nested interrupts.

---

## 8. Serial Telemetry & Console

The default `main.cpp` communicates over **USART2** (PA2 TX, PA3 RX) at **115200 baud, 8N1**, which is wired directly to the ST-LINK Virtual COM Port (`/dev/ttyACM0`):

```bash
picocom -b 115200 /dev/ttyACM0
# Or using tio:
tio /dev/ttyACM0 -b 115200
```

To exit `picocom`, press `Ctrl-A` followed by `Ctrl-X`.
