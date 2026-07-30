#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace gpio
{

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

enum class PinState : std::uint8_t
{
	LOW = 0,
	HIGH
};

struct DigitalInputConfig
{
	Pull pull = Pull::NONE;
	bool enableInterrupt = false;
};

struct DigitalOutputConfig
{
	OutputType outputType = OutputType::PUSH_PULL;
	OutputSpeed outputSpeed = OutputSpeed::LOW;
	Pull pull = Pull::NONE;
	PinState initialState = PinState::LOW;
};

struct AlternateConfig
{
	OutputType outputType = OutputType::PUSH_PULL;
	OutputSpeed outputSpeed = OutputSpeed::LOW;
	Pull pull = Pull::NONE;
};

} // namespace gpio
