#include "gpio/gpio.hpp"
#include "common/registers.hpp"

namespace gpio {

void Pin::setMode(Mode mode) const {
	reg::setTwoBitFieldValue(port_->MODER, pinNumber_ * 2, mode);
}

void Pin::setOutputType(OutputType outputType) const {
	reg::setBitValue(port_->OTYPER, pinNumber_, outputType);
}

void Pin::setOutputSpeed(OutputSpeed outputSpeed) const {
	reg::setTwoBitFieldValue(port_->OSPEEDR, pinNumber_ * 2, outputSpeed);
}

void Pin::setPull(Pull pull) const {
	reg::setTwoBitFieldValue(port_->PUPDR, pinNumber_ * 2, pull);
}

void Pin::setAlternateFunction(AlternateFunction AFType) const {
	reg::setFourBitFieldValue(port_->AFR[pinNumber_ / 8], (pinNumber_ % 8) * 4,
			AFType);
}

void Pin::configureInput(Pull pull) const {
	setMode(Mode::INPUT);
	setPull(pull);
}

void Pin::configureOutput(OutputType outputType, OutputSpeed outputSpeed,
		Pull pull) const {
	setMode(Mode::OUTPUT);
	setOutputType(outputType);
	setOutputSpeed(outputSpeed);
	setPull(pull);
}

void Pin::configureAlternate(AlternateFunction AFType, OutputType outputType,
		OutputSpeed outputSpeed, Pull pull) const {
	setMode(Mode::ALTERNATE);
	setOutputType(outputType);
	setOutputSpeed(outputSpeed);
	setPull(pull);
	setAlternateFunction(AFType);
}

void Pin::configureAnalog() const {
	setMode(Mode::ANALOG);
}

bool Pin::isHigh() const {
	return reg::readBit(port_->IDR, pinNumber_);
}

bool Pin::isLow() const {
	return !(reg::readBit(port_->IDR, pinNumber_));
}

void Pin::toggle() const {
	if (reg::readBit(port_->ODR, pinNumber_)) {
		reset();
	} else {
		set();
	}
}

void Pin::write(PinState state) const {
	if (state == PinState::HIGH) {
		set();
	} else {
		reset();
	}
}

PinState Pin::read() const {
	return static_cast<PinState>(reg::readBit(port_->IDR, pinNumber_));
}

void Pin::set() const {
	reg::write(port_->BSRR, reg::singleBitMask(pinNumber_));
}

void Pin::reset() const {
	reg::write(port_->BSRR, reg::singleBitMask(pinNumber_ + 16));
}

} // gpio namespace
