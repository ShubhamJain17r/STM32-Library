#include "gpio/gpio.hpp"
#include "common/registers.hpp"
#include "rcc/rcc.hpp"

namespace gpio {

void Pin::setMode(Mode mode) const {
    reg::setBitField<2>(port_->MODER, pinNumber_ * 2, mode);
}

void Pin::setOutputType(OutputType outputType) const {
    reg::setBitField<1>(port_->OTYPER, pinNumber_, outputType);
}

void Pin::setOutputSpeed(OutputSpeed outputSpeed) const {
    reg::setBitField<2>(port_->OSPEEDR, pinNumber_ * 2, outputSpeed);
}

void Pin::setPull(Pull pull) const {
    reg::setBitField<2>(port_->PUPDR, pinNumber_ * 2, pull);
}

void Pin::setAlternateFunction(AlternateFunction AFType) const {
    reg::setBitField<4>(port_->AFR[pinNumber_ / 8], (pinNumber_ % 8) * 4, AFType);
}

void Pin::configureInput(Pull pull) const {
    rcc::enableClock_GPIO(port_);
    setMode(Mode::INPUT);
    setPull(pull);
}

void Pin::configureOutput(OutputType outputType, OutputSpeed outputSpeed, Pull pull) const {
    rcc::enableClock_GPIO(port_);
    setMode(Mode::OUTPUT);
    setOutputType(outputType);
    setOutputSpeed(outputSpeed);
    setPull(pull);
}

void Pin::configureAlternate(AlternateFunction AFType, OutputType outputType, OutputSpeed outputSpeed, Pull pull) const {
    rcc::enableClock_GPIO(port_);
    setMode(Mode::ALTERNATE);
    setOutputType(outputType);
    setOutputSpeed(outputSpeed);
    setPull(pull);
    setAlternateFunction(AFType);
}

void Pin::configureAnalog() const {
    rcc::enableClock_GPIO(port_);
    setMode(Mode::ANALOG);
}

} // namespace gpio
