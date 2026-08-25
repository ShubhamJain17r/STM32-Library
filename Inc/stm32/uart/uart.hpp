/**
 * @file uart.hpp
 * @brief High-level asynchronous, interrupt-driven UART driver
 * 
 * Provides:
 * - Non-blocking interrupt-driven transmission & reception using ring buffers
 * - Type aliases for all USART/UART instances (Uart1 through Uart6)
 * - Configurable baud rate, oversampling, parity, stop bits, and word length
 * - Clean stream API: write(str), write(data, len), read(), readByte(), available()
 * - User-level callbacks for Transfer Complete (TC) and Idle Line Detection (IDLE)
 */

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

    inline static Storage storage_;

    static void handleRXNE() noexcept;
    static void handleTXE()  noexcept;
    static void handleIDLE() noexcept;

    static void writeByte(std::uint8_t data) noexcept;

public:
    ~UartHandler() = default;

    UartHandler(const UartHandler&)            = delete;
    UartHandler& operator=(const UartHandler&) = delete;
    UartHandler(UartHandler&&)                 = delete;
    UartHandler& operator=(UartHandler&&)      = delete;

    explicit UartHandler();
    explicit UartHandler(std::uint32_t baud);
    explicit UartHandler(gpio::Pin tx, gpio::Pin rx);
    explicit UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud);
    explicit UartHandler(const uartConfig<I>& config);

    /**
     * @brief Writes a null-terminated string to the TX buffer.
     */
    void write(const char* str) noexcept;

    /**
     * @brief Writes a byte array of specified length to the TX buffer.
     */
    void write(const std::uint8_t* data, std::size_t length) noexcept;

    /**
     * @brief Reads a single character from the RX buffer (blocks if empty).
     */
    char read() noexcept;

    /**
     * @brief Reads a single byte from the RX buffer (blocks if empty).
     */
    std::uint8_t readByte() noexcept;

    /**
     * @brief Reads multiple bytes from the RX buffer into a destination buffer.
     */
    void read(std::uint8_t* buf, std::size_t length) noexcept;

    /**
     * @brief Returns true if one or more bytes are available in the RX buffer.
     */
    bool available() const noexcept;

    void enable()  noexcept;
    void disable() noexcept;

    void attachTransferCompleteCallback(stm32::Callback cb) noexcept;
    void attachReceiveCompleteCallback(stm32::Callback cb)  noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

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
    interrupt::UartEvent::setDeveloperCallback(I, interrupt::Event::RxNotEmpty, &UartHandler::handleRXNE);
    interrupt::UartEvent::setDeveloperCallback(I, interrupt::Event::TxEmpty,    &UartHandler::handleTXE);
    interrupt::UartEvent::setDeveloperCallback(I, interrupt::Event::IdleState,  &UartHandler::handleIDLE);

    // 4. Enable hardware interrupt sources.
    interrupt::enableEvent(uart, interrupt::Event::RxNotEmpty);
    interrupt::enableEvent(uart, interrupt::Event::IdleState);

    // 5. Configure UART registers.
    helper::setOversampling(uart, config.oversampling);
    helper::setWordLength(uart, config.wordLength);
    helper::setParity(uart, config.parity);
    helper::setStopBits(uart, config.stopBits);
    helper::setMode(uart, config.mode);

    helper::configureBaudRate(uart, config.baud, Traits<I>::bus);

    // 6. Enable the NVIC line and the UART peripheral.
    interrupt::enableIRQ(Traits<I>::irq);
    helper::enable(uart);
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::handleRXNE() noexcept
{
    const std::uint8_t byte = static_cast<std::uint8_t>(reg::read(Traits<I>::peripheral()->DR));
    storage_.rxBuf.push(byte);
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::handleTXE() noexcept
{
    std::uint8_t byte;

    if(storage_.txBuf.pop(byte))
    {
        reg::write(Traits<I>::peripheral()->DR, byte);
    }
    else
    {
        interrupt::disableEvent(Traits<I>::peripheral(), interrupt::Event::TxEmpty);
    }
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::handleIDLE() noexcept
{
    volatile std::uint32_t tmp = reg::read(Traits<I>::peripheral()->SR);
    tmp = reg::read(Traits<I>::peripheral()->DR);
    (void)tmp;
}

template<Instance I, std::size_t TxBufSize, std::size_t RxBufSize>
void UartHandler<I, TxBufSize, RxBufSize>::writeByte(std::uint8_t data) noexcept
{
    while(true)
    {
        __disable_irq();
        const bool pushed = storage_.txBuf.push(data);
        __enable_irq();

        if(pushed)
        {
            break;
        }
    }

    interrupt::enableEvent(Traits<I>::peripheral(), interrupt::Event::TxEmpty);
}

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

// ---------------------------------------------------------------------------
// Convenient Type Aliases
// ---------------------------------------------------------------------------
using Uart1 = UartHandler<Instance::usart1>;
using Uart2 = UartHandler<Instance::usart2>;
using Uart3 = UartHandler<Instance::usart3>;
using Uart4 = UartHandler<Instance::uart4>;
using Uart5 = UartHandler<Instance::uart5>;
using Uart6 = UartHandler<Instance::usart6>;

} // namespace uart
