# Role & Task
You are an expert embedded systems engineer and senior bare-metal C++ firmware architect specializing in ARM Cortex-M microcontrollers and STMicroelectronics silicon.

You are performing an in-depth technical audit of an open-source, bare-metal C++17 Hardware Abstraction Layer (HAL) and Peripheral Driver Library written for the STMicroelectronics STM32F446RET6 microcontroller (ARM Cortex-M4 with single-precision hardware FPU, 512 KB Flash, 128 KB SRAM, LQFP64 package) on the NUCLEO-F446RE (MB1136) development board.

---

## 1. Primary Technical Standards & References
All evaluation and auditing must be cross-referenced against the official silicon documentation:
1. Datasheet: STMicroelectronics DS10693 Rev 11 (STM32F446xC/E)
2. Reference Manual: STMicroelectronics RM0390 Rev 9 (STM32F446xx advanced Arm-based 32-bit MCUs)
3. Board User Manual: STMicroelectronics UM1724 Rev 17 (STM32 Nucleo-64 boards MB1136)
4. ARM Architecture: ARMv7-M Architecture Reference Manual (ARM DDI 0403E.e)

---

## 2. Core Architectural Philosophy & Hard Constraints
1. Zero Runtime Overhead & Compile-Time Safety:
   - Heavy use of C++17 constexpr, template metaprogramming, and traits classes to resolve peripheral addresses, alternate functions, bus prescalers, and pin configurations at compile time.
   - Elimination of dynamic memory allocation (malloc, free, new, delete are forbidden; heap size is 0 bytes).
   - Zero C++ Run-Time Type Information (-fno-rtti).
   - Zero C++ Exceptions (-fno-exceptions, -fno-use-cxa-atexit).
   - Zero standard library overhead: std::function is strictly prohibited in ISR contexts; deterministic function pointer aliases (using Callback = void(*)() noexcept;) or static tables are used instead.
2. Deterministic ISR Dispatching:
   - Interrupt service routines route through hardware vector tables in assembly, then dispatch to static type-safe event tables without heap allocation or lambda captures.
   - Ring buffers utilize power-of-two / statically-sized storage with volatile head, tail, and count indices for lockless single-producer single-consumer concurrency.
3. Strict Scope:
   - Phases 0 through 10 are fully implemented.
   - Phase 11 (DMA) is intentionally unstarted and out-of-scope.

---

## 3. Repository & Directory Structure
The repository is engineered for cross-platform portability on Linux and Windows:

```
├── CMakeLists.txt                         # Root CMake build configuration
├── Makefile                               # Standalone GNU Makefile with flash/debug targets
├── cmake/
│   └── arm-none-eabi-gcc.cmake            # Cross-compilation toolchain file
├── STM32F446RETX_FLASH.ld                 # GNU Linker script (Flash target)
├── STM32F446RETX_RAM.ld                   # GNU Linker script (SRAM target)
├── Drivers/CMSIS/                         # Official CMSIS 5 Core & STM32F4 Device headers
│   ├── Device/ST/STM32F4xx/Include/       # stm32f446xx.h, system_stm32f4xx.h
│   └── Include/                           # core_cm4.h, cmsis_gcc.h, etc.
├── Startup/
│   └── startup_stm32f446retx.s            # Vector table, Reset_Handler, FPU init, weak IRQs
├── Src/
│   ├── main.cpp                           # Integrated hardware demonstration application
│   ├── syscalls.c                         # Minimal newlib POSIX stubs
│   └── sysmem.c                           # _sbrk memory boundary stub
├── Inc/stm32/                             # Core hardware abstraction headers & TUs
│   ├── common/
│   │   ├── registers/registers.hpp        # Atomic volatile bitwise register primitives
│   │   ├── buffer/buffer.hpp              # Static RingBuffer<T, N> circular FIFO
│   │   ├── callback/callback.hpp          # Callback type aliases
│   │   ├── rcc/                           # Dynamic clock decoding & peripheral clock gating
│   │   │   ├── rcc.hpp                    # Bus enums, PLL decoder, getSYSCLK/getTimerClock
│   │   │   └── rcc_enable.hpp             # Clock enable overloads with __DSB() bus barriers
│   │   └── systick/                       # SysTick monotonic timer, delayMs, delayUs, Timeout
│   ├── gpio/                              # Type-safe Pin value types, Modes, Speeds, AF matrix
│   │   ├── core/                          # Pin, PinMap (PA0..PH1), gpio_helper
│   │   ├── af/                            # AF traits matrix and pin routing helpers
│   │   ├── digital_output/                # DigitalOutput (high, low, toggle, write)
│   │   ├── digital_input/                 # DigitalInput (read, isHigh, isLow, pull-up/down)
│   │   └── interrupt_input/               # InterruptInput (EXTI-integrated button/line)
│   ├── exti/                              # External Interrupt Manager (lines 0-15, SWIER, IMR)
│   ├── timer/                             # General Purpose & Advanced Timers (TIM1-TIM14)
│   │   ├── timebase/                      # TimerHandler (periodic timebases, tick counters)
│   │   ├── pwm/                           # PwmHandler (multi-channel hardware PWM, duty cycle)
│   │   ├── input_capture/                 # InputCaptureHandler (readFrequency, pulse width)
│   │   ├── one_pulse/                     # OnePulseHandler (deterministic hardware single pulse)
│   │   └── interrupt/                     # Timer IRQ dispatchers (update, CC1-CC4, break)
│   ├── uart/                              # USART1, USART2, USART3, UART4, UART5, USART6
│   │   ├── core/                          # Traits, Types (Parity, StopBits, Baud, OVER8/16)
│   │   ├── interrupt/                     # ISR dispatch (RXNE, TXE, IDLE)
│   │   └── uart.hpp                       # UartHandler (buffered asynchronous ring buffer)
│   ├── spi/                               # SPI1, SPI2, SPI3, SPI4
│   │   ├── core/                          # Traits, Clock polarity/phase, Prescaler decoding
│   │   └── spi.hpp                        # SpiHandler (blocking & non-blocking interrupt)
│   ├── i2c/                               # I2C1, I2C2, I2C3
│   │   ├── core/                          # Standard (100kHz) and Fast (400kHz) timing
│   │   └── i2c.hpp                        # I2cHandler (master read/write, stuck-bus recovery)
│   └── adc/                               # ADC1, ADC2, ADC3
│       ├── core/                          # 12-bit conversion, multi-channel scan, VREFINT, Temp
│       └── adc.hpp                        # AdcHandler (single read, sequence, analog watchdog)
└── docs/                                  # Architectural docs and 31 working standalone examples
```

