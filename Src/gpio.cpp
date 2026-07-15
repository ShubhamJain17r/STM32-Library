#include "gpio.hpp"

namespace gpio
{

void Pin::setMode(Mode mode) const
{
	setTwoBitField(port_->MODER, pinNumber_ * 2, mode);
}

void Pin::setOutputType(OutputType outputType) const
{
	setBit(port_->OTYPER, pinNumber_, outputType);
}

void Pin::setOutputSpeed(OutputSpeed outputSpeed) const
{
	setTwoBitField(port_->OSPEEDR, pinNumber_ * 2, outputSpeed);
}

void Pin::setPull(Pull pull) const
{
	setTwoBitField(port_->PUPDR, pinNumber_ * 2, pull);
}
void Pin::setAlternateFunction(std::uint8_t AFType) const
{
	setFourBitField(port_->AFR[pinNumber_ / 8], (pinNumber_ % 8) * 4, AFType);
}

void Pin::configureInput(Pull pull) const
{	setMode(Mode::INPUT);
	setPull(pull);
}

void Pin::configureOutput(OutputType outputType, OutputSpeed outputSpeed, Pull pull) const
{	setMode(Mode::OUTPUT);
	setOutputType(outputType);
	setOutputSpeed(outputSpeed);
	setPull(pull);
}

void Pin::configureAternate(OutputType outputType, OutputSpeed outputSpeed, Pull pull, std::uint8_t AFType) const
{	setMode(Mode::ALTERNATE);
	setOutputType(outputType);
	setOutputSpeed(outputSpeed);
	setPull(pull);
	setAlternateFunction(AFType);
}

void Pin::configureAnalog() const
{	setMode(Mode::ANALOG);}

bool Pin::isHigh() const
{

}

bool Pin::isLow() const
{

}

void Pin::toggle() const
{

}
void Pin::write(PinState state) const
{

}

PinState Pin::read() const
{

}

} // gpio namespace
