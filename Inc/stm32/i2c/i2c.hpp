/**
 * @file i2c.hpp
 * @brief Master umbrella header for STM32 I2C Subsystem
 */

#pragma once

#include "stm32/i2c/core/i2c_types.hpp"
#include "stm32/i2c/core/i2c_traits.hpp"
#include "stm32/i2c/core/i2c_af_traits.hpp"
#include "stm32/i2c/core/i2c_config.hpp"
#include "stm32/i2c/core/i2c_helper.hpp"
#include "stm32/i2c/interrupt/i2c_interrupt.hpp"
#include "stm32/i2c/handler/i2c_master.hpp"

namespace i2c
{

// ---------------------------------------------------------------------------
// Convenient Type Aliases
// ---------------------------------------------------------------------------
using I2c1 = I2cMaster<Instance::i2c1>;
using I2c2 = I2cMaster<Instance::i2c2>;
using I2c3 = I2cMaster<Instance::i2c3>;

using I2c1Config = I2cConfig<Instance::i2c1>;
using I2c2Config = I2cConfig<Instance::i2c2>;
using I2c3Config = I2cConfig<Instance::i2c3>;

} // namespace i2c

