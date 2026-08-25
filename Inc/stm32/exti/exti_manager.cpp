#include "stm32/exti/exti_manager.hpp"

namespace exti
{

bool ExtiManager::registerPin(const gpio::Pin& pin, Trigger trigger, stm32::Callback cb, EdgeCallback edgeCb) noexcept
{
    if(pin.number >= 16)
    {
        return false;
    }

    auto& entry = table[pin.number];

    // Conflict check: if another port already claimed this line, reject
    if(entry.port != nullptr && entry.port != pin.port)
    {
        return false;
    }

    entry.port         = pin.port;
    entry.trigger      = trigger;
    entry.callback     = cb;
    entry.edgeCallback = edgeCb;

    return true;
}

void ExtiManager::unregisterPin(const gpio::Pin& pin) noexcept
{
    if(pin.number < 16 && table[pin.number].port == pin.port)
    {
        table[pin.number] = ExtiEntry{};
    }
}

void ExtiManager::setCallback(const gpio::Pin& pin, stm32::Callback cb) noexcept
{
    if(pin.number < 16)
    {
        table[pin.number].port = pin.port;
        table[pin.number].callback = cb;
    }
}

void ExtiManager::setEdgeCallback(const gpio::Pin& pin, EdgeCallback cb) noexcept
{
    if(pin.number < 16)
    {
        table[pin.number].port = pin.port;
        table[pin.number].edgeCallback = cb;
    }
}

void ExtiManager::setRisingCallback(const gpio::Pin& pin, stm32::Callback cb) noexcept
{
    if(pin.number < 16)
    {
        table[pin.number].port = pin.port;
        table[pin.number].risingCallback = cb;
    }
}

void ExtiManager::setFallingCallback(const gpio::Pin& pin, stm32::Callback cb) noexcept
{
    if(pin.number < 16)
    {
        table[pin.number].port = pin.port;
        table[pin.number].fallingCallback = cb;
    }
}

void ExtiManager::handleInterrupt(std::uint8_t line) noexcept
{
    clearPending(line);

    if(line >= 16)
    {
        return;
    }

    auto& entry = table[line];

    if(!entry.port)
    {
        return;
    }

    // Deterministic trigger dispatching
    if(entry.trigger == Trigger::Rising)
    {
        if(entry.callback)       entry.callback();
        if(entry.risingCallback) entry.risingCallback();
        if(entry.edgeCallback)   entry.edgeCallback(Edge::Rising);
    }
    else if(entry.trigger == Trigger::Falling)
    {
        if(entry.callback)        entry.callback();
        if(entry.fallingCallback) entry.fallingCallback();
        if(entry.edgeCallback)    entry.edgeCallback(Edge::Falling);
    }
    else if(entry.trigger == Trigger::Both)
    {
        const bool state = reg::isAnyBitSet(entry.port->IDR, 1U << line);
        const Edge edge = state ? Edge::Rising : Edge::Falling;

        if(entry.callback) entry.callback();

        if(edge == Edge::Rising)
        {
            if(entry.risingCallback) entry.risingCallback();
        }
        else
        {
            if(entry.fallingCallback) entry.fallingCallback();
        }

        if(entry.edgeCallback) entry.edgeCallback(edge);
    }
    else
    {
        // Generic fallback if callbacks were attached individually
        if(entry.callback) entry.callback();
        if(entry.risingCallback) entry.risingCallback();
        if(entry.fallingCallback) entry.fallingCallback();
    }
}

} // namespace exti
