#pragma once

#include <cstdint>
#include <array>

#include "stm32/gpio/af/af_types.hpp"
#include "Stm32/gpio/pin_map.hpp"

namespace gpio::af
{

template<Signal>
struct Traits;

template<>
struct Traits<Signal::USART1_TX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF7;

	static constexpr std::array<Pin, 2> pins = {PA9, PB6};
};

template<>
struct Traits<Signal::USART1_RX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF7;

	static constexpr std::array<Pin, 2> pins = {PA10, PB7};
};

template<>
struct Traits<Signal::USART2_TX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF7;

	static constexpr std::array<Pin, 2> pins = {PA2, PD5};
};

template<>
struct Traits<Signal::USART2_RX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF7;

	static constexpr std::array<Pin, 2> pins = {PA3, PD6};
};

template<>
struct Traits<Signal::USART3_TX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF7;

	static constexpr std::array<Pin, 2> pins = {PB10, PD8};
};

template<>
struct Traits<Signal::USART3_RX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF7;

	static constexpr std::array<Pin, 2> pins = {PB11, PD9};
};

template<>
struct Traits<Signal::UART4_TX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF8;

	static constexpr std::array<Pin, 2> pins = {PA0, PC10};
};

template<>
struct Traits<Signal::UART4_RX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF8;

	static constexpr std::array<Pin, 2> pins = {PA1, PC11};
};

template<>
struct Traits<Signal::UART5_TX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF8;

	static constexpr std::array<Pin, 2> pins = {PC12, PE8};
};

template<>
struct Traits<Signal::UART5_RX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF8;

	static constexpr std::array<Pin, 2> pins = {PD2, PE7};
};

template<>
struct Traits<Signal::USART6_TX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF8;

	static constexpr std::array<Pin, 2> pins = {PC6, PG14};
};

template<>
struct Traits<Signal::USART6_RX>
{
	static constexpr AlternateFunction af = AlternateFunction::AF8;

	static constexpr std::array<Pin, 2> pins = {PC7, PG9};
};

} // namespace gpio::af
