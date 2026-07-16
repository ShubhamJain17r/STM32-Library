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

enum class AlternateFunction : uint8_t
{
    AF0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8,
    AF9,
    AF10,
    AF11,
    AF12,
    AF13,
    AF14,
    AF15
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
	void setAlternateFunction(AlternateFunction) const;

public:
	constexpr Pin(std::uint8_t pinNumber, GPIO_TypeDef* port) :
		pinNumber_(pinNumber), port_(port) {}

	void configureInput(Pull pull = Pull::NONE) const;

	void configureOutput(
	    OutputType outputType = OutputType::PUSH_PULL,
	    OutputSpeed outputSpeed = OutputSpeed::LOW,
	    Pull pull = Pull::NONE) const;

	void configureAlternate(
		AlternateFunction,
	    OutputType outputType = OutputType::PUSH_PULL,
	    OutputSpeed outputSpeed = OutputSpeed::LOW,
	    Pull pull = Pull::NONE) const;

	void configureAnalog() const;

	bool isHigh() const;
	bool isLow() const;

	void toggle() const;
	void write(PinState) const;
	PinState read() const;
};

}	// gpio namespace
