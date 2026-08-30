/**
 * @file adc_helper.hpp
 * @brief Low-level register manipulation helper functions for ADC peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/adc/core/adc_types.hpp"

namespace adc::helper
{

inline void enable(ADC_TypeDef* adc) noexcept
{
    reg::setBits(adc->CR2, ADC_CR2_ADON);
}

inline void disable(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->CR2, ADC_CR2_ADON);
}

inline bool isEnabled(const ADC_TypeDef* adc) noexcept
{
    return reg::isAnyBitSet(adc->CR2, ADC_CR2_ADON);
}

inline void startConversion(ADC_TypeDef* adc) noexcept
{
    reg::setBits(adc->CR2, ADC_CR2_SWSTART);
}

inline bool isConversionComplete(const ADC_TypeDef* adc) noexcept
{
    return reg::isAnyBitSet(adc->SR, ADC_SR_EOC);
}

inline void clearConversionFlag(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->SR, ADC_SR_EOC);
}

inline bool isWatchdogFired(const ADC_TypeDef* adc) noexcept
{
    return reg::isAnyBitSet(adc->SR, ADC_SR_AWD);
}

inline void clearWatchdogFlag(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->SR, ADC_SR_AWD);
}

inline bool isOverrun(const ADC_TypeDef* adc) noexcept
{
    return reg::isAnyBitSet(adc->SR, ADC_SR_OVR);
}

inline void clearOverrunFlag(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->SR, ADC_SR_OVR);
}

inline void setResolution(ADC_TypeDef* adc, Resolution res) noexcept
{
    reg::modifyBits(adc->CR1, ADC_CR1_RES, static_cast<std::uint32_t>(res) << ADC_CR1_RES_Pos);
}

inline void setDataAlignment(ADC_TypeDef* adc, Alignment align) noexcept
{
    if(align == Alignment::Left)
        reg::setBits(adc->CR2, ADC_CR2_ALIGN);
    else
        reg::clearBits(adc->CR2, ADC_CR2_ALIGN);
}

inline void setContinuousMode(ADC_TypeDef* adc, bool continuous) noexcept
{
    if(continuous)
        reg::setBits(adc->CR2, ADC_CR2_CONT);
    else
        reg::clearBits(adc->CR2, ADC_CR2_CONT);
}

inline void setScanMode(ADC_TypeDef* adc, bool scan) noexcept
{
    if(scan)
        reg::setBits(adc->CR1, ADC_CR1_SCAN);
    else
        reg::clearBits(adc->CR1, ADC_CR1_SCAN);
}

inline void setChannelSamplingTime(ADC_TypeDef* adc, Channel ch, SamplingTime smp) noexcept
{
    const auto chNum = static_cast<std::uint8_t>(ch);
    const auto smpVal = static_cast<std::uint32_t>(smp);

    if(chNum < 10)
    {
        const std::uint8_t pos = chNum * 3U;
        reg::modifyBits(adc->SMPR2, 0x7U << pos, smpVal << pos);
    }
    else if(chNum <= 18)
    {
        const std::uint8_t pos = (chNum - 10U) * 3U;
        reg::modifyBits(adc->SMPR1, 0x7U << pos, smpVal << pos);
    }
}

inline void setSequenceLength(ADC_TypeDef* adc, std::uint8_t length) noexcept
{
    if(length == 0) length = 1;
    if(length > 16) length = 16;
    reg::modifyBits(adc->SQR1, ADC_SQR1_L, static_cast<std::uint32_t>(length - 1U) << ADC_SQR1_L_Pos);
}

inline void setSequenceChannel(ADC_TypeDef* adc, std::uint8_t rank, Channel ch) noexcept
{
    const auto chNum = static_cast<std::uint32_t>(ch);

    if(rank >= 1 && rank <= 6)
    {
        const std::uint8_t pos = (rank - 1U) * 5U;
        reg::modifyBits(adc->SQR3, 0x1FU << pos, chNum << pos);
    }
    else if(rank >= 7 && rank <= 12)
    {
        const std::uint8_t pos = (rank - 7U) * 5U;
        reg::modifyBits(adc->SQR2, 0x1FU << pos, chNum << pos);
    }
    else if(rank >= 13 && rank <= 16)
    {
        const std::uint8_t pos = (rank - 13U) * 5U;
        reg::modifyBits(adc->SQR1, 0x1FU << pos, chNum << pos);
    }
}

inline std::uint16_t readData(const ADC_TypeDef* adc) noexcept
{
    return static_cast<std::uint16_t>(reg::read(adc->DR));
}

inline void setWatchdogThresholds(ADC_TypeDef* adc, std::uint16_t low, std::uint16_t high) noexcept
{
    reg::write(adc->LTR, low & 0x0FFFU);
    reg::write(adc->HTR, high & 0x0FFFU);
}

inline void enableWatchdog(ADC_TypeDef* adc, Channel ch) noexcept
{
    reg::modifyBits(adc->CR1, ADC_CR1_AWDCH, static_cast<std::uint32_t>(ch) << ADC_CR1_AWDCH_Pos);
    reg::setBits(adc->CR1, ADC_CR1_AWDEN | ADC_CR1_AWDSGL);
}

inline void disableWatchdog(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->CR1, ADC_CR1_AWDEN | ADC_CR1_AWDSGL);
}

inline void enableEocInterrupt(ADC_TypeDef* adc) noexcept
{
    reg::setBits(adc->CR1, ADC_CR1_EOCIE);
}

inline void disableEocInterrupt(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->CR1, ADC_CR1_EOCIE);
}

inline void enableWatchdogInterrupt(ADC_TypeDef* adc) noexcept
{
    reg::setBits(adc->CR1, ADC_CR1_AWDIE);
}

inline void disableWatchdogInterrupt(ADC_TypeDef* adc) noexcept
{
    reg::clearBits(adc->CR1, ADC_CR1_AWDIE);
}

// Common CCR Control
inline void setPrescaler(ADC_Common_TypeDef* common, Prescaler psc) noexcept
{
    reg::modifyBits(common->CCR, ADC_CCR_ADCPRE, static_cast<std::uint32_t>(psc) << ADC_CCR_ADCPRE_Pos);
}

inline void enableTemperatureAndVref(ADC_Common_TypeDef* common) noexcept
{
    reg::setBits(common->CCR, ADC_CCR_TSVREFE);
}

inline void disableTemperatureAndVref(ADC_Common_TypeDef* common) noexcept
{
    reg::clearBits(common->CCR, ADC_CCR_TSVREFE);
}

inline void enableVbat(ADC_Common_TypeDef* common) noexcept
{
    reg::setBits(common->CCR, ADC_CCR_VBATE);
}

inline void disableVbat(ADC_Common_TypeDef* common) noexcept
{
    reg::clearBits(common->CCR, ADC_CCR_VBATE);
}

} // namespace adc::helper

