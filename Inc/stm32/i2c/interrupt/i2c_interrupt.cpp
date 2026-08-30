/**
 * @file i2c_interrupt.cpp
 * @brief Implementation of I2C interrupt event and error dispatcher
 */

#include "stm32/common/registers/registers.hpp"
#include "stm32/i2c/core/i2c_helper.hpp"
#include "stm32/i2c/interrupt/i2c_interrupt.hpp"

namespace i2c::interrupt
{

static constexpr std::size_t index(Instance I) noexcept
{
    return static_cast<std::size_t>(I);
}

static I2C_TypeDef* getPeripheral(Instance I) noexcept
{
    switch(I)
    {
        case Instance::i2c1: return I2C1;
        case Instance::i2c2: return I2C2;
        case Instance::i2c3: return I2C3;
    }
    return nullptr;
}

void I2cEvent::setDeveloperCallback(Instance I, Event ev, stm32::Callback cb) noexcept
{
    switch(ev)
    {
        case Event::Start:
            developerCallbacks_[index(I)].start = cb;
            break;
        case Event::Address:
            developerCallbacks_[index(I)].address = cb;
            break;
        case Event::ByteTransferFinished:
            developerCallbacks_[index(I)].byteTransferFinished = cb;
            break;
        case Event::RxNotEmpty:
            developerCallbacks_[index(I)].rxNotEmpty = cb;
            break;
        case Event::TxEmpty:
            developerCallbacks_[index(I)].txEmpty = cb;
            break;
        case Event::Stop:
            developerCallbacks_[index(I)].stop = cb;
            break;
        case Event::Error:
            developerCallbacks_[index(I)].error = cb;
            break;
    }
}

void I2cEvent::setUserCallback(Instance I, Event ev, stm32::Callback cb) noexcept
{
    switch(ev)
    {
        case Event::Start:
            userCallbacks_[index(I)].start = cb;
            break;
        case Event::Address:
            userCallbacks_[index(I)].address = cb;
            break;
        case Event::ByteTransferFinished:
            userCallbacks_[index(I)].byteTransferFinished = cb;
            break;
        case Event::RxNotEmpty:
            userCallbacks_[index(I)].rxNotEmpty = cb;
            break;
        case Event::TxEmpty:
            userCallbacks_[index(I)].txEmpty = cb;
            break;
        case Event::Stop:
            userCallbacks_[index(I)].stop = cb;
            break;
        case Event::Error:
            userCallbacks_[index(I)].error = cb;
            break;
    }
}

void I2cEvent::handleEvent(Instance I) noexcept
{
    auto* i2c = getPeripheral(I);
    if(!i2c) return;

    const std::uint32_t sr1 = reg::read(i2c->SR1);

    const auto& dev  = developerCallbacks_[index(I)];
    const auto& user = userCallbacks_[index(I)];

    // 1. SB (Start Bit Generated)
    if(sr1 & I2C_SR1_SB)
    {
        if(dev.start)  dev.start();
        if(user.start) user.start();
    }

    // 2. ADDR (Address Sent / Matched)
    if(sr1 & I2C_SR1_ADDR)
    {
        if(dev.address)  dev.address();
        if(user.address) user.address();
    }

    // 3. BTF (Byte Transfer Finished)
    if(sr1 & I2C_SR1_BTF)
    {
        if(dev.byteTransferFinished)  dev.byteTransferFinished();
        if(user.byteTransferFinished) user.byteTransferFinished();
    }

    // 4. RXNE (Data Register Not Empty)
    if(sr1 & I2C_SR1_RXNE)
    {
        if(dev.rxNotEmpty)  dev.rxNotEmpty();
        if(user.rxNotEmpty) user.rxNotEmpty();
    }

    // 5. TXE (Data Register Empty)
    if(sr1 & I2C_SR1_TXE)
    {
        if(dev.txEmpty)  dev.txEmpty();
        if(user.txEmpty) user.txEmpty();
    }

    // 6. STOPF (Stop Detection in Slave Mode)
    if(sr1 & I2C_SR1_STOPF)
    {
        // Clear STOPF by read to SR1 followed by write to CR1
        volatile std::uint32_t dummy = reg::read(i2c->SR1);
        reg::write(i2c->CR1, reg::read(i2c->CR1));
        (void)dummy;

        if(dev.stop)  dev.stop();
        if(user.stop) user.stop();
    }
}

void I2cEvent::handleError(Instance I) noexcept
{
    auto* i2c = getPeripheral(I);
    if(!i2c) return;

    const std::uint32_t sr1 = reg::read(i2c->SR1);

    // Clear error flags
    if(sr1 & I2C_SR1_AF)      helper::clearAcknowledgeFailure(i2c);
    if(sr1 & I2C_SR1_BERR)    helper::clearBusError(i2c);
    if(sr1 & I2C_SR1_ARLO)    helper::clearArbitrationLost(i2c);
    if(sr1 & I2C_SR1_OVR)     reg::clearBits(i2c->SR1, I2C_SR1_OVR);
    if(sr1 & I2C_SR1_TIMEOUT) reg::clearBits(i2c->SR1, I2C_SR1_TIMEOUT);

    const auto& dev  = developerCallbacks_[index(I)];
    const auto& user = userCallbacks_[index(I)];

    if(dev.error)  dev.error();
    if(user.error) user.error();
}

} // namespace i2c::interrupt

