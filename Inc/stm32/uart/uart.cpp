#include "stm32/uart/uart.hpp"

#include "stm32/gpio/gpio_helper.hpp"

#include "stm32/gpio/af/af_helper.hpp"

#include "stm32/uart/uart_helper.hpp"

namespace uart
{

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
	gpio::af::configure<Traits<I>::rxSignal>(config.tx, config.txPinConfig);

    Traits<I>::enableClock();

    auto* uart = Traits<I>::peripheral();

    helper::setOversampling(uart, config.oversampling);
    helper::setWordLength(uart, config.wordLength);
    helper::setParity(uart, config.parity);
    helper::setStopBits(uart, config.stopBits);
    helper::setMode(uart, config.mode);

    helper::configureBaudRate(uart, config.baud, Traits<I>::bus);

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
    while(*str)
    {
        write(*str++);
    }
}

template<Instance I>
inline void UartHandler<I>::write(const std::uint8_t* data, std::size_t length) noexcept
{
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

} // namespace uart
