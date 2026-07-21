#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "common/registers.hpp"
#include "gpio/gpio_types.hpp"

namespace gpio {

class Pin {
private:
    GPIO_TypeDef *const port_;
    std::uint8_t pinNumber_;

public:
	Pin() = delete;

    constexpr Pin(GPIO_TypeDef *port, std::uint8_t pinNumber) :
		port_(port), pinNumber_(pinNumber) {}

    Pin(const Pin&) = delete;
    Pin& operator=(const Pin&) = delete;

    Pin(Pin&&) noexcept = default;
    Pin& operator=(Pin&&) noexcept = default;

private:
    void setMode(Mode) const;
    void setOutputType(OutputType) const;
    void setOutputSpeed(OutputSpeed) const;
    void setPull(Pull) const;
    void setAlternateFunction(AlternateFunction) const;

public:
    void configureInput(Pull pull = Pull::NONE) const;
    void configureOutput(OutputType outputType = OutputType::PUSH_PULL, OutputSpeed outputSpeed = OutputSpeed::LOW, Pull pull = Pull::NONE) const;
    void configureAlternate(AlternateFunction af, OutputType outputType = OutputType::PUSH_PULL, OutputSpeed outputSpeed = OutputSpeed::LOW, Pull pull = Pull::NONE) const;
    void configureAnalog() const;

    inline bool isHigh() const { return reg::readBit(port_->IDR, pinNumber_); }
    inline bool isLow() const  { return !isHigh(); }
    inline PinState read() const { return static_cast<PinState>(reg::readBit(port_->IDR, pinNumber_)); }

    inline void set() const   { port_->BSRR = reg::singleBitMask(pinNumber_); }
    inline void reset() const { port_->BSRR = reg::singleBitMask(pinNumber_ + 16); }

    inline void toggle() const {
        std::uint32_t odr = port_->ODR;
        port_->BSRR = ((odr & reg::singleBitMask(pinNumber_)) << 16) | (~odr & reg::singleBitMask(pinNumber_));
    }

    inline void write(PinState state) const { if (state == PinState::HIGH) set(); else reset(); }

    constexpr std::uint8_t getPinNumber() const { return pinNumber_; }
    constexpr GPIO_TypeDef* getPort() const     { return port_; }
};

inline std::uint32_t mapSyscfgGpio(const GPIO_TypeDef* port) noexcept {
    if (port < GPIOA || port > GPIOH) return 0;
    return (reinterpret_cast<std::uintptr_t>(port) - GPIOA_BASE) / 0x400;
}

} // namespace gpio
