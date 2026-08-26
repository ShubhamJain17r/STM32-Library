/**
 * @file gpio_types.hpp
 * @brief Core scoped enums for STM32 GPIO pins (Mode, OutputType, OutputSpeed, Pull, PinState, Edge)
 */

#pragma once

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

enum class Edge
{
	RISING,
	FALLING
};

} // namespace gpio