Note on Translation Units: 16 C++ translation units reside under Inc/stm32/** (e.g. Inc/stm32/uart/interrupt/uart_interrupt.cpp, Inc/stm32/exti/exti_manager.cpp). The build systems (Makefile and CMakeLists.txt) explicitly compile these source units alongside Src/*.cpp.

---

## 4. Implemented Subsystems & Technical Details

### Phase 1: Common Hardware Primitives (Inc/stm32/common/)
- registers.hpp: Inline template wrappers over volatile memory locations: reg::read, reg::write, reg::setBits, reg::clearBits, reg::modifyBits, reg::isAnyBitSet, reg::isAllBitsSet.
- buffer.hpp: Zero-allocation buffer::RingBuffer<T, N> with power-of-two validation, volatile indices (head_, tail_, count_), and thread-safe / ISR-safe APIs.

### Phase 2: RCC Clock Tree Engine (Inc/stm32/common/rcc/)
- Dynamic runtime register decoder: Computes active SYSCLK, HCLK, PCLK1, and PCLK2 directly from RCC->CFGR (SWS bits) and RCC->PLLCFGR (PLLM, PLLN, PLLP, PLLR, PLLSRC) without hardcoded frequencies.
- Timer clock decoding: Correctly models APB prescaler multiplier (1x if APB prescaler = 1, else 2x).
- Bus synchronization: Gating functions in rcc_enable.hpp issue a Data Synchronization Barrier (__DSB()) after enabling AHBxENR / APBxENR bits to satisfy the 2-cycle bus latency requirement (RM0390 Section 6.2.14).

### Phase 3: GPIO Subsystem (Inc/stm32/gpio/)
- Strongly typed Pin struct wrapping port pointer and pin number with mask(), portIndex(), and index() methods.
- Full pin mapping PA0 through PH1.
- Clean separation into DigitalOutput, DigitalInput, and InterruptInput.
- Alternate Function (AF0 through AF15) compile-time traits table mapping timer, UART, SPI, and I2C channels to physical pins.

### Phase 4: SysTick Monotonic Timebase (Inc/stm32/common/systick/)
- 1 ms monotonic counter driven by Cortex-M4 Core SysTick timer.
- High-resolution micros() synthesizing current countdown value in SysTick->VAL with millisecond tick count.
- Non-blocking systick::Timeout class supporting elapsed, remaining, and restart/reset timing.

### Phase 5: EXTI Subsystem (Inc/stm32/exti/)
- ExtiManager: Static table routing lines 0 through 15 to application callbacks.
- Supports Rising, Falling, Both, and Software-triggered interrupts (EXTI->SWIER).
- Correct port-independent execution when an interrupt is triggered purely via software without a physical GPIO port binding.

### Phase 6: General Purpose & Advanced Timers (Inc/stm32/timer/)
- Universal abstraction over TIM1–TIM14 with distinction between Advanced (TIM1/TIM8 with MOE break), 32-bit (TIM2/TIM5), General Purpose 16-bit (TIM3/TIM4/TIM9-14), and Basic (TIM6/TIM7).
- PwmHandler: Frequency calculation, ARR/PSC generation, and multi-channel duty cycle control in percentage or permille.
- InputCaptureHandler: Channel capture values and automatic input frequency decoding (readFrequency()).
- OnePulseHandler: Hardware One-Pulse Mode (OPM) using PWM Mode 2 for deterministic microsecond-accurate single pulses without software jitter.

### Phase 7: UART / USART Subsystem (Inc/stm32/uart/)
- Full support for USART1, USART2, USART3, UART4, UART5, USART6 with NVIC dispatch.
- Baud rate generator with exact floating-point-free integer fixed-point arithmetic for both 16x oversampling (OVER8=0) and 8x oversampling (OVER8=1, where mantissa uses 3-bit shift and bit 3 is cleared per RM0390 Section 29.3.4).
- Asynchronous circular ring-buffered RX/TX with interrupt masking critical sections (__disable_irq() / __enable_irq()).
- Dynamic byte count query through available().

### Phase 8: SPI Subsystem (Inc/stm32/spi/)
- Full-duplex Master and Slave drivers for SPI1, SPI2, SPI3, and SPI4.
- Baud rate prescaler calculation from active APB bus frequency.
- Hardware Chip-Select (NSS) or Software GPIO slave management.
- Hardware pin constraint documented: Default SPI4 pins reside on Port E, which is physically omitted on the 64-pin LQFP64 STM32F446RE (UM1724 Table 19).

### Phase 9: I2C Subsystem (Inc/stm32/i2c/)
- Standard Mode (100 kHz) and Fast Mode (400 kHz) timing calculation.
- Master transmitter and receiver state machines with repeated-start and 7-bit addressing.
- Hardware Bus Recovery routine: Clears stuck slave devices by toggling SCL 9 times with STOP condition generation.

### Phase 10: ADC Subsystem (Inc/stm32/adc/)
- 12-bit successive approximation analog-to-digital converter abstraction (ADC1, ADC2, ADC3).
- Single channel, multi-channel sequential scanning, and continuous conversions.
- Dedicated drivers for internal Temperature Sensor (VSENSE) and Internal Reference Voltage (VREFINT = 1.21 V).
- Configurable Hardware Analog Watchdog (AWD) with high/low threshold interrupts.

---

## 5. Build, Memory, and Verification Metrics
- Compiler: arm-none-eabi-g++ (GCC 7.2.1 through GCC 14.x)
- Compilation Flags: -std=c++17 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Wall -Wextra -Wpedantic -ffunction-sections -fdata-sections -O2 -fno-exceptions -fno-rtti -fno-use-cxa-atexit
- Linker Flags: -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs
- Memory Footprint (Full application exercising SysTick, GPIO, EXTI, UART, Timer, SPI, I2C, ADC):
  - Flash: 11,436 bytes (2.2% of 512 KB)
  - SRAM: 3,208 bytes (2.5% of 128 KB)
  - Heap: 0 bytes (zero dynamic allocation)
- Test Coverage: All 31 documentation example files in docs/**/examples/*.cpp compile cleanly with 0 errors and 0 warnings.

---

## 6. Audit Instructions
Please conduct a thorough, rigorous technical audit of this architecture focusing on:
1. Hardware & Register-Level Correctness:
   - Verify that peripheral bit manipulation adheres strictly to ST RM0390 Rev 9.
   - Audit race conditions, volatile qualifiers, or missing barrier instructions (__DSB(), __ISB()) in ISR handlers.
   - Verify baud rate, clock prescaler, and timer frequency equations against silicon limits.
2. C++17 Embedded Best Practices & MISRA C++ Compliance:
   - Evaluate zero-cost abstraction compliance and ensure no hidden temporaries, vtables, or stack blowups occur.
   - Review type safety of enums (enum class), pointer casts, and constexpr evaluation.
3. Interrupt & Concurrency Safety:
   - Audit critical section protection (__disable_irq() / __enable_irq()) around shared ring buffers.
   - Check reentrancy and volatile semantics of ISR state variables.
4. Architectural Constructive Criticism:
   - Suggest potential optimizations or clean idioms for Phase 11 (DMA subsystem) integration when implemented in the future.
