/**
 * @file i2c_master.hpp
 * @brief High-level I2C master driver with timeout protection, bus recovery, and scanner
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <cstddef>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/common/systick/systick.hpp"

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"
#include "stm32/gpio/af/af_helper.hpp"
#include "stm32/gpio/digital_output/digital_output.hpp"

#include "stm32/i2c/core/i2c_types.hpp"
#include "stm32/i2c/core/i2c_traits.hpp"
#include "stm32/i2c/core/i2c_af_traits.hpp"
#include "stm32/i2c/core/i2c_config.hpp"
#include "stm32/i2c/core/i2c_helper.hpp"
#include "stm32/i2c/interrupt/i2c_interrupt.hpp"

namespace i2c
{

template<Instance I>
class I2cMaster
{
public:
    I2cMaster() = delete;
    ~I2cMaster();

    I2cMaster(const I2cMaster&) = delete;
    I2cMaster& operator=(const I2cMaster&) = delete;

    I2cMaster(I2cMaster&&) = delete;
    I2cMaster& operator=(I2cMaster&&) = delete;

    explicit I2cMaster(std::uint32_t speedHz = 100'000);
    explicit I2cMaster(gpio::Pin scl, gpio::Pin sda, std::uint32_t speedHz = 100'000);
    explicit I2cMaster(const I2cConfig<I>& config);

    // Master Transmit & Receive
    bool write(std::uint8_t devAddr, const std::uint8_t* data, std::size_t length, bool sendStop = true, std::uint32_t timeoutMs = 50) noexcept;
    bool write(std::uint8_t devAddr, std::uint8_t byte, bool sendStop = true, std::uint32_t timeoutMs = 50) noexcept;

    bool read(std::uint8_t devAddr, std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs = 50) noexcept;
    bool read(std::uint8_t devAddr, std::uint8_t& byte, std::uint32_t timeoutMs = 50) noexcept;

    // Register-Level Sensor Read / Write Helpers
    bool writeRegister(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t value, std::uint32_t timeoutMs = 50) noexcept;
    bool writeRegister(std::uint8_t devAddr, std::uint8_t regAddr, const std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs = 50) noexcept;
    bool writeRegister16(std::uint8_t devAddr, std::uint16_t regAddr, const std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs = 50) noexcept;

    bool readRegister(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t& value, std::uint32_t timeoutMs = 50) noexcept;
    bool readRegister(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs = 50) noexcept;
    bool readRegister16(std::uint8_t devAddr, std::uint16_t regAddr, std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs = 50) noexcept;

    // Bus Health & Utilities
    bool isDeviceReady(std::uint8_t address, std::uint32_t trials = 2, std::uint32_t timeoutMs = 25) noexcept;
    std::size_t scanBus(std::uint8_t* foundAddresses, std::size_t maxCount) noexcept;
    bool recoverBus() noexcept;

    void enable() noexcept;
    void disable() noexcept;
    void reset() noexcept;
    void setSpeed(std::uint32_t speedHz, DutyCycle duty = DutyCycle::Duty2_1) noexcept;

    I2cStatus getLastError() const noexcept { return lastStatus_; }

private:
    I2cConfig<I> config_;
    I2cStatus    lastStatus_ = I2cStatus::Success;

    void initPins() noexcept;
    void configureTiming(std::uint32_t speedHz, DutyCycle duty) noexcept;
    bool startAddress(std::uint8_t devAddr, bool isRead, systick::Timeout& to) noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

template<Instance I>
void I2cMaster<I>::initPins() noexcept
{
    // Configure SCL Pin (Open-Drain, High-Speed, AF4)
    const auto sclAf = af::alternateFunction<I, af::Signal::SCL>(config_.scl);
    if(sclAf != af::AlternateFunction::INVALID)
    {
        config_.scl.enableClock();
        gpio::af::setAlternateFunction(config_.scl, sclAf);
        gpio::helper::setOutputType(config_.scl, gpio::OutputType::OPEN_DRAIN);
        gpio::helper::setOutputSpeed(config_.scl, gpio::OutputSpeed::HIGH);
        gpio::helper::setPull(config_.scl, config_.enableInternalPullup ? gpio::Pull::UP : gpio::Pull::NONE);
        gpio::helper::setMode(config_.scl, gpio::Mode::ALTERNATE);
    }

    // Configure SDA Pin (Open-Drain, High-Speed, AF4)
    const auto sdaAf = af::alternateFunction<I, af::Signal::SDA>(config_.sda);
    if(sdaAf != af::AlternateFunction::INVALID)
    {
        config_.sda.enableClock();
        gpio::af::setAlternateFunction(config_.sda, sdaAf);
        gpio::helper::setOutputType(config_.sda, gpio::OutputType::OPEN_DRAIN);
        gpio::helper::setOutputSpeed(config_.sda, gpio::OutputSpeed::HIGH);
        gpio::helper::setPull(config_.sda, config_.enableInternalPullup ? gpio::Pull::UP : gpio::Pull::NONE);
        gpio::helper::setMode(config_.sda, gpio::Mode::ALTERNATE);
    }
}

template<Instance I>
void I2cMaster<I>::configureTiming(std::uint32_t speedHz, DutyCycle duty) noexcept
{
    auto* i2c = Traits<I>::peripheral();

    if(speedHz == 0) speedHz = 100'000;

    const std::uint32_t pclk1 = rcc::getPCLK1();
    std::uint32_t pclk1Mhz = pclk1 / 1'000'000U;
    if(pclk1Mhz < 2U)  pclk1Mhz = 2U;
    if(pclk1Mhz > 45U) pclk1Mhz = 45U;

    // 1. Program peripheral input clock frequency (FREQ[5:0] in CR2)
    helper::setClockFrequency(i2c, static_cast<std::uint8_t>(pclk1Mhz));

    // 2. Program CCR and TRISE
    if(speedHz <= 100'000)
    {
        // Standard Mode (100 kHz)
        std::uint32_t ccr = pclk1 / (speedHz * 2U);
        if(ccr < 4U) ccr = 4U;
        helper::setClockControl(i2c, ccr & 0x0FFFU);

        // Maximum rise time in Sm = 1000 ns
        helper::setRiseTime(i2c, (pclk1Mhz + 1U) & 0x3FU);
    }
    else
    {
        // Fast Mode (400 kHz)
        std::uint32_t ccr = 0;
        if(duty == DutyCycle::Duty2_1)
        {
            ccr = pclk1 / (speedHz * 3U);
            if(ccr < 1U) ccr = 1U;
            ccr |= (1U << 15); // F/S = 1 (Fast mode), DUTY = 0
        }
        else // Duty16_9
        {
            ccr = pclk1 / (speedHz * 25U);
            if(ccr < 1U) ccr = 1U;
            ccr |= (1U << 15) | (1U << 14); // F/S = 1, DUTY = 1
        }
        helper::setClockControl(i2c, ccr);

        // Maximum rise time in Fm = 300 ns
        const std::uint32_t trise = ((pclk1Mhz * 300U) / 1000U) + 1U;
        helper::setRiseTime(i2c, trise & 0x3FU);
    }
}

template<Instance I>
I2cMaster<I>::I2cMaster(std::uint32_t speedHz)
    : I2cMaster(I2cConfig<I>{speedHz})
{}

template<Instance I>
I2cMaster<I>::I2cMaster(gpio::Pin scl, gpio::Pin sda, std::uint32_t speedHz)
    : I2cMaster(I2cConfig<I>{scl, sda, speedHz})
{}

template<Instance I>
I2cMaster<I>::I2cMaster(const I2cConfig<I>& config)
    : config_(config)
{
    // 1. Enable peripheral clock
    Traits<I>::enableClock();

    auto* i2c = Traits<I>::peripheral();

    // 2. Perform software reset to clear any sticky state
    helper::setSoftwareReset(i2c, true);
    for(volatile int k = 0; k < 1000; ++k);
    helper::setSoftwareReset(i2c, false);

    // 3. Configure GPIO Alternate Function Open-Drain pins
    initPins();

    // 4. Configure Clock Tree Timing (FREQ, CCR, TRISE)
    configureTiming(config_.speedHz, config_.dutyCycle);

    // 5. Configure Own Address (OAR1)
    helper::setOwnAddress(i2c, config_.ownAddress);

    // 6. Enable I2C peripheral and ACK
    helper::enable(i2c);
    helper::enableAck(i2c);
}

template<Instance I>
I2cMaster<I>::~I2cMaster()
{
    disable();
}

template<Instance I>
void I2cMaster<I>::enable() noexcept
{
    helper::enable(Traits<I>::peripheral());
    helper::enableAck(Traits<I>::peripheral());
}

template<Instance I>
void I2cMaster<I>::disable() noexcept
{
    helper::disable(Traits<I>::peripheral());
}

template<Instance I>
void I2cMaster<I>::reset() noexcept
{
    auto* i2c = Traits<I>::peripheral();
    helper::setSoftwareReset(i2c, true);
    for(volatile int k = 0; k < 1000; ++k);
    helper::setSoftwareReset(i2c, false);
    configureTiming(config_.speedHz, config_.dutyCycle);
    helper::enable(i2c);
    helper::enableAck(i2c);
}

template<Instance I>
void I2cMaster<I>::setSpeed(std::uint32_t speedHz, DutyCycle duty) noexcept
{
    config_.speedHz = speedHz;
    config_.dutyCycle = duty;

    disable();
    configureTiming(speedHz, duty);
    enable();
}

template<Instance I>
bool I2cMaster<I>::recoverBus() noexcept
{
    // Configure SCL and SDA as standard GPIO outputs to clock out stuck slaves
    config_.scl.enableClock();
    config_.sda.enableClock();

    gpio::helper::setMode(config_.scl, gpio::Mode::OUTPUT);
    gpio::helper::setOutputType(config_.scl, gpio::OutputType::OPEN_DRAIN);
    gpio::helper::setPull(config_.scl, gpio::Pull::UP);

    gpio::helper::setMode(config_.sda, gpio::Mode::INPUT);
    gpio::helper::setPull(config_.sda, gpio::Pull::UP);

    // Clock SCL up to 9 times while SDA is held low
    for(int i = 0; i < 9; ++i)
    {
        reg::write(config_.scl.port->BSRR, config_.scl.mask() << 16U); // SCL LOW
        systick::delayUs(5);
        reg::write(config_.scl.port->BSRR, config_.scl.mask());        // SCL HIGH
        systick::delayUs(5);

        if(reg::isAnyBitSet(config_.sda.port->IDR, config_.sda.mask()))
        {
            break; // SDA has been released by slave
        }
    }

    // Re-initialize hardware peripheral and AF pins
    initPins();
    reset();
    return true;
}

template<Instance I>
bool I2cMaster<I>::startAddress(std::uint8_t devAddr, bool isRead, systick::Timeout& to) noexcept
{
    auto* i2c = Traits<I>::peripheral();

    // 1. Wait until bus is not busy
    while(helper::isBusBusy(i2c))
    {
        if(to.expired())
        {
            lastStatus_ = I2cStatus::Busy;
            return false;
        }
    }

    // 2. Generate START condition
    helper::generateStart(i2c);

    // 3. Wait for SB (Start Bit Generated)
    while(!helper::isStartBit(i2c))
    {
        if(to.expired())
        {
            lastStatus_ = I2cStatus::Timeout;
            return false;
        }
    }

    // 4. Send 7-bit device address + R/W bit
    const std::uint8_t addrByte = (devAddr << 1U) | (isRead ? 1U : 0U);
    helper::writeData(i2c, addrByte);

    // 5. Wait for ADDR flag or AF (NACK)
    while(!helper::isAddressSent(i2c))
    {
        if(helper::isAcknowledgeFailure(i2c))
        {
            helper::clearAcknowledgeFailure(i2c);
            helper::generateStop(i2c);
            lastStatus_ = I2cStatus::Nack;
            return false;
        }
        if(to.expired())
        {
            helper::generateStop(i2c);
            lastStatus_ = I2cStatus::Timeout;
            return false;
        }
    }

    lastStatus_ = I2cStatus::Success;
    return true;
}

template<Instance I>
bool I2cMaster<I>::write(std::uint8_t devAddr, const std::uint8_t* data, std::size_t length, bool sendStop, std::uint32_t timeoutMs) noexcept
{
    auto* i2c = Traits<I>::peripheral();
    systick::Timeout to(timeoutMs);

    if(!startAddress(devAddr, false, to))
    {
        return false;
    }

    // Clear ADDR flag
    helper::clearAddressFlag(i2c);

    // Transmit data bytes
    for(std::size_t i = 0; i < length; ++i)
    {
        while(!helper::isTxEmpty(i2c))
        {
            if(helper::isAcknowledgeFailure(i2c))
            {
                helper::clearAcknowledgeFailure(i2c);
                helper::generateStop(i2c);
                lastStatus_ = I2cStatus::Nack;
                return false;
            }
            if(to.expired())
            {
                helper::generateStop(i2c);
                lastStatus_ = I2cStatus::Timeout;
                return false;
            }
        }

        helper::writeData(i2c, data[i]);
    }

    // Wait for BTF (Byte Transfer Finished)
    while(!helper::isByteTransferFinished(i2c))
    {
        if(to.expired())
        {
            helper::generateStop(i2c);
            lastStatus_ = I2cStatus::Timeout;
            return false;
        }
    }

    if(sendStop)
    {
        helper::generateStop(i2c);
    }

    lastStatus_ = I2cStatus::Success;
    return true;
}

template<Instance I>
bool I2cMaster<I>::write(std::uint8_t devAddr, std::uint8_t byte, bool sendStop, std::uint32_t timeoutMs) noexcept
{
    return write(devAddr, &byte, 1, sendStop, timeoutMs);
}

template<Instance I>
bool I2cMaster<I>::read(std::uint8_t devAddr, std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs) noexcept
{
    if(length == 0 || !data) return false;

    auto* i2c = Traits<I>::peripheral();
    systick::Timeout to(timeoutMs);

    if(length == 1)
    {
        // 1-Byte Reception Sequence (ST Errata Compliant)
        helper::disableAck(i2c);

        if(!startAddress(devAddr, true, to))
        {
            helper::enableAck(i2c);
            return false;
        }

        helper::clearAddressFlag(i2c);
        helper::generateStop(i2c);

        while(!helper::isRxNotEmpty(i2c))
        {
            if(to.expired())
            {
                lastStatus_ = I2cStatus::Timeout;
                helper::enableAck(i2c);
                return false;
            }
        }

        data[0] = helper::readData(i2c);
        helper::enableAck(i2c);
    }
    else if(length == 2)
    {
        // 2-Byte Reception Sequence
        helper::enablePos(i2c);
        helper::disableAck(i2c);

        if(!startAddress(devAddr, true, to))
        {
            helper::disablePos(i2c);
            helper::enableAck(i2c);
            return false;
        }

        helper::clearAddressFlag(i2c);

        while(!helper::isByteTransferFinished(i2c))
        {
            if(to.expired())
            {
                helper::generateStop(i2c);
                helper::disablePos(i2c);
                helper::enableAck(i2c);
                lastStatus_ = I2cStatus::Timeout;
                return false;
            }
        }

        helper::generateStop(i2c);
        data[0] = helper::readData(i2c);
        data[1] = helper::readData(i2c);

        helper::disablePos(i2c);
        helper::enableAck(i2c);
    }
    else // length > 2
    {
        helper::enableAck(i2c);

        if(!startAddress(devAddr, true, to))
        {
            return false;
        }

        helper::clearAddressFlag(i2c);

        for(std::size_t i = 0; i < length - 3; ++i)
        {
            while(!helper::isRxNotEmpty(i2c))
            {
                if(to.expired())
                {
                    helper::generateStop(i2c);
                    lastStatus_ = I2cStatus::Timeout;
                    return false;
                }
            }
            data[i] = helper::readData(i2c);
        }

        // Byte N-2
        while(!helper::isByteTransferFinished(i2c))
        {
            if(to.expired()) { helper::generateStop(i2c); return false; }
        }
        helper::disableAck(i2c);
        data[length - 3] = helper::readData(i2c);

        // Byte N-1 and N
        while(!helper::isByteTransferFinished(i2c))
        {
            if(to.expired()) { helper::generateStop(i2c); return false; }
        }
        helper::generateStop(i2c);
        data[length - 2] = helper::readData(i2c);
        data[length - 1] = helper::readData(i2c);

        helper::enableAck(i2c);
    }

    lastStatus_ = I2cStatus::Success;
    return true;
}

template<Instance I>
bool I2cMaster<I>::read(std::uint8_t devAddr, std::uint8_t& byte, std::uint32_t timeoutMs) noexcept
{
    return read(devAddr, &byte, 1, timeoutMs);
}

template<Instance I>
bool I2cMaster<I>::writeRegister(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t value, std::uint32_t timeoutMs) noexcept
{
    const std::uint8_t payload[2] = {regAddr, value};
    return write(devAddr, payload, 2, true, timeoutMs);
}

template<Instance I>
bool I2cMaster<I>::writeRegister(std::uint8_t devAddr, std::uint8_t regAddr, const std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs) noexcept
{
    auto* i2c = Traits<I>::peripheral();
    systick::Timeout to(timeoutMs);

    if(!startAddress(devAddr, false, to)) return false;
    helper::clearAddressFlag(i2c);

    // Send register address
    while(!helper::isTxEmpty(i2c))
    {
        if(to.expired()) { helper::generateStop(i2c); return false; }
    }
    helper::writeData(i2c, regAddr);

    // Send data buffer
    for(std::size_t i = 0; i < length; ++i)
    {
        while(!helper::isTxEmpty(i2c))
        {
            if(to.expired()) { helper::generateStop(i2c); return false; }
        }
        helper::writeData(i2c, data[i]);
    }

    while(!helper::isByteTransferFinished(i2c))
    {
        if(to.expired()) { helper::generateStop(i2c); return false; }
    }

    helper::generateStop(i2c);
    return true;
}

template<Instance I>
bool I2cMaster<I>::writeRegister16(std::uint8_t devAddr, std::uint16_t regAddr, const std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs) noexcept
{
    auto* i2c = Traits<I>::peripheral();
    systick::Timeout to(timeoutMs);

    if(!startAddress(devAddr, false, to)) return false;
    helper::clearAddressFlag(i2c);

    // Send 16-bit register address MSB then LSB
    while(!helper::isTxEmpty(i2c)) { if(to.expired()) { helper::generateStop(i2c); return false; } }
    helper::writeData(i2c, static_cast<std::uint8_t>(regAddr >> 8U));

    while(!helper::isTxEmpty(i2c)) { if(to.expired()) { helper::generateStop(i2c); return false; } }
    helper::writeData(i2c, static_cast<std::uint8_t>(regAddr & 0xFFU));

    for(std::size_t i = 0; i < length; ++i)
    {
        while(!helper::isTxEmpty(i2c)) { if(to.expired()) { helper::generateStop(i2c); return false; } }
        helper::writeData(i2c, data[i]);
    }

    while(!helper::isByteTransferFinished(i2c)) { if(to.expired()) { helper::generateStop(i2c); return false; } }
    helper::generateStop(i2c);
    return true;
}

template<Instance I>
bool I2cMaster<I>::readRegister(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t& value, std::uint32_t timeoutMs) noexcept
{
    return readRegister(devAddr, regAddr, &value, 1, timeoutMs);
}

template<Instance I>
bool I2cMaster<I>::readRegister(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs) noexcept
{
    // Write register address with repeated START (sendStop = false)
    if(!write(devAddr, &regAddr, 1, false, timeoutMs))
    {
        return false;
    }
    // Read response
    return read(devAddr, data, length, timeoutMs);
}

template<Instance I>
bool I2cMaster<I>::readRegister16(std::uint8_t devAddr, std::uint16_t regAddr, std::uint8_t* data, std::size_t length, std::uint32_t timeoutMs) noexcept
{
    const std::uint8_t addrBuf[2] = {
        static_cast<std::uint8_t>(regAddr >> 8U),
        static_cast<std::uint8_t>(regAddr & 0xFFU)
    };

    if(!write(devAddr, addrBuf, 2, false, timeoutMs))
    {
        return false;
    }
    return read(devAddr, data, length, timeoutMs);
}

template<Instance I>
bool I2cMaster<I>::isDeviceReady(std::uint8_t address, std::uint32_t trials, std::uint32_t timeoutMs) noexcept
{
    while(trials--)
    {
        systick::Timeout to(timeoutMs);
        if(startAddress(address, false, to))
        {
            helper::clearAddressFlag(Traits<I>::peripheral());
            helper::generateStop(Traits<I>::peripheral());
            return true;
        }
    }
    return false;
}

template<Instance I>
std::size_t I2cMaster<I>::scanBus(std::uint8_t* foundAddresses, std::size_t maxCount) noexcept
{
    std::size_t count = 0;

    // Standard 7-bit address range (0x08 through 0x77)
    for(std::uint8_t addr = 0x08; addr <= 0x77; ++addr)
    {
        if(isDeviceReady(addr, 1, 15))
        {
            if(foundAddresses && count < maxCount)
            {
                foundAddresses[count] = addr;
            }
            ++count;
        }
        systick::delayUs(100);
    }

    return count;
}

} // namespace i2c

