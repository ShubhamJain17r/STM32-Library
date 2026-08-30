# Common Utilities Subsystem

## 1. Overview & Architecture

The Common Utilities subsystem provides core bare-metal data structures, static type traits, and register-level primitives used across the entire library:
* **`reg::` Bit Manipulation Templates**: Zero-overhead inline functions for reading, writing, setting, clearing, and modifying hardware registers.
* **`stm32::RingBuffer<T, N>`**: Static compile-time lock-free circular FIFO ring buffer with power-of-two bitmask indexing.
* **`stm32::Callback`**: Standard function pointer type for interrupt and event callbacks (`void (*)()`).

---

## 2. Feature Matrix: Implemented vs Architecture

| Feature | Description | Implementation | Status |
| :--- | :--- | :--- | :--- |
| **`reg::read` / `reg::write`** | Direct atomic volatile 32-bit register access | `stm32/common/registers/registers.hpp` | 🟢 Implemented |
| **`reg::setBits` / `reg::clearBits`** | Atomic/Masked bitwise register operations | `stm32/common/registers/registers.hpp` | 🟢 Implemented |
| **`reg::modifyBits`** | Clear mask + OR value in single step | `stm32/common/registers/registers.hpp` | 🟢 Implemented |
| **Static `RingBuffer<T, N>`** | Zero heap allocation circular FIFO | `stm32/common/ring_buffer/ring_buffer.hpp` | 🟢 Implemented |
| **`stm32::Callback`** | Deterministic function pointer alias | `stm32/common/callback/callback.hpp` | 🟢 Implemented |

---

## 3. API Reference & Syntax

```cpp
#include "stm32/common/registers/registers.hpp"
#include "stm32/common/ring_buffer/ring_buffer.hpp"
#include "stm32/common/callback/callback.hpp"

// 1. Bitwise Register Operations
reg::setBits(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
reg::clearBits(USART1->CR1, USART_CR1_UE);
reg::modifyBits(GPIOA->MODER, 0x3 << 10, 0x1 << 10);

// 2. Static RingBuffer (Power-of-2 size)
stm32::RingBuffer<std::uint8_t, 128> fifo;
fifo.push(0x55);
std::uint8_t data = 0;
if(fifo.pop(data)) {
    // Successfully retrieved byte
}
```

