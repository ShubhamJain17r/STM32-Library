/**
 * @file spi_interrupt.cpp
 * @brief Implementation of SPI interrupt dispatcher
 */

#include "stm32/common/registers/registers.hpp"
#include "stm32/spi/interrupt/spi_interrupt.hpp"

namespace spi::interrupt
{

static constexpr std::size_t index(Instance I) noexcept
{
    return static_cast<std::size_t>(I);
}

static SPI_TypeDef* getPeripheral(Instance I) noexcept
{
    switch(I)
    {
        case Instance::spi1: return SPI1;
        case Instance::spi2: return SPI2;
        case Instance::spi3: return SPI3;
        case Instance::spi4: return SPI4;
    }
    return nullptr;
}

void SpiEvent::setDeveloperCallback(Instance I, Event ev, stm32::Callback cb) noexcept
{
    switch(ev)
    {
        case Event::TxEmpty:
            developerCallbacks_[index(I)].txEmpty = cb;
            break;
        case Event::RxNotEmpty:
            developerCallbacks_[index(I)].rxNotEmpty = cb;
            break;
        case Event::Error:
            developerCallbacks_[index(I)].error = cb;
            break;
        case Event::TransferComplete:
            developerCallbacks_[index(I)].transferComplete = cb;
            break;
    }
}

void SpiEvent::setUserCallback(Instance I, Event ev, stm32::Callback cb) noexcept
{
    switch(ev)
    {
        case Event::TxEmpty:
            userCallbacks_[index(I)].txEmpty = cb;
            break;
        case Event::RxNotEmpty:
            userCallbacks_[index(I)].rxNotEmpty = cb;
            break;
        case Event::Error:
            userCallbacks_[index(I)].error = cb;
            break;
        case Event::TransferComplete:
            userCallbacks_[index(I)].transferComplete = cb;
            break;
    }
}

void SpiEvent::handleEvent(Instance I) noexcept
{
    auto* spi = getPeripheral(I);
    if(!spi) return;

    const std::uint32_t sr  = reg::read(spi->SR);
    const std::uint32_t cr2 = reg::read(spi->CR2);

    const auto& dev  = developerCallbacks_[index(I)];
    const auto& user = userCallbacks_[index(I)];

    // 1. RXNE (Receive buffer not empty)
    if((sr & SPI_SR_RXNE) && (cr2 & SPI_CR2_RXNEIE))
    {
        if(dev.rxNotEmpty)  dev.rxNotEmpty();
        if(user.rxNotEmpty) user.rxNotEmpty();
    }

    // 2. TXE (Transmit buffer empty)
    if((sr & SPI_SR_TXE) && (cr2 & SPI_CR2_TXEIE))
    {
        if(dev.txEmpty)  dev.txEmpty();
        if(user.txEmpty) user.txEmpty();
    }

    // 3. Error Flags (OVR, MODF, CRCERR, FRE)
    if((sr & (SPI_SR_OVR | SPI_SR_MODF | SPI_SR_CRCERR | SPI_SR_FRE)) && (cr2 & SPI_CR2_ERRIE))
    {
        if(dev.error)  dev.error();
        if(user.error) user.error();
    }
}

} // namespace spi::interrupt

