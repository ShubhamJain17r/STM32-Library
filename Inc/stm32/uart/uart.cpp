#include "stm32/uart/uart.hpp"

namespace uart
{

template<Instance I>
UartHandler<I>::UartHandler()
{

}

template<Instance I>
UartHandler<I>::UartHandler(std::uint32_t baud)
{

}

template<Instance I>
UartHandler<I>::UartHandler(gpio::Pin tx, gpio::Pin rx)
{

}

template<Instance I>
UartHandler<I>::UartHandler(gpio::Pin tx, gpio::Pin rx, std::uint32_t baud)
{

}

template<Instance I>
UartHandler<I>::UartHandler(const uartConfig<I>& config)
{

}

} // namespace uart
