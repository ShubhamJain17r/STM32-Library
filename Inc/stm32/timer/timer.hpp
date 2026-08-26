/**
 * @file timer.hpp
 * @brief Master umbrella header for STM32 Timer Subsystem
 */

#pragma once

#include "stm32/timer/core/timer_types.hpp"
#include "stm32/timer/core/timer_traits.hpp"
#include "stm32/timer/core/timer_af_traits.hpp"
#include "stm32/timer/core/timer_helper.hpp"

#include "stm32/timer/timebase/timer.hpp"
#include "stm32/timer/pwm/pwm.hpp"
#include "stm32/timer/input_capture/input_capture.hpp"
#include "stm32/timer/interrupt/timer_interrupt.hpp"
