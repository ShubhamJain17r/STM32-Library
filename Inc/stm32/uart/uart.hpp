#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers.hpp"
#include "stm32/common/callback.hpp"
#include "stm32/common/buffer.hpp"

#include "stm32/gpio/af/af_helper.hpp"

#include "stm32/uart/uart_helper.hpp"
#include "stm32/uart/uart_config.hpp"

#include "stm32/uart/interrupt/uart_interrupt.hpp"

namespace uart
{

// ----------------------------------------------------------------------------
// UartHandler<Instance, TxBufSize, RxBufSize>
//
// Interrupt-driven UART abstraction.
//
// TX path: write() pushes bytes into txBuf_ and enables TXEIE. The TXE ISR
//          (handleTXE) drains txBuf_ one byte at a time and disables TXEIE
//          when the buffer is empty.
//
// RX path: The RXNE ISR (handleRXNE) reads DR and pushes bytes into rxBuf_.
//          available() and read() operate on rxBuf_, never directly on DR.
//
// Handlers are static member functions — valid plain function pointers with
// no 'this' context required. Per-instance state lives in 'inline static
// Storage storage_', which is valid because only one hardware UART instance
// of each 'I' can physically exist.
//
// TxBufSize, RxBufSize: compile-time ring buffer sizes in bytes. Default 64.
// ----------------------------------------------------------------------------

template<Instance I, std::size_t TxBufSize = 64, std::size_t RxBufSize = 64>
class UartHandler
{
    static_assert(TxBufSize > 0, "TxBufSize must be greater than zero");
    static_assert(RxBufSize > 0, "RxBufSize must be greater than zero");

private:
    struct Storage
    {
        buffer::RingBuffer<std::uint8_t, TxBufSize> txBuf;
        buffer::RingBuffer<std::uint8_t, RxBufSize> rxBuf;
    };

    // One storage instance per template instantiation. Shared by all objects
    // of the same UartHandler<I, TxBufSize, RxBufSize> type.
    inline static Storage storage_;

    // Static ISR handlers — registered as stm32::Callback (void(*)()).
    static void handleRXNE() noexcept;
    static void handleTXE()  noexcept;
    static void handleIDLE() noexcept;

    // Internal single-byte write — pushes to txBuf_ and arms TXEIE.
    static void writeByte(std::uint8_t data) noexcept;

public:
    ~UartHandler() = default;

    UartHandler(const UartHandler&)            = delete;
    UartHandler& operator=(const UartHandler&) = delete;
    UartHandler(UartHandler&&)                 = delete;
    UartHandler& operator=(UartHandler&&)      = delete;

    // Constructors — all delegate to the config constructor.
    explicit UartHandler();
    explicit UartHandler(std::uint32_t baud);
    explicit UartHandler(gpio::Pin tx, gpio::Pin rx);
    explicit UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud);
    explicit UartHandler(const uartConfig<I>& config);

    // TX — queues bytes into txBuf_. Spins only if the TX buffer is full.
    void write(const char* str) noexcept;
    void write(const std::uint8_t* data, std::size_t length) noexcept;

    // RX — drains rxBuf_. read()/readByte() block until a byte is available.
    char         read()                               noexcept;
    std::uint8_t readByte()                           noexcept;
    void         read(std::uint8_t* buf, std::size_t length) noexcept;

    bool available() const noexcept;

    void enable()  noexcept;
    void disable() noexcept;

    // User-level callbacks invoked from the ISR after developer handlers run.
    void attachTransferCompleteCallback(stm32::Callback cb) noexcept;
    void attachReceiveCompleteCallback(stm32::Callback cb)  noexcept;
};

// --- Constructor implementations --------------------------------------------

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
UartHandler<I, TxBufSize, RxBufSize>::UartHandler()
    : UartHandler(uartConfig<I>{})
{}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
UartHandler<I, TxBufSize, RxBufSize>::UartHandler(std::uint32_t baud)
    : UartHandler(uartConfig<I>{baud})
{}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
UartHandler<I, TxBufSize, RxBufSize>::UartHandler(gpio::Pin tx, gpio::Pin rx)
    : UartHandler(uartConfig<I>{tx, rx})
{}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
UartHandler<I, TxBufSize, RxBufSize>::UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud)
    : UartHandler(uartConfig<I>{tx, rx, baud})
{}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
UartHandler<I, TxBufSize, RxBufSize>::UartHandler(const uartConfig<I>& config)
{
    // 1. Configure GPIO alternate functions for TX and RX pins.
    gpio::af::configure<Traits<I>::txSignal>(config.tx, config.txPinConfig);
    gpio::af::configure<Traits<I>::rxSignal>(config.rx, config.rxPinConfig);

    // 2. Enable the UART peripheral clock.
    Traits<I>::enableClock();

    auto* uart = Traits<I>::peripheral();

    // 3. Register static handlers with the interrupt dispatch layer.
    //    Static member functions have type void(*)() — no UB, no this-pointer needed.
    interrupt::UartEvent::setDeveloperCallback(I, interrupt::Event::RxNotEmpty, &UartHandler::handleRXNE);
    interrupt::UartEvent::setDeveloperCallback(I, interrupt::Event::TxEmpty,    &UartHandler::handleTXE);
    interrupt::UartEvent::setDeveloperCallback(I, interrupt::Event::IdleState,  &UartHandler::handleIDLE);

    // 4. Enable hardware interrupt sources.
    //    TXEIE is NOT enabled here — it is enabled on-demand in writeByte()
    //    only when data is queued, to avoid a spurious immediate TXE interrupt.
    interrupt::enableEvent(uart, interrupt::Event::RxNotEmpty);
    interrupt::enableEvent(uart, interrupt::Event::IdleState);

    // 5. Configure UART registers.
    helper::setOversampling(uart, config.oversampling);
    helper::setWordLength(uart, config.wordLength);
    helper::setParity(uart, config.parity);
    helper::setStopBits(uart, config.stopBits);
    helper::setMode(uart, config.mode);

    // configureBaudRate reads CR1.OVER8, which must be set before this call.
    helper::configureBaudRate(uart, config.baud, Traits<I>::bus);

    // 6. Enable the NVIC line and the UART peripheral.
    interrupt::enableIRQ(Traits<I>::irq);
    helper::enable(uart);
}

