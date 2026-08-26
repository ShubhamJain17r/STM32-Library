/**
 * @file spi_config.hpp
 * @brief Configuration structures and default settings for SPI peripherals
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/core/pin.hpp"
#include "stm32/spi/core/spi_types.hpp"
#include "stm32/spi/core/spi_traits.hpp"

namespace spi
{

template<Instance I>
struct SpiConfig
{
    gpio::Pin sck  = Traits<I>::defaultSck;
    gpio::Pin miso = Traits<I>::defaultMiso;
    gpio::Pin mosi = Traits<I>::defaultMosi;
    gpio::Pin nss  = Traits<I>::defaultNss;

    std::uint32_t baudRateHz = 1'000'000; // Default 1 MHz

    Mode mode = Mode::Master;
    SpiMode spiMode = SpiMode::Mode0;
    BitOrder bitOrder = BitOrder::MsbFirst;
    DataSize dataSize = DataSize::Bits8;
    Direction direction = Direction::FullDuplex;
    SlaveSelect slaveSelect = SlaveSelect::Software;

    constexpr SpiConfig() = default;

    constexpr explicit SpiConfig(std::uint32_t baud, SpiMode sm = SpiMode::Mode0)
        : baudRateHz(baud), spiMode(sm)
    {}

    constexpr SpiConfig(gpio::Pin sckPin, gpio::Pin misoPin, gpio::Pin mosiPin, std::uint32_t baud = 1'000'000)
        : sck(sckPin), miso(misoPin), mosi(mosiPin), baudRateHz(baud)
    {}

    constexpr SpiConfig(gpio::Pin sckPin, gpio::Pin misoPin, gpio::Pin mosiPin, gpio::Pin nssPin, std::uint32_t baud = 1'000'000)
        : sck(sckPin), miso(misoPin), mosi(mosiPin), nss(nssPin), baudRateHz(baud)
    {}
};

} // namespace spi

