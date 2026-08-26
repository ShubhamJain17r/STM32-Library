# Common Utilities & Infrastructure Documentation

## 1. Overview

The `stm32/common` module provides the shared foundation for all peripheral drivers in the library:
* **`registers.hpp` (`namespace reg`)**: Zero-overhead, type-safe register manipulation inline templates.
* **`buffer.hpp` (`namespace buffer`)**: Statically allocated circular ring buffer (`RingBuffer<T, N>`).
* **`callback.hpp` (`namespace stm32`)**: Lightweight function pointer callback abstraction (`void(*)()`).

---

## 2. Register Helper Layer (`namespace reg`)

All register access in the library is routed through `registers.hpp`. By using template argument deduction, these functions work seamlessly with both 32-bit registers (e.g. `GPIOA->MODER`) and 16-bit registers (e.g. `TIM2->PSC` on certain CMSIS headers) without manual casting.

### API Reference
```cpp
namespace reg
{
    // Atomic or direct bit manipulation
    template<typename T, typename M>
    void setBits(volatile T& reg, M mask) noexcept;

    template<typename T, typename M>
    void clearBits(volatile T& reg, M mask) noexcept;

    template<typename T, typename M, typename V>
    void modifyBits(volatile T& reg, M clearMask, V setMask) noexcept;

    template<typename T, typename M, typename V>
    void writeField(volatile T& reg, M clearMask, V value, std::uint8_t pos) noexcept;

    // Bit inspection
    template<typename T, typename M>
    T readBits(const volatile T& reg, M mask) noexcept;

    template<typename T, typename M>
    bool isBitSet(const volatile T& reg, M mask) noexcept;

    template<typename T, typename M>
    bool isAnyBitSet(const volatile T& reg, M mask) noexcept;

    // Direct read/write
    template<typename T, typename V>
    void write(volatile T& reg, V value) noexcept;

    template<typename T>
    T read(const volatile T& reg) noexcept;
}
```

---

## 3. Circular Ring Buffer (`buffer::RingBuffer<T, N>`)

The `RingBuffer` is a fixed-capacity, statically allocated circular FIFO queue with zero dynamic memory allocation.

### API Reference
```cpp
namespace buffer
{
    template<typename T, std::size_t N>
    class RingBuffer
    {
    public:
        bool push(const T& value) noexcept;  // Returns false if full
        bool pop(T& value) noexcept;         // Returns false if empty

        bool empty() const noexcept;
        bool full() const noexcept;
        void clear() noexcept;

        std::size_t size() const noexcept;
        constexpr std::size_t capacity() const noexcept;

        T& front() noexcept;
        const T& front() const noexcept;

        T& back() noexcept;
        const T& back() const noexcept;
    };
}
```

---

## 4. Standard Callback Model (`namespace stm32`)

```cpp
namespace stm32
{
    using Callback = void(*)();
}
```
All ISR callbacks in the library use this zero-overhead function pointer signature.

