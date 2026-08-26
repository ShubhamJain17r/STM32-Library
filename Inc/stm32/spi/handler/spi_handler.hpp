/**
 * @file spi_handler.hpp
 * @brief High-level SPI master & slave driver with polling and asynchronous transfer APIs
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <cstddef>

#include "stm32/common/registers/registers.hpp"
#include "stm32/common/rcc/rcc.hpp"
#include "stm32/common/rcc/rcc_enable.hpp"
#include "stm32/common/callback/callback.hpp"

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/gpio_types.hpp"
#include "stm32/gpio/core/gpio_helper.hpp"
#include "stm32/gpio/af/af_helper.hpp"
#include "stm32/gpio/digital_output/digital_output.hpp"

#include "stm32/spi/core/spi_types.hpp"
#include "stm32/spi/core/spi_traits.hpp"
#include "stm32/spi/core/spi_af_traits.hpp"
#include "stm32/spi/core/spi_config.hpp"
#include "stm32/spi/core/spi_helper.hpp"
#include "stm32/spi/interrupt/spi_interrupt.hpp"

namespace spi
{

/**
 * @brief RAII Guard for Software Chip Select (CS) pins
 */
class ChipSelectGuard
{
public:
    explicit ChipSelectGuard(gpio::DigitalOutput& cs) noexcept : cs_(cs)
    {
        cs_.low();
    }

    ~ChipSelectGuard()
    {
        cs_.high();
    }

    ChipSelectGuard(const ChipSelectGuard&) = delete;
    ChipSelectGuard& operator=(const ChipSelectGuard&) = delete;

private:
    gpio::DigitalOutput& cs_;
};

template<Instance I>
class SpiHandler
{
private:
    struct AsyncState
    {
        const std::uint8_t* txBuf    = nullptr;
        std::uint8_t*       rxBuf    = nullptr;
        std::size_t         length   = 0;
        volatile std::size_t txIndex = 0;
        volatile std::size_t rxIndex = 0;
        volatile bool       busy     = false;
    };

    inline static AsyncState asyncState_;

    static void handleTxAsync() noexcept;
    static void handleRxAsync() noexcept;

public:
    ~SpiHandler();

    SpiHandler(const SpiHandler&) = delete;
    SpiHandler& operator=(const SpiHandler&) = delete;

    SpiHandler(SpiHandler&&) = delete;
    SpiHandler& operator=(SpiHandler&&) = delete;

    explicit SpiHandler(std::uint32_t baudRateHz = 1'000'000, SpiMode mode = SpiMode::Mode0);
    explicit SpiHandler(gpio::Pin sck, gpio::Pin miso, gpio::Pin mosi, std::uint32_t baudRateHz = 1'000'000);
    explicit SpiHandler(const SpiConfig<I>& config);

    // Synchronous / Polling Transfers
    std::uint8_t  transfer(std::uint8_t data) noexcept;
    std::uint16_t transfer16(std::uint16_t data) noexcept;

    void transmit(const std::uint8_t* data, std::size_t length) noexcept;
    void transmit16(const std::uint16_t* data, std::size_t length) noexcept;

    void receive(std::uint8_t* data, std::size_t length, std::uint8_t dummy = 0xFF) noexcept;
    void receive16(std::uint16_t* data, std::size_t length, std::uint16_t dummy = 0xFFFF) noexcept;

    void transfer(const std::uint8_t* txBuf, std::uint8_t* rxBuf, std::size_t length) noexcept;
    void transfer16(const std::uint16_t* txBuf, std::uint16_t* rxBuf, std::size_t length) noexcept;

    // Asynchronous / Non-blocking Transfers
    bool transferAsync(const std::uint8_t* txBuf, std::uint8_t* rxBuf, std::size_t length) noexcept;
    bool isAsyncBusy() const noexcept;
    void attachTransferCompleteCallback(stm32::Callback cb) noexcept;

    void enable() noexcept;
    void disable() noexcept;
    bool isBusy() const noexcept;

