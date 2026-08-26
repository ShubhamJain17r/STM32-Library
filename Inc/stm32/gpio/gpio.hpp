/**
 * @file gpio.hpp
 * @brief Master umbrella header for STM32 GPIO Subsystem
 */

#pragma once

#include "stm32/gpio/core/gpio_types.hpp"
#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/pin_map.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"

#include "stm32/gpio/digital_output/digital_output.hpp"
#include "stm32/gpio/digital_input/digital_input.hpp"
#include "stm32/gpio/interrupt_input/interrupt_input.hpp"

#include "stm32/gpio/af/af_types.hpp"
#include "stm32/gpio/af/af_traits.hpp"
#include "stm32/gpio/af/af_helper.hpp"
#include "stm32/gpio/af/af_config.hpp"
