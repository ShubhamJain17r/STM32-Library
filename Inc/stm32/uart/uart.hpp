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
//    inline void write(char);
//
//    inline char read();
//
//    inline void enable();
//
//    inline void disable();
};

} // namespace uart
