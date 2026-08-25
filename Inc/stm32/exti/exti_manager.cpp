#include "stm32/exti/exti_manager.hpp"

namespace exti
{

void ExtiManager::setRisingCallback(const gpio::Pin& pin, stm32::Callback cb)
{
    auto& entry = table[pin.number];

    entry.port   = pin.port;
    entry.rising = cb;
}

void ExtiManager::setFallingCallback(const gpio::Pin& pin, stm32::Callback cb)
{
    auto& entry = table[pin.number];

    entry.port    = pin.port;
    entry.falling = cb;
}

void ExtiManager::handleInterrupt(std::uint8_t line)
{
	clearPending(line);

    auto& entry = table[line];

    bool state = entry.port->IDR & (1u << line);

    if(state)
    {
        if(entry.rising)
            entry.rising();
    }
    else
    {
        if(entry.falling)
            entry.falling();
    }
}

} // namespace exti
