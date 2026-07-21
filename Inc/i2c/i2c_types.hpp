#pragma once

#include <cstdint>
#include "stm32f446xx.h"

namespace i2c
{

enum class Operation : std::uint8_t
{
	WRITE = 0,
	READ
};

typedef struct
{

}I2cConfig;

} // namespace i2c
