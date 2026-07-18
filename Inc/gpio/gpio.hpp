#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include "common/registers.hpp"

namespace gpio {

enum class PinState : std::uint8_t { LOW = 0, HIGH };
enum class Mode : std::uint8_t { INPUT = 0, OUTPUT, ALTERNATE, ANALOG };
enum class OutputType : std::uint8_t { PUSH_PULL = 0, OPEN_DRAIN };
enum class OutputSpeed : std::uint8_t { LOW = 0, MEDIUM, FAST, HIGH };
enum class Pull : std::uint8_t { NONE = 0, UP, DOWN };

enum class AlternateFunction : std::uint8_t {
    AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15
};

class Pin {
private:
    std::uint8_t pinNumber_;
    GPIO_TypeDef *const port_;

    void setMode(Mode) const;
    void setOutputType(OutputType) const;
    void setOutputSpeed(OutputSpeed) const;
    void setPull(Pull) const;
    void setAlternateFunction(AlternateFunction) const;

public:
    constexpr Pin(std::uint8_t pinNumber, GPIO_TypeDef *port) :
        pinNumber_(pinNumber), port_(port) {}

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

inline std::uint32_t GPIO_syscfg_map(const GPIO_TypeDef* port) noexcept {
    if (port < GPIOA || port > GPIOH) return 0;
    return (reinterpret_cast<std::uintptr_t>(port) - GPIOA_BASE) / 0x400;
}

} // namespace gpio
