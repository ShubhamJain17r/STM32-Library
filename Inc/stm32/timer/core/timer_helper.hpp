/**
 * @file timer_helper.hpp
 * @brief Low-level register manipulation helper functions for Timers, PWM, OC, and IC
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/timer/core/timer_types.hpp"

namespace timer::helper
{

inline void setPrescaler(TIM_TypeDef* timer, std::uint32_t psc) noexcept
{
    reg::write(timer->PSC, psc);
}

inline std::uint32_t getPrescaler(const TIM_TypeDef* timer) noexcept
{
    return reg::read(timer->PSC);
}

inline void setAutoReload(TIM_TypeDef* timer, std::uint32_t arr) noexcept
{
    reg::write(timer->ARR, arr);
}

inline std::uint32_t getAutoReload(const TIM_TypeDef* timer) noexcept
{
    return reg::read(timer->ARR);
}

inline void setCounter(TIM_TypeDef* timer, std::uint32_t cnt) noexcept
{
    reg::write(timer->CNT, cnt);
}

inline std::uint32_t getCounter(const TIM_TypeDef* timer) noexcept
{
    return reg::read(timer->CNT);
}

inline void enableCounter(TIM_TypeDef* timer) noexcept
{
    reg::setBits(timer->CR1, TIM_CR1_CEN);
}

inline void disableCounter(TIM_TypeDef* timer) noexcept
{
    reg::clearBits(timer->CR1, TIM_CR1_CEN);
}

inline bool isCounterEnabled(const TIM_TypeDef* timer) noexcept
{
    return reg::isAnyBitSet(timer->CR1, TIM_CR1_CEN);
}

inline void setOnePulseMode(TIM_TypeDef* timer, bool enable) noexcept
{
    if(enable)
        reg::setBits(timer->CR1, TIM_CR1_OPM);
    else
        reg::clearBits(timer->CR1, TIM_CR1_OPM);
}

inline void setCounterMode(TIM_TypeDef* timer, CounterMode mode) noexcept
{
    switch(mode)
    {
        case CounterMode::Up:
            reg::clearBits(timer->CR1, TIM_CR1_DIR | TIM_CR1_CMS);
            break;
        case CounterMode::Down:
            reg::modifyBits(timer->CR1, TIM_CR1_CMS, TIM_CR1_DIR);
            break;
        case CounterMode::CenterAligned1:
            reg::modifyBits(timer->CR1, TIM_CR1_CMS, TIM_CR1_CMS_0);
            break;
        case CounterMode::CenterAligned2:
            reg::modifyBits(timer->CR1, TIM_CR1_CMS, TIM_CR1_CMS_1);
            break;
        case CounterMode::CenterAligned3:
            reg::modifyBits(timer->CR1, TIM_CR1_CMS, TIM_CR1_CMS);
            break;
    }
}

inline void enableAutoReloadPreload(TIM_TypeDef* timer) noexcept
{
    reg::setBits(timer->CR1, TIM_CR1_ARPE);
}

inline void disableAutoReloadPreload(TIM_TypeDef* timer) noexcept
{
    reg::clearBits(timer->CR1, TIM_CR1_ARPE);
}

inline void generateUpdate(TIM_TypeDef* timer) noexcept
{
    reg::setBits(timer->EGR, TIM_EGR_UG);
}

inline void enableUpdateInterrupt(TIM_TypeDef* timer) noexcept
{
    reg::setBits(timer->DIER, TIM_DIER_UIE);
}

inline void disableUpdateInterrupt(TIM_TypeDef* timer) noexcept
{
    reg::clearBits(timer->DIER, TIM_DIER_UIE);
}

inline void clearUpdateFlag(TIM_TypeDef* timer) noexcept
{
    reg::clearBits(timer->SR, TIM_SR_UIF);
}

inline bool isUpdateFlag(const TIM_TypeDef* timer) noexcept
{
    return reg::isAnyBitSet(timer->SR, TIM_SR_UIF);
}

inline void enableChannelInterrupt(TIM_TypeDef* timer, Channel ch) noexcept
{
    reg::setBits(timer->DIER, 1U << static_cast<std::uint8_t>(ch));
}

inline void disableChannelInterrupt(TIM_TypeDef* timer, Channel ch) noexcept
{
    reg::clearBits(timer->DIER, 1U << static_cast<std::uint8_t>(ch));
}

inline void clearChannelFlag(TIM_TypeDef* timer, Channel ch) noexcept
{
    reg::clearBits(timer->SR, 1U << static_cast<std::uint8_t>(ch));
}

inline bool isChannelFlag(const TIM_TypeDef* timer, Channel ch) noexcept
{
    return reg::isAnyBitSet(timer->SR, 1U << static_cast<std::uint8_t>(ch));
}

inline void setCompare(TIM_TypeDef* timer, Channel ch, std::uint32_t value) noexcept
{
    switch(ch)
    {
        case Channel::CH1: reg::write(timer->CCR1, value); break;
        case Channel::CH2: reg::write(timer->CCR2, value); break;
        case Channel::CH3: reg::write(timer->CCR3, value); break;
        case Channel::CH4: reg::write(timer->CCR4, value); break;
    }
}

inline std::uint32_t getCompare(const TIM_TypeDef* timer, Channel ch) noexcept
{
    switch(ch)
    {
        case Channel::CH1: return reg::read(timer->CCR1);
        case Channel::CH2: return reg::read(timer->CCR2);
        case Channel::CH3: return reg::read(timer->CCR3);
        case Channel::CH4: return reg::read(timer->CCR4);
    }
    return 0U;
}

inline std::uint32_t getCapture(const TIM_TypeDef* timer, Channel ch) noexcept
{
    return getCompare(timer, ch);
}

inline void configureOutputCompare(TIM_TypeDef* timer, Channel ch, OutputCompareMode mode, Polarity polarity) noexcept
{
    const auto modeVal = static_cast<std::uint32_t>(mode);

    if(ch == Channel::CH1)
    {
        reg::modifyBits(timer->CCMR1, 0x00FFU, (modeVal << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE);
        if(polarity == Polarity::ActiveLow)
            reg::setBits(timer->CCER, TIM_CCER_CC1P);
        else
            reg::clearBits(timer->CCER, TIM_CCER_CC1P);
    }
    else if(ch == Channel::CH2)
    {
        reg::modifyBits(timer->CCMR1, 0xFF00U, (modeVal << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE);
        if(polarity == Polarity::ActiveLow)
            reg::setBits(timer->CCER, TIM_CCER_CC2P);
        else
            reg::clearBits(timer->CCER, TIM_CCER_CC2P);
    }
    else if(ch == Channel::CH3)
    {
        reg::modifyBits(timer->CCMR2, 0x00FFU, (modeVal << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE);
        if(polarity == Polarity::ActiveLow)
            reg::setBits(timer->CCER, TIM_CCER_CC3P);
        else
            reg::clearBits(timer->CCER, TIM_CCER_CC3P);
    }
    else if(ch == Channel::CH4)
    {
        reg::modifyBits(timer->CCMR2, 0xFF00U, (modeVal << TIM_CCMR2_OC4M_Pos) | TIM_CCMR2_OC4PE);
        if(polarity == Polarity::ActiveLow)
            reg::setBits(timer->CCER, TIM_CCER_CC4P);
        else
            reg::clearBits(timer->CCER, TIM_CCER_CC4P);
    }
}

inline void enableChannelOutput(TIM_TypeDef* timer, Channel ch) noexcept
{
    switch(ch)
    {
        case Channel::CH1: reg::setBits(timer->CCER, TIM_CCER_CC1E); break;
        case Channel::CH2: reg::setBits(timer->CCER, TIM_CCER_CC2E); break;
        case Channel::CH3: reg::setBits(timer->CCER, TIM_CCER_CC3E); break;
        case Channel::CH4: reg::setBits(timer->CCER, TIM_CCER_CC4E); break;
    }
}

inline void disableChannelOutput(TIM_TypeDef* timer, Channel ch) noexcept
{
    switch(ch)
    {
        case Channel::CH1: reg::clearBits(timer->CCER, TIM_CCER_CC1E); break;
        case Channel::CH2: reg::clearBits(timer->CCER, TIM_CCER_CC2E); break;
        case Channel::CH3: reg::clearBits(timer->CCER, TIM_CCER_CC3E); break;
        case Channel::CH4: reg::clearBits(timer->CCER, TIM_CCER_CC4E); break;
    }
}

inline void enableMainOutput(TIM_TypeDef* timer) noexcept
{
    reg::setBits(timer->BDTR, TIM_BDTR_MOE);
}

inline void disableMainOutput(TIM_TypeDef* timer) noexcept
{
    reg::clearBits(timer->BDTR, TIM_BDTR_MOE);
}

inline void configureInputCapture(TIM_TypeDef* timer, Channel ch, CaptureEdge edge, InputPrescaler psc) noexcept
{
    const auto pscVal = static_cast<std::uint32_t>(psc);

    if(ch == Channel::CH1)
    {
        reg::modifyBits(timer->CCMR1, 0x00FFU, TIM_CCMR1_CC1S_0 | (pscVal << TIM_CCMR1_IC1PSC_Pos));
        if(edge == CaptureEdge::Rising)
            reg::clearBits(timer->CCER, TIM_CCER_CC1P | TIM_CCER_CC1NP);
        else if(edge == CaptureEdge::Falling)
            reg::modifyBits(timer->CCER, TIM_CCER_CC1P | TIM_CCER_CC1NP, TIM_CCER_CC1P);
        else // Both
            reg::setBits(timer->CCER, TIM_CCER_CC1P | TIM_CCER_CC1NP);
    }
    else if(ch == Channel::CH2)
    {
        reg::modifyBits(timer->CCMR1, 0xFF00U, TIM_CCMR1_CC2S_0 | (pscVal << TIM_CCMR1_IC2PSC_Pos));
        if(edge == CaptureEdge::Rising)
            reg::clearBits(timer->CCER, TIM_CCER_CC2P | TIM_CCER_CC2NP);
        else if(edge == CaptureEdge::Falling)
            reg::modifyBits(timer->CCER, TIM_CCER_CC2P | TIM_CCER_CC2NP, TIM_CCER_CC2P);
        else
            reg::setBits(timer->CCER, TIM_CCER_CC2P | TIM_CCER_CC2NP);
    }
    else if(ch == Channel::CH3)
    {
        reg::modifyBits(timer->CCMR2, 0x00FFU, TIM_CCMR2_CC3S_0 | (pscVal << TIM_CCMR2_IC3PSC_Pos));
        if(edge == CaptureEdge::Rising)
            reg::clearBits(timer->CCER, TIM_CCER_CC3P | TIM_CCER_CC3NP);
        else if(edge == CaptureEdge::Falling)
            reg::modifyBits(timer->CCER, TIM_CCER_CC3P | TIM_CCER_CC3NP, TIM_CCER_CC3P);
        else
            reg::setBits(timer->CCER, TIM_CCER_CC3P | TIM_CCER_CC3NP);
    }
    else if(ch == Channel::CH4)
    {
        reg::modifyBits(timer->CCMR2, 0xFF00U, TIM_CCMR2_CC4S_0 | (pscVal << TIM_CCMR2_IC4PSC_Pos));
        if(edge == CaptureEdge::Rising)
            reg::clearBits(timer->CCER, TIM_CCER_CC4P | TIM_CCER_CC4NP);
        else if(edge == CaptureEdge::Falling)
            reg::modifyBits(timer->CCER, TIM_CCER_CC4P | TIM_CCER_CC4NP, TIM_CCER_CC4P);
        else
            reg::setBits(timer->CCER, TIM_CCER_CC4P | TIM_CCER_CC4NP);
    }
}

} // namespace timer::helper

