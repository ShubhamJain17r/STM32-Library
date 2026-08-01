#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace i2c
{

template<I2C_TypeDef*>
struct I2cTraits;

template<>
struct I2cTraits<I2C1>
{
	static constexpr IRQn_Type eventIrq = I2C1_EV_IRQn;
	static constexpr IRQn_Type errorIrq = I2C1_ER_IRQn;
};

} // namespace i2c
