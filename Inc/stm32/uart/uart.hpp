#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/uart/uart_config.hpp"

namespace uart
{

template<Instance I>
class UartHandler
{
private:

public:
	explicit UartHandler();

    explicit UartHandler(std::uint32_t baud);

    explicit UartHandler(gpio::Pin tx, gpio::Pin rx);

    explicit UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud);

    explicit UartHandler(const uartConfig<I>& config);

private:

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
};

using Uart1 = UartHandler<Instance::usart1>;
using Uart2 = UartHandler<Instance::usart2>;
using Uart3 = UartHandler<Instance::usart3>;
using Uart4 = UartHandler<Instance::uart4>;
using Uart5 = UartHandler<Instance::uart5>;
using Uart6 = UartHandler<Instance::usart6>;

} // namespace uart
