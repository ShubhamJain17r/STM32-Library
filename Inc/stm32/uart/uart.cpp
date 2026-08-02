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
	// TODO : gpio configuration
	gpio::af::configure<Traits<I>::txSignal>(config.tx, config.txPinConfig);
	gpio::af::configure<Traits<I>::rxSignal>(config.tx, config.txPinConfig);

    Traits<I>::enableClock();

    auto* uart = Traits<I>::peripheral();

    helper::setOversampling(uart, config.oversampling);
    helper::setWordLength(uart, config.wordLength);
    helper::setParity(uart, config.parity);
    helper::setStopBits(uart, config.stopBits);
    helper::setMode(uart, config.mode);

    helper::configureBaudRate(uart, config.baud);

    helper::enable(uart);
}

} // namespace uart
