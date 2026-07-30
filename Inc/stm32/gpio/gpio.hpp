#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "gpio/pin_map.hpp"

namespace gpio
{

class DigitalOutput
{
private:
	struct Pin pin_;

public:
	DigitalOutput() = delete;
	~DigitalOutput() = default;

	DigitalOutput(const DigitalOutput&) = delete;
	DigitalOutput& operator=(const DigitalOutput&) = delete;

	DigitalOutput(DigitalOutput&&) = default;
	DigitalOutput& operator=(DigitalOutput&&) = default;

	explicit DigitalOutput(Pin pin, DigitalOutputConfig config = {});

private:

public:

};

class DigitalInput
{
private:
	struct Pin pin_;

public:
	DigitalInput() = delete;
	~DigitalInput() = default;

	DigitalInput(const DigitalInput&) = delete;
	DigitalInput& operator=(const DigitalInput&) = delete;

	DigitalInput(DigitalInput&&) = default;
	DigitalInput& operator=(DigitalInput&&) = default;

	explicit DigitalInput(Pin pin, DigitalInputConfig config = {});
private:

public:

};

} // namespace gpio
