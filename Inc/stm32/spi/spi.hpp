/**
 * @file spi.hpp
 * @brief Master umbrella header for STM32 SPI Subsystem
 */

#pragma once

#include "stm32/spi/core/spi_types.hpp"
#include "stm32/spi/core/spi_traits.hpp"
#include "stm32/spi/core/spi_af_traits.hpp"
#include "stm32/spi/core/spi_config.hpp"
#include "stm32/spi/core/spi_helper.hpp"
#include "stm32/spi/interrupt/spi_interrupt.hpp"
#include "stm32/spi/handler/spi_handler.hpp"

namespace spi
{

// ---------------------------------------------------------------------------
// Convenient Type Aliases
// ---------------------------------------------------------------------------
using Spi1 = SpiHandler<Instance::spi1>;
using Spi2 = SpiHandler<Instance::spi2>;
using Spi3 = SpiHandler<Instance::spi3>;
using Spi4 = SpiHandler<Instance::spi4>;

using Spi1Config = SpiConfig<Instance::spi1>;
using Spi2Config = SpiConfig<Instance::spi2>;
using Spi3Config = SpiConfig<Instance::spi3>;
using Spi4Config = SpiConfig<Instance::spi4>;

} // namespace spi
