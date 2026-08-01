#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/common/rcc.hpp"

#include "stm32/gpio/pin_map.hpp"

namespace uart
{

template<USART_TypeDef* USART>
struct Traits;

template<>
struct Traits<USART1>
{
	static constexpr gpio::Pin defaultTx = gpio::PA9;
	static constexpr gpio::Pin defaultRx = gpio::PA10;

	static constexpr IRQn_Type irq = USART1_IRQn;

	void enableClock() noexcept
	{
		rcc::enableUartClock(USART1);
	}

	static constexpr std::uint32_t busClock = rcc::apb2Clock;
};

template<>
struct Traits<USART2>
{
	static constexpr gpio::Pin defaultTx = gpio::PA2;
	static constexpr gpio::Pin defaultRx = gpio::PA3;

	static constexpr IRQn_Type irq = USART2_IRQn;

	void enableClock() noexcept
	{
		rcc::enableUartClock(USART2);
	}

	static constexpr std::uint32_t busClock = rcc::apb1Clock;
};

template<>
struct Traits<USART3>
{
	static constexpr gpio::Pin defaultTx = gpio::PB10;
	static constexpr gpio::Pin defaultRx = gpio::PB11;

	static constexpr IRQn_Type irq = USART3_IRQn;

	void enableClock() noexcept
	{
		rcc::enableUartClock(USART3);
	}

	static constexpr std::uint32_t busClock = rcc::apb1Clock;
};

template<>
struct Traits<UART4>
{
	static constexpr gpio::Pin defaultTx = gpio::PA0;
	static constexpr gpio::Pin defaultRx = gpio::PA1;

	static constexpr IRQn_Type irq = UART4_IRQn;

	void enableClock() noexcept
	{
		rcc::enableUartClock(UART4);
	}

	static constexpr std::uint32_t busClock = rcc::apb1Clock;
};

template<>
struct Traits<UART5>
{
	static constexpr gpio::Pin defaultTx = gpio::PC12;
	static constexpr gpio::Pin defaultRx = gpio::PD2;

	static constexpr IRQn_Type irq = UART5_IRQn;

	void enableClock() noexcept
	{
		rcc::enableUartClock(UART5);
	}

	static constexpr std::uint32_t busClock = rcc::apb1Clock;
};

template<>
struct Traits<USART6>
{
	static constexpr gpio::Pin defaultTx = gpio::PC6;
	static constexpr gpio::Pin defaultRx = gpio::PC7;

	static constexpr IRQn_Type irq = USART6_IRQn;

	void enableClock() noexcept
	{
		rcc::enableUartClock(USART6);
	}

	static constexpr std::uint32_t busClock = rcc::apb2Clock;
};



} // namespace uart
