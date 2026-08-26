/**
 * @file spi_helper.hpp
 * @brief Low-level register manipulation helper functions for SPI peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/registers/registers.hpp"
#include "stm32/spi/core/spi_types.hpp"

namespace spi::helper
{

inline void enable(SPI_TypeDef* spi) noexcept
{
    reg::setBits(spi->CR1, SPI_CR1_SPE);
}

inline void disable(SPI_TypeDef* spi) noexcept
{
    reg::clearBits(spi->CR1, SPI_CR1_SPE);
}

inline bool isEnabled(const SPI_TypeDef* spi) noexcept
{
    return reg::isAnyBitSet(spi->CR1, SPI_CR1_SPE);
}

inline bool isTxEmpty(const SPI_TypeDef* spi) noexcept
{
    return reg::isAnyBitSet(spi->SR, SPI_SR_TXE);
}

inline bool isRxNotEmpty(const SPI_TypeDef* spi) noexcept
{
    return reg::isAnyBitSet(spi->SR, SPI_SR_RXNE);
}

inline bool isBusy(const SPI_TypeDef* spi) noexcept
{
    return reg::isAnyBitSet(spi->SR, SPI_SR_BSY);
}

inline void write8(SPI_TypeDef* spi, std::uint8_t data) noexcept
{
    reg::write(*reinterpret_cast<volatile std::uint8_t*>(&spi->DR), data);
}

inline std::uint8_t read8(const SPI_TypeDef* spi) noexcept
{
    return reg::read(*reinterpret_cast<const volatile std::uint8_t*>(&spi->DR));
}

inline void write16(SPI_TypeDef* spi, std::uint16_t data) noexcept
{
    reg::write(spi->DR, data);
}

inline std::uint16_t read16(const SPI_TypeDef* spi) noexcept
{
    return static_cast<std::uint16_t>(reg::read(spi->DR));
}

inline void setMode(SPI_TypeDef* spi, Mode mode) noexcept
{
    if(mode == Mode::Master)
        reg::setBits(spi->CR1, SPI_CR1_MSTR);
    else
        reg::clearBits(spi->CR1, SPI_CR1_MSTR);
}

inline void setPrescaler(SPI_TypeDef* spi, Prescaler psc) noexcept
{
    reg::modifyBits(spi->CR1, SPI_CR1_BR, static_cast<std::uint32_t>(psc) << SPI_CR1_BR_Pos);
}

inline void setClockPolarity(SPI_TypeDef* spi, ClockPolarity cpol) noexcept
{
    if(cpol == ClockPolarity::High)
        reg::setBits(spi->CR1, SPI_CR1_CPOL);
    else
        reg::clearBits(spi->CR1, SPI_CR1_CPOL);
}

inline void setClockPhase(SPI_TypeDef* spi, ClockPhase cpha) noexcept
{
    if(cpha == ClockPhase::Edge2)
        reg::setBits(spi->CR1, SPI_CR1_CPHA);
    else
        reg::clearBits(spi->CR1, SPI_CR1_CPHA);
}

inline void setSpiMode(SPI_TypeDef* spi, SpiMode mode) noexcept
{
    switch(mode)
    {
        case SpiMode::Mode0: // CPOL = 0, CPHA = 0
            reg::clearBits(spi->CR1, SPI_CR1_CPOL | SPI_CR1_CPHA);
            break;
        case SpiMode::Mode1: // CPOL = 0, CPHA = 1
            reg::modifyBits(spi->CR1, SPI_CR1_CPOL, SPI_CR1_CPHA);
            break;
        case SpiMode::Mode2: // CPOL = 1, CPHA = 0
            reg::modifyBits(spi->CR1, SPI_CR1_CPHA, SPI_CR1_CPOL);
            break;
        case SpiMode::Mode3: // CPOL = 1, CPHA = 1
            reg::setBits(spi->CR1, SPI_CR1_CPOL | SPI_CR1_CPHA);
            break;
    }
}

inline void setDataSize(SPI_TypeDef* spi, DataSize size) noexcept
{
    if(size == DataSize::Bits16)
        reg::setBits(spi->CR1, SPI_CR1_DFF);
    else
        reg::clearBits(spi->CR1, SPI_CR1_DFF);
}

inline void setBitOrder(SPI_TypeDef* spi, BitOrder order) noexcept
{
    if(order == BitOrder::LsbFirst)
        reg::setBits(spi->CR1, SPI_CR1_LSBFIRST);
    else
        reg::clearBits(spi->CR1, SPI_CR1_LSBFIRST);
}

inline void setSoftwareSlaveManagement(SPI_TypeDef* spi, bool enable) noexcept
{
    if(enable)
        reg::setBits(spi->CR1, SPI_CR1_SSM);
    else
        reg::clearBits(spi->CR1, SPI_CR1_SSM);
}

inline void setInternalSlaveSelect(SPI_TypeDef* spi, bool high) noexcept
{
    if(high)
        reg::setBits(spi->CR1, SPI_CR1_SSI);
    else
        reg::clearBits(spi->CR1, SPI_CR1_SSI);
}

inline void setSSOutput(SPI_TypeDef* spi, bool enable) noexcept
{
    if(enable)
        reg::setBits(spi->CR2, SPI_CR2_SSOE);
    else
        reg::clearBits(spi->CR2, SPI_CR2_SSOE);
}

inline void setDirection(SPI_TypeDef* spi, Direction dir) noexcept
{
    switch(dir)
    {
        case Direction::FullDuplex:
            reg::clearBits(spi->CR1, SPI_CR1_BIDIMODE | SPI_CR1_RXONLY);
            break;
        case Direction::RxOnly:
            reg::modifyBits(spi->CR1, SPI_CR1_BIDIMODE, SPI_CR1_RXONLY);
            break;
        case Direction::TxOnly:
            reg::clearBits(spi->CR1, SPI_CR1_BIDIMODE | SPI_CR1_RXONLY);
            break;
        case Direction::HalfDuplex:
            reg::setBits(spi->CR1, SPI_CR1_BIDIMODE);
            break;
    }
}

inline void enableTxInterrupt(SPI_TypeDef* spi) noexcept
{
    reg::setBits(spi->CR2, SPI_CR2_TXEIE);
}

inline void disableTxInterrupt(SPI_TypeDef* spi) noexcept
{
    reg::clearBits(spi->CR2, SPI_CR2_TXEIE);
}

inline void enableRxInterrupt(SPI_TypeDef* spi) noexcept
{
    reg::setBits(spi->CR2, SPI_CR2_RXNEIE);
}

inline void disableRxInterrupt(SPI_TypeDef* spi) noexcept
{
    reg::clearBits(spi->CR2, SPI_CR2_RXNEIE);
}

inline void enableErrorInterrupt(SPI_TypeDef* spi) noexcept
{
    reg::setBits(spi->CR2, SPI_CR2_ERRIE);
}

inline void disableErrorInterrupt(SPI_TypeDef* spi) noexcept
{
    reg::clearBits(spi->CR2, SPI_CR2_ERRIE);
}

} // namespace spi::helper

