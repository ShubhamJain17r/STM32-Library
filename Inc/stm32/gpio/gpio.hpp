#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/pin_map.hpp"
#include "stm32/gpio/gpio_types.hpp"

namespace gpio
{

class DigitalOutput
{
private:
	Pin pin_;

public:
	DigitalOutput() = delete;
	~DigitalOutput() = default;

	DigitalOutput(const DigitalOutput&) = delete;
	DigitalOutput& operator=(const DigitalOutput&) = delete;

	DigitalOutput(DigitalOutput&&) = delete;
	DigitalOutput& operator=(DigitalOutput&&) = delete;

	explicit DigitalOutput(Pin pin, DigitalOutputConfig config = {});

private:

public:

};

class DigitalInput
{
private:
	Pin pin_;

public:
	DigitalInput() = delete;
	~DigitalInput() = default;

	DigitalInput(const DigitalInput&) = delete;
	DigitalInput& operator=(const DigitalInput&) = delete;

	DigitalInput(DigitalInput&&) = delete;
	DigitalInput& operator=(DigitalInput&&) = delete;

	explicit DigitalInput(Pin pin, DigitalInputConfig config = {});
private:

public:

};

} // namespace gpio
