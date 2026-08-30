/**
 * @file i2c_helper.hpp
 * @brief Low-level register manipulation helper functions for I2C peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/i2c/core/i2c_types.hpp"

namespace i2c::helper
{

inline void enable(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR1, I2C_CR1_PE);
}

inline void disable(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->CR1, I2C_CR1_PE);
}

inline bool isEnabled(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->CR1, I2C_CR1_PE);
}

inline void setSoftwareReset(I2C_TypeDef* i2c, bool reset) noexcept
{
    if(reset)
        reg::setBits(i2c->CR1, I2C_CR1_SWRST);
    else
        reg::clearBits(i2c->CR1, I2C_CR1_SWRST);
}

inline void generateStart(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR1, I2C_CR1_START);
}

inline void generateStop(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR1, I2C_CR1_STOP);
}

inline void enableAck(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR1, I2C_CR1_ACK);
}

inline void disableAck(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->CR1, I2C_CR1_ACK);
}

inline void enablePos(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR1, I2C_CR1_POS);
}

inline void disablePos(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->CR1, I2C_CR1_POS);
}

inline void setClockFrequency(I2C_TypeDef* i2c, std::uint8_t freqMhz) noexcept
{
    reg::modifyBits(i2c->CR2, I2C_CR2_FREQ, freqMhz & 0x3FU);
}

inline void setClockControl(I2C_TypeDef* i2c, std::uint32_t ccrVal) noexcept
{
    reg::write(i2c->CCR, ccrVal);
}

inline void setRiseTime(I2C_TypeDef* i2c, std::uint32_t triseVal) noexcept
{
    reg::write(i2c->TRISE, triseVal);
}

inline void setOwnAddress(I2C_TypeDef* i2c, std::uint8_t address) noexcept
{
    reg::write(i2c->OAR1, (static_cast<std::uint32_t>(address) << 1U) | 0x4000U);
}

inline bool isStartBit(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_SB);
}

inline bool isAddressSent(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_ADDR);
}

inline bool isTxEmpty(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_TXE);
}

inline bool isRxNotEmpty(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_RXNE);
}

inline bool isByteTransferFinished(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_BTF);
}

inline bool isAcknowledgeFailure(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_AF);
}

inline bool isBusError(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_BERR);
}

inline bool isArbitrationLost(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_ARLO);
}

inline bool isOverrun(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_OVR);
}

inline bool isTimeout(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR1, I2C_SR1_TIMEOUT);
}

inline bool isBusBusy(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR2, I2C_SR2_BUSY);
}

inline bool isMaster(const I2C_TypeDef* i2c) noexcept
{
    return reg::isAnyBitSet(i2c->SR2, I2C_SR2_MSL);
}

inline void clearAddressFlag(I2C_TypeDef* i2c) noexcept
{
    volatile std::uint32_t dummy = reg::read(i2c->SR1);
    dummy = reg::read(i2c->SR2);
    (void)dummy;
}

inline void clearAcknowledgeFailure(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->SR1, I2C_SR1_AF);
}

inline void clearBusError(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->SR1, I2C_SR1_BERR);
}

inline void clearArbitrationLost(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->SR1, I2C_SR1_ARLO);
}

inline void writeData(I2C_TypeDef* i2c, std::uint8_t data) noexcept
{
    reg::write(i2c->DR, data);
}

inline std::uint8_t readData(const I2C_TypeDef* i2c) noexcept
{
    return static_cast<std::uint8_t>(reg::read(i2c->DR));
}

inline void enableEventInterrupt(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR2, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
}

inline void disableEventInterrupt(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->CR2, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
}

inline void enableErrorInterrupt(I2C_TypeDef* i2c) noexcept
{
    reg::setBits(i2c->CR2, I2C_CR2_ITERREN);
}

inline void disableErrorInterrupt(I2C_TypeDef* i2c) noexcept
{
    reg::clearBits(i2c->CR2, I2C_CR2_ITERREN);
}

} // namespace i2c::helper

