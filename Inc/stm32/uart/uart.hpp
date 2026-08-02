#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/uart/uart_traits.hpp"
#include "stm32/uart/uart_config.hpp"

#include "stm32/gpio/pin.hpp"

namespace uart
{

template<USART_TypeDef* Instance>
class UartHandler
{
private:

public:
//	using Traits_ = Traits<Instance>;
//
//	explicit UartHandler();
//
//    explicit UartHandler(uint32_t baud);
//
//    explicit UartHandler(gpio::Pin tx, gpio::Pin rx);
//
//    explicit UartHandler(gpio::Pin tx, gpio::Pin rx, uint32_t baud);
//
//    explicit UartHandler(const uartConfig<Instance>& config);

private:

public:
//    void write(char);
//
//    char read();
//
//    void enable();
//
//    void disable();
};

} // namespace uart