    void setBaudRate(std::uint32_t baudRateHz) noexcept;
    void setPrescaler(Prescaler psc) noexcept;
    void setSpiMode(SpiMode mode) noexcept;
    void setDataSize(DataSize size) noexcept;
    void setBitOrder(BitOrder order) noexcept;

private:
    void initPins(const SpiConfig<I>& config) noexcept;
    Prescaler calculatePrescaler(std::uint32_t targetBaudHz) const noexcept;
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

template<Instance I>
Prescaler SpiHandler<I>::calculatePrescaler(std::uint32_t targetBaudHz) const noexcept
{
    if(targetBaudHz == 0) targetBaudHz = 1;

    const std::uint32_t pclk = rcc::frequency(Traits<I>::bus);

    // Divisors: 2, 4, 8, 16, 32, 64, 128, 256
    constexpr std::uint32_t divisors[8] = {2, 4, 8, 16, 32, 64, 128, 256};

    for(std::uint8_t i = 0; i < 7; ++i)
    {
        if(pclk / divisors[i] <= targetBaudHz)
        {
            return static_cast<Prescaler>(i);
        }
    }

    return Prescaler::Div256;
}

template<Instance I>
void SpiHandler<I>::initPins(const SpiConfig<I>& config) noexcept
{
    // Configure SCK Pin
    const auto sckAf = af::alternateFunction<I, af::Signal::SCK>(config.sck);
    if(sckAf != af::AlternateFunction::INVALID)
    {
        config.sck.enableClock();
        gpio::af::setAlternateFunction(config.sck, sckAf);
        gpio::helper::setOutputType(config.sck, gpio::OutputType::PUSH_PULL);
        gpio::helper::setOutputSpeed(config.sck, gpio::OutputSpeed::HIGH);
        gpio::helper::setPull(config.sck, gpio::Pull::NONE);
        gpio::helper::setMode(config.sck, gpio::Mode::ALTERNATE);
    }

    // Configure MISO Pin
    const auto misoAf = af::alternateFunction<I, af::Signal::MISO>(config.miso);
    if(misoAf != af::AlternateFunction::INVALID)
    {
        config.miso.enableClock();
        gpio::af::setAlternateFunction(config.miso, misoAf);
        gpio::helper::setPull(config.miso, gpio::Pull::NONE);
        gpio::helper::setMode(config.miso, gpio::Mode::ALTERNATE);
    }

    // Configure MOSI Pin
    const auto mosiAf = af::alternateFunction<I, af::Signal::MOSI>(config.mosi);
    if(mosiAf != af::AlternateFunction::INVALID)
    {
        config.mosi.enableClock();
        gpio::af::setAlternateFunction(config.mosi, mosiAf);
        gpio::helper::setOutputType(config.mosi, gpio::OutputType::PUSH_PULL);
        gpio::helper::setOutputSpeed(config.mosi, gpio::OutputSpeed::HIGH);
        gpio::helper::setPull(config.mosi, gpio::Pull::NONE);
        gpio::helper::setMode(config.mosi, gpio::Mode::ALTERNATE);
    }

    // Configure Hardware NSS Pin if requested
    if(config.slaveSelect != SlaveSelect::Software)
    {
        const auto nssAf = af::alternateFunction<I, af::Signal::NSS>(config.nss);
        if(nssAf != af::AlternateFunction::INVALID)
        {
            config.nss.enableClock();
            gpio::af::setAlternateFunction(config.nss, nssAf);
            gpio::helper::setOutputType(config.nss, gpio::OutputType::PUSH_PULL);
            gpio::helper::setOutputSpeed(config.nss, gpio::OutputSpeed::HIGH);
            gpio::helper::setPull(config.nss, gpio::Pull::NONE);
            gpio::helper::setMode(config.nss, gpio::Mode::ALTERNATE);
        }
    }
}

template<Instance I>
SpiHandler<I>::SpiHandler(std::uint32_t baudRateHz, SpiMode mode)
    : SpiHandler(SpiConfig<I>{baudRateHz, mode})
{}

template<Instance I>
SpiHandler<I>::SpiHandler(gpio::Pin sck, gpio::Pin miso, gpio::Pin mosi, std::uint32_t baudRateHz)
    : SpiHandler(SpiConfig<I>{sck, miso, mosi, baudRateHz})
{}

template<Instance I>
SpiHandler<I>::SpiHandler(const SpiConfig<I>& config)
{
    // 1. Enable peripheral clock
    Traits<I>::enableClock();

    auto* spi = Traits<I>::peripheral();

    // 2. Disable SPI before configuration
    helper::disable(spi);

    // 3. Configure GPIO Alternate Functions
    initPins(config);

    // 4. Configure Master / Slave mode
    helper::setMode(spi, config.mode);

    // 5. Configure Baud Rate Prescaler
    const Prescaler psc = calculatePrescaler(config.baudRateHz);
    helper::setPrescaler(spi, psc);

    // 6. Configure SPI Mode (CPOL / CPHA)
    helper::setSpiMode(spi, config.spiMode);

    // 7. Configure Data Size and Bit Order
    helper::setDataSize(spi, config.dataSize);
    helper::setBitOrder(spi, config.bitOrder);
    helper::setDirection(spi, config.direction);

    // 8. Configure Slave Management (SSM / SSI / SSOE)
    if(config.slaveSelect == SlaveSelect::Software)
    {
        helper::setSoftwareSlaveManagement(spi, true);
        helper::setInternalSlaveSelect(spi, config.mode == Mode::Master);
    }
    else if(config.slaveSelect == SlaveSelect::HardwareOutput)
    {
        helper::setSoftwareSlaveManagement(spi, false);
        helper::setSSOutput(spi, true);
    }
    else // HardwareInput
    {
        helper::setSoftwareSlaveManagement(spi, false);
        helper::setSSOutput(spi, false);
    }

    // 9. Register developer async interrupt handlers
    interrupt::SpiEvent::setDeveloperCallback(I, interrupt::Event::TxEmpty,    &SpiHandler::handleTxAsync);
    interrupt::SpiEvent::setDeveloperCallback(I, interrupt::Event::RxNotEmpty, &SpiHandler::handleRxAsync);

    // 10. Enable NVIC IRQ and SPI peripheral
    interrupt::enableIRQ(Traits<I>::irq);
    helper::enable(spi);
}

template<Instance I>
SpiHandler<I>::~SpiHandler()
{
    disable();
    interrupt::disableIRQ(Traits<I>::irq);
}

template<Instance I>
std::uint8_t SpiHandler<I>::transfer(std::uint8_t data) noexcept
{
    auto* spi = Traits<I>::peripheral();

    // Wait until TX buffer is empty
    while(!helper::isTxEmpty(spi));
    helper::write8(spi, data);

    // Wait until RX buffer is not empty
    while(!helper::isRxNotEmpty(spi));
    return helper::read8(spi);
}

template<Instance I>
std::uint16_t SpiHandler<I>::transfer16(std::uint16_t data) noexcept
{
    auto* spi = Traits<I>::peripheral();

    while(!helper::isTxEmpty(spi));
    helper::write16(spi, data);

    while(!helper::isRxNotEmpty(spi));
    return helper::read16(spi);
}

template<Instance I>
void SpiHandler<I>::transmit(const std::uint8_t* data, std::size_t length) noexcept
{
    while(length--)
    {
        transfer(*data++);
    }
    while(isBusy());
}

template<Instance I>
void SpiHandler<I>::transmit16(const std::uint16_t* data, std::size_t length) noexcept
{
    while(length--)
    {
        transfer16(*data++);
    }
    while(isBusy());
}

template<Instance I>
void SpiHandler<I>::receive(std::uint8_t* data, std::size_t length, std::uint8_t dummy) noexcept
{
    while(length--)
    {
        *data++ = transfer(dummy);
    }
    while(isBusy());
}

template<Instance I>
void SpiHandler<I>::receive16(std::uint16_t* data, std::size_t length, std::uint16_t dummy) noexcept
{
    while(length--)
    {
        *data++ = transfer16(dummy);
    }
    while(isBusy());
}

template<Instance I>
void SpiHandler<I>::transfer(const std::uint8_t* txBuf, std::uint8_t* rxBuf, std::size_t length) noexcept
{
    for(std::size_t i = 0; i < length; ++i)
    {
        const std::uint8_t txByte = txBuf ? txBuf[i] : 0xFFU;
        const std::uint8_t rxByte = transfer(txByte);
        if(rxBuf)
        {
            rxBuf[i] = rxByte;
        }
    }
    while(isBusy());
}

template<Instance I>
void SpiHandler<I>::transfer16(const std::uint16_t* txBuf, std::uint16_t* rxBuf, std::size_t length) noexcept
{
    for(std::size_t i = 0; i < length; ++i)
    {
        const std::uint16_t txWord = txBuf ? txBuf[i] : 0xFFFFU;
        const std::uint16_t rxWord = transfer16(txWord);
        if(rxBuf)
        {
            rxBuf[i] = rxWord;
        }
    }
    while(isBusy());
}

template<Instance I>
bool SpiHandler<I>::transferAsync(const std::uint8_t* txBuf, std::uint8_t* rxBuf, std::size_t length) noexcept
{
    if(asyncState_.busy || length == 0) return false;

    asyncState_.txBuf   = txBuf;
    asyncState_.rxBuf   = rxBuf;
    asyncState_.length  = length;
    asyncState_.txIndex = 0;
    asyncState_.rxIndex = 0;
    asyncState_.busy    = true;

    auto* spi = Traits<I>::peripheral();

    // Enable RXNE and TXE interrupts
    helper::enableRxInterrupt(spi);
    helper::enableTxInterrupt(spi);

    return true;
}

template<Instance I>
void SpiHandler<I>::handleTxAsync() noexcept
{
    auto* spi = Traits<I>::peripheral();

    if(asyncState_.txIndex < asyncState_.length)
    {
        const std::uint8_t byte = asyncState_.txBuf ? asyncState_.txBuf[asyncState_.txIndex] : 0xFFU;
        helper::write8(spi, byte);
        ++asyncState_.txIndex;
    }
    else
    {
        helper::disableTxInterrupt(spi);
    }
}

template<Instance I>
void SpiHandler<I>::handleRxAsync() noexcept
{
    auto* spi = Traits<I>::peripheral();

    const std::uint8_t byte = helper::read8(spi);

    if(asyncState_.rxIndex < asyncState_.length)
    {
        if(asyncState_.rxBuf)
        {
            asyncState_.rxBuf[asyncState_.rxIndex] = byte;
        }
        ++asyncState_.rxIndex;
    }

    if(asyncState_.rxIndex >= asyncState_.length)
    {
        helper::disableRxInterrupt(spi);
        asyncState_.busy = false;

        // Trigger user callback
        interrupt::SpiEvent::handleEvent(I);
    }
}

template<Instance I>
bool SpiHandler<I>::isAsyncBusy() const noexcept
{
    return asyncState_.busy;
}

template<Instance I>
void SpiHandler<I>::attachTransferCompleteCallback(stm32::Callback cb) noexcept
{
    interrupt::SpiEvent::setUserCallback(I, interrupt::Event::TransferComplete, cb);
}

template<Instance I>
void SpiHandler<I>::enable() noexcept
{
    helper::enable(Traits<I>::peripheral());
}

template<Instance I>
void SpiHandler<I>::disable() noexcept
{
    helper::disable(Traits<I>::peripheral());
}

template<Instance I>
bool SpiHandler<I>::isBusy() const noexcept
{
    return helper::isBusy(Traits<I>::peripheral());
}

template<Instance I>
void SpiHandler<I>::setBaudRate(std::uint32_t baudRateHz) noexcept
{
    const bool wasEnabled = helper::isEnabled(Traits<I>::peripheral());
    if(wasEnabled) disable();

    setPrescaler(calculatePrescaler(baudRateHz));

    if(wasEnabled) enable();
}

template<Instance I>
void SpiHandler<I>::setPrescaler(Prescaler psc) noexcept
{
    helper::setPrescaler(Traits<I>::peripheral(), psc);
}

template<Instance I>
void SpiHandler<I>::setSpiMode(SpiMode mode) noexcept
{
    const bool wasEnabled = helper::isEnabled(Traits<I>::peripheral());
    if(wasEnabled) disable();

    helper::setSpiMode(Traits<I>::peripheral(), mode);

    if(wasEnabled) enable();
}

template<Instance I>
void SpiHandler<I>::setDataSize(DataSize size) noexcept
{
    const bool wasEnabled = helper::isEnabled(Traits<I>::peripheral());
    if(wasEnabled) disable();

    helper::setDataSize(Traits<I>::peripheral(), size);

    if(wasEnabled) enable();
}

template<Instance I>
void SpiHandler<I>::setBitOrder(BitOrder order) noexcept
{
    const bool wasEnabled = helper::isEnabled(Traits<I>::peripheral());
    if(wasEnabled) disable();

    helper::setBitOrder(Traits<I>::peripheral(), order);

    if(wasEnabled) enable();
}

} // namespace spi

