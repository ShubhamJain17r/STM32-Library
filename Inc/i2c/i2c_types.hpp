#pragma once

#include <cstdint>
#include "stm32f446xx.h"

namespace i2c
{

enum class CommunicationSpeed : std::uint8_t
{
	STANDARD_MODE = 0,
	FAST_MODE
};

enum class OperatingMode
{
	TARGET, CONTROLLER
};

enum class Operation : std::uint8_t
{
	WRITE = 0,
	READ
};

typedef struct
{

}I2cConfig;

} // namespace i2c
