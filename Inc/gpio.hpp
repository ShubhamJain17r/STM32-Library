#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace gpio
{

enum class PinState : std::uint8_t
{
	LOW = 0,
	HIGH
};

enum class Mode : std::uint8_t
{
	INPUT = 0,
	OUTPUT,
	ALTERNATE,
	ANALOG
};

enum class OutputType : std::uint8_t
{
	PUSH_PULL = 0,
	OPEN_DRAIN
};

enum class OutputSpeed : std::uint8_t
{
	LOW = 0,
	MEDIUM,
	FAST,
	HIGH
};

enum class Pull : std::uint8_t
{
	NONE = 0,
	UP,
	DOWN
};

class Pin
{
private:
	std::uint8_t pinNumber_;
	GPIO_TypeDef* port_;

	void setMode(Mode) const;
	void setOutputType(OutputType) const;
	void setOutputSpeed(OutputSpeed) const;
	void setPull(Pull) const;
	void setAlternateFunction(std::uint8_t) const;

public:
	constexpr Pin(std::uint8_t pinNumber, GPIO_TypeDef* port) :
		pinNumber_(pinNumber), port_(port) {}

	void configureInput(Pull) const;
	void configureOutput(OutputType, OutputSpeed, Pull) const;
	void configureAternate(OutputType, OutputSpeed, Pull, std::uint8_t) const;
	void configureAnalog() const;

	bool isHigh() const;
	bool isLow() const;

	void toggle() const;
	void write(PinState) const;
	PinState read() const;
};

}	// gpio namespace
