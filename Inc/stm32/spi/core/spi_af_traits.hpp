/**
 * @file spi_af_traits.hpp
 * @brief Alternate function pin capability mappings for SPI peripherals on STM32F446
 */

#pragma once

#include <cstdint>
#include <array>

#include "stm32/gpio/core/pin.hpp"
#include "stm32/gpio/core/pin_map.hpp"
#include "stm32/gpio/af/af_types.hpp"
#include "stm32/spi/core/spi_types.hpp"

namespace spi::af
{

using gpio::Pin;
using gpio::af::AlternateFunction;

enum class Signal : std::uint8_t
{
    SCK,
    MISO,
    MOSI,
    NSS
};

template<Instance I, Signal S>
struct SignalPins;

// ===========================================================================
// SPI1 (AF5)
// ===========================================================================
template<> struct SignalPins<Instance::spi1, Signal::SCK> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PA5, gpio::PB3};
};
template<> struct SignalPins<Instance::spi1, Signal::MISO> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PA6, gpio::PB4};
};
template<> struct SignalPins<Instance::spi1, Signal::MOSI> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PA7, gpio::PB5};
};
template<> struct SignalPins<Instance::spi1, Signal::NSS> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PA4, gpio::PA15};
};

// ===========================================================================
// SPI2 (AF5)
// ===========================================================================
template<> struct SignalPins<Instance::spi2, Signal::SCK> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 4> pins = {gpio::PB10, gpio::PB13, gpio::PC7, gpio::PD3};
};
template<> struct SignalPins<Instance::spi2, Signal::MISO> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PB14, gpio::PC2};
};
template<> struct SignalPins<Instance::spi2, Signal::MOSI> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 3> pins = {gpio::PB15, gpio::PC1, gpio::PC3};
};
template<> struct SignalPins<Instance::spi2, Signal::NSS> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 3> pins = {gpio::PB9, gpio::PB12, gpio::PD0};
};

// ===========================================================================
// SPI3 (AF6)
// ===========================================================================
template<> struct SignalPins<Instance::spi3, Signal::SCK> {
    static constexpr AlternateFunction af = AlternateFunction::AF6;
    static constexpr std::array<Pin, 2> pins = {gpio::PB3, gpio::PC10};
};
template<> struct SignalPins<Instance::spi3, Signal::MISO> {
    static constexpr AlternateFunction af = AlternateFunction::AF6;
    static constexpr std::array<Pin, 2> pins = {gpio::PB4, gpio::PC11};
};
template<> struct SignalPins<Instance::spi3, Signal::MOSI> {
    static constexpr AlternateFunction af = AlternateFunction::AF6;
    static constexpr std::array<Pin, 2> pins = {gpio::PB5, gpio::PC12};
};
template<> struct SignalPins<Instance::spi3, Signal::NSS> {
    static constexpr AlternateFunction af = AlternateFunction::AF6;
    static constexpr std::array<Pin, 2> pins = {gpio::PA4, gpio::PA15};
};

// ===========================================================================
// SPI4 (AF5)
// ===========================================================================
template<> struct SignalPins<Instance::spi4, Signal::SCK> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PE2, gpio::PE12};
};
template<> struct SignalPins<Instance::spi4, Signal::MISO> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PE5, gpio::PE13};
};
template<> struct SignalPins<Instance::spi4, Signal::MOSI> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PE6, gpio::PE14};
};
template<> struct SignalPins<Instance::spi4, Signal::NSS> {
    static constexpr AlternateFunction af = AlternateFunction::AF5;
    static constexpr std::array<Pin, 2> pins = {gpio::PE4, gpio::PE11};
};

template<Instance I, Signal S>
constexpr bool supports(Pin pin) noexcept
{
    for(const auto& p : SignalPins<I, S>::pins)
    {
        if(p == pin) return true;
    }
    return false;
}

template<Instance I, Signal S>
constexpr AlternateFunction alternateFunction(Pin pin) noexcept
{
    return supports<I, S>(pin) ? SignalPins<I, S>::af : AlternateFunction::INVALID;
}

} // namespace spi::af