// --- Static ISR handlers ----------------------------------------------------

// handleRXNE: called by the ISR when a new byte has arrived in DR.
// Reading DR clears the RXNE flag. Drop silently if rxBuf_ is full.
template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::handleRXNE() noexcept
{
    const std::uint8_t byte = static_cast<std::uint8_t>(Traits<I>::peripheral()->DR);
    const std::uint8_t byte = static_cast<std::uint8_t>(reg::read(Traits<I>::peripheral()->DR));
    storage_.rxBuf.push(byte); // returns false if full; byte is discarded
}

// handleTXE: called by the ISR when DR is ready for the next TX byte.
// If the TX buffer has data, write the next byte. If empty, disarm TXEIE
// so the ISR stops firing.
template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::handleTXE() noexcept
{
    std::uint8_t byte;

    if(storage_.txBuf.pop(byte))
    {
        Traits<I>::peripheral()->DR = byte;
        reg::write(Traits<I>::peripheral()->DR, byte);
    }
    else
    {
        // Buffer drained — disarm TXEIE. writeByte() will re-arm it on the
        // next write.
        interrupt::disableEvent(Traits<I>::peripheral(), interrupt::Event::TxEmpty);
    }
}

// handleIDLE: called by the ISR when the RX line has been idle for one
// frame duration after receiving data (useful for framing variable-length
// messages).
// The IDLE flag (STM32F4) is cleared by reading SR then DR. The SR was
// already read in handleEvent(). Reading DR here completes the sequence.
// By the time IDLE fires, all pending RXNE events have been processed first
// inside handleEvent(), so this DR read does not consume a valid byte.
template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::handleIDLE() noexcept
{
    volatile std::uint32_t tmp = Traits<I>::peripheral()->SR;
    tmp = Traits<I>::peripheral()->DR;
    volatile std::uint32_t tmp = reg::read(Traits<I>::peripheral()->SR);
    tmp = reg::read(Traits<I>::peripheral()->DR);
    (void)tmp;
}

// --- writeByte --------------------------------------------------------------

// Pushes one byte into txBuf_ and arms TXEIE.
// Spins (with interrupts enabled) if the TX buffer is full, allowing the TXE
// ISR to drain it before we continue.
// A brief critical section (disable_irq) protects the RingBuffer count_ field
// from a concurrent ISR pop.
template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::writeByte(std::uint8_t data) noexcept
{
    // Spin with interrupts enabled so the TXE ISR can drain the buffer.
    while(storage_.txBuf.full())
    {
    }

    // Protect the push from concurrent ISR pop modifying count_.
    __disable_irq();
    storage_.txBuf.push(data);
    __enable_irq();

    // Arm TXEIE. If the UART is idle (TXE=1), the ISR fires immediately on
    // the next instruction and begins draining. If UART is mid-byte, it fires
    // after the current byte completes.
    interrupt::enableEvent(Traits<I>::peripheral(), interrupt::Event::TxEmpty);
}

// --- Public write / read ---------------------------------------------------

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::write(const char* str) noexcept
{
    while(*str)
    {
        writeByte(static_cast<std::uint8_t>(*str++));
    }
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::write(const std::uint8_t* data, std::size_t length) noexcept
{
    while(length--)
    {
        writeByte(*data++);
    }
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
char UartHandler<I, TxBufSize, RxBufSize>::read() noexcept
{
    return static_cast<char>(readByte());
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
std::uint8_t UartHandler<I, TxBufSize, RxBufSize>::readByte() noexcept
{
    std::uint8_t byte;

    // Spin until the RXNE ISR pushes a byte into rxBuf_.
    while(true)
    {
        __disable_irq();
        const bool popped = storage_.rxBuf.pop(byte);
        __enable_irq();

        if(popped)
        {
            return byte;
        }
    }
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::read(std::uint8_t* buf, std::size_t length) noexcept
{
    while(length--)
    {
        *buf++ = readByte();
    }
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
bool UartHandler<I, TxBufSize, RxBufSize>::available() const noexcept
{
    __disable_irq();
    const bool notEmpty = !storage_.rxBuf.empty();
    __enable_irq();
    return notEmpty;
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::enable() noexcept
{
    helper::enable(Traits<I>::peripheral());
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::disable() noexcept
{
    helper::disable(Traits<I>::peripheral());
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::attachTransferCompleteCallback(stm32::Callback cb) noexcept
{
    interrupt::UartEvent::setUserCallback(I, interrupt::Event::TxComplete, cb);
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::attachReceiveCompleteCallback(stm32::Callback cb) noexcept
{
    interrupt::UartEvent::setUserCallback(I, interrupt::Event::IdleState, cb);
}

// --- Convenience aliases ---------------------------------------------------
// Default buffer sizes (64 bytes TX, 64 bytes RX).
// For non-default sizes: UartHandler<Instance::usart2, 128, 256> serial;

using Uart1 = UartHandler<Instance::usart1>;
using Uart2 = UartHandler<Instance::usart2>;
using Uart3 = UartHandler<Instance::usart3>;
using Uart4 = UartHandler<Instance::uart4>;
using Uart5 = UartHandler<Instance::uart5>;
using Uart6 = UartHandler<Instance::usart6>;

} // namespace uart

