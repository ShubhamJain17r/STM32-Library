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
    void write(char);
    void write(std::uint8_t);
    void write(const char*);
    void write(const std::uint8_t*, std::size_t);

    char read();
    std::uint8_t readByte();
    void read(std::uint8_t*, std::size_t);

    bool available() const;

    void enable();
    void disable();
};

} // namespace uart
