#pragma once

#include <stm32/common/buffer.hpp>
#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/af/af_helper.hpp"

#include "stm32/uart/uart_helper.hpp"
#include "stm32/uart/uart_config.hpp"

#include "stm32/uart/interrupt/uart_interrupt.hpp"

#include "stm32/common/buffer.hpp"

namespace uart
{

template<Instance I>
class UartHandler
{
private:
	inline static buffer::RingBuffer<char, 128> rxBuffer_;
	inline static buffer::RingBuffer<char, 128> txBuffer_;

public:
	explicit UartHandler();

    explicit UartHandler(std::uint32_t baud);

    explicit UartHandler(gpio::Pin tx, gpio::Pin rx);

    explicit UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud);

    explicit UartHandler(const uartConfig<I>& config);

private:
    void handleRXNE() noexcept;
    void handleTXE() noexcept;

    void handleIDLE() noexcept;

public:
    void write(char) noexcept;
    void write(std::uint8_t) noexcept;
    void write(const char*) noexcept;
    void write(const std::uint8_t*, std::size_t) noexcept;

    char read() noexcept;
    std::uint8_t readByte() noexcept;
    void read(std::uint8_t*, std::size_t) noexcept;

    bool available() const noexcept;

    void enable() noexcept;
    void disable() noexcept;

    void attachTransferCompleteCallback(Callback cb) noexcept;
};

template<Instance I>
UartHandler<I>::UartHandler()
    : UartHandler(uartConfig<I>{})
{
}

template<Instance I>
UartHandler<I>::UartHandler(std::uint32_t baud)
    : UartHandler(uartConfig<I>{baud})
{
}

template<Instance I>
UartHandler<I>::UartHandler(gpio::Pin tx,
                            gpio::Pin rx)
    : UartHandler(uartConfig<I>{tx, rx})
{
}

template<Instance I>
UartHandler<I>::UartHandler(gpio::Pin tx,
                            gpio::Pin rx,
                            std::uint32_t baud)
    : UartHandler(uartConfig<I>{tx, rx, baud})
{
}

template<Instance I>
UartHandler<I>::UartHandler(const uartConfig<I>& config)
{
	gpio::af::configure<Traits<I>::txSignal>(config.tx, config.txPinConfig);
	gpio::af::configure<Traits<I>::rxSignal>(config.rx, config.rxPinConfig);

    Traits<I>::enableClock();

    auto* uart = Traits<I>::peripheral();

    interrupt::enableEvent(uart, interrupt::Event::RxNotEmpty);

    helper::setOversampling(uart, config.oversampling);
    helper::setWordLength(uart, config.wordLength);
    helper::setParity(uart, config.parity);
    helper::setStopBits(uart, config.stopBits);
    helper::setMode(uart, config.mode);

    helper::configureBaudRate(uart, config.baud, Traits<I>::bus);

    interrupt::enableIRQ(Traits<I>::irq);

    helper::enable(uart);
}

template<Instance I>
inline void UartHandler<I>::write(char c) noexcept
{
    helper::write(Traits<I>::peripheral(), static_cast<std::uint8_t>(c));
}

template<Instance I>
inline void UartHandler<I>::write(std::uint8_t data) noexcept
{
    helper::write(Traits<I>::peripheral(), data);
}

template<Instance I>
inline void UartHandler<I>::write(const char* str) noexcept
{
	interrupt::enableEvent(uart, interrupt::Event::TxEmpty);
    while(*str)
    {
        write(*str++);
    }
}

template<Instance I>
inline void UartHandler<I>::write(const std::uint8_t* data, std::size_t length) noexcept
{
	interrupt::enableEvent(uart, interrupt::Event::TxEmpty);
    while(length--)
    {
        write(*data++);
    }
}

template<Instance I>
inline char UartHandler<I>::read() noexcept
{
    return static_cast<char>(helper::read(Traits<I>::peripheral()));
}

template<Instance I>
inline std::uint8_t UartHandler<I>::readByte() noexcept
{
    return static_cast<std::uint8_t>(helper::read(Traits<I>::peripheral()));
}

template<Instance I>
inline void UartHandler<I>::read(std::uint8_t* buffer, std::size_t length) noexcept
{
    while(length--)
    {
        *buffer++ = readByte();
    }
}

template<Instance I>
inline bool UartHandler<I>::available() const noexcept
{
    return helper::rxReady(Traits<I>::peripheral());
}

template<Instance I>
inline void UartHandler<I>::enable() noexcept
{
    helper::enable(Traits<I>::peripheral());
}

template<Instance I>
inline void UartHandler<I>::disable() noexcept
{
    helper::disable(Traits<I>::peripheral());
}

template<Instance I>
inline void attachReceiveCallback(Callback cb) noexcept
{
	interrupt::UartEvent::setUserCallback(I, interrupt::Event::RxNotEmpty, cb);
}

template<Instance I>
inline void attachTransmitCallback(Callback cb) noexcept
{
	interrupt::UartEvent::setUserCallback(I, interrupt::Event::TxEmpty, cb);
}

template<Instance I>
inline void attachTransferCompleteCallback(Callback cb) noexcept
{
	interrupt::UartEvent::setUserCallback(I, interrupt::Event::TxComplete, cb);
}

using Uart1 = UartHandler<Instance::usart1>;
using Uart2 = UartHandler<Instance::usart2>;
using Uart3 = UartHandler<Instance::usart3>;
using Uart4 = UartHandler<Instance::uart4>;
using Uart5 = UartHandler<Instance::uart5>;
using Uart6 = UartHandler<Instance::usart6>;

} // namespace uart
