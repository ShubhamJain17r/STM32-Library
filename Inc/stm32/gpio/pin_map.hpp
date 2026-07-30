#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/rcc/rcc.hpp"

namespace gpio
{

constexpr uint8_t portIndex(GPIO_TypeDef* port)
{
    if(port == GPIOA) return 0;
    if(port == GPIOB) return 1;
    if(port == GPIOC) return 2;
    if(port == GPIOD) return 3;
    if(port == GPIOE) return 4;
    if(port == GPIOF) return 5;
    if(port == GPIOG) return 6;
    if(port == GPIOH) return 7;

    return 0xFF;
}

struct Pin
{
	GPIO_TypeDef* port;
	std::uint8_t number;

	constexpr std::uint32_t mask() const
	{
		return (1U << number);
	}

	constexpr std::uint8_t index() const
	{
		return ((portIndex(port) << 4) | number);
	}

	inline void enableClock() const noexcept
	{
		rcc::enableGpioClock(port);
	}
};

constexpr Pin PA0{GPIOA, 0};
constexpr Pin PA1{GPIOA, 1};
constexpr Pin PA2{GPIOA, 2};
constexpr Pin PA3{GPIOA, 3};
constexpr Pin PA4{GPIOA, 4};
constexpr Pin PA5{GPIOA, 5};
constexpr Pin PA6{GPIOA, 6};
constexpr Pin PA7{GPIOA, 7};
constexpr Pin PA8{GPIOA, 8};
constexpr Pin PA9{GPIOA, 9};
constexpr Pin PA10{GPIOA, 10};
constexpr Pin PA11{GPIOA, 11};
constexpr Pin PA12{GPIOA, 12};
constexpr Pin PA13{GPIOA, 13};
constexpr Pin PA14{GPIOA, 14};
constexpr Pin PA15{GPIOA, 15};

constexpr Pin PB0{GPIOB, 0};
constexpr Pin PB1{GPIOB, 1};
constexpr Pin PB2{GPIOB, 2};
constexpr Pin PB3{GPIOB, 3};
constexpr Pin PB4{GPIOB, 4};
constexpr Pin PB5{GPIOB, 5};
constexpr Pin PB6{GPIOB, 6};
constexpr Pin PB7{GPIOB, 7};
constexpr Pin PB8{GPIOB, 8};
constexpr Pin PB9{GPIOB, 9};
constexpr Pin PB10{GPIOB, 10};
constexpr Pin PB11{GPIOB, 11};
constexpr Pin PB12{GPIOB, 12};
constexpr Pin PB13{GPIOB, 13};
constexpr Pin PB14{GPIOB, 14};
constexpr Pin PB15{GPIOB, 15};

constexpr Pin PC0{GPIOC, 0};
constexpr Pin PC1{GPIOC, 1};
constexpr Pin PC2{GPIOC, 2};
constexpr Pin PC3{GPIOC, 3};
constexpr Pin PC4{GPIOC, 4};
constexpr Pin PC5{GPIOC, 5};
constexpr Pin PC6{GPIOC, 6};
constexpr Pin PC7{GPIOC, 7};
constexpr Pin PC8{GPIOC, 8};
constexpr Pin PC9{GPIOC, 9};
constexpr Pin PC10{GPIOC, 10};
constexpr Pin PC11{GPIOC, 11};
constexpr Pin PC12{GPIOC, 12};
constexpr Pin PC13{GPIOC, 13};
constexpr Pin PC14{GPIOC, 14};
constexpr Pin PC15{GPIOC, 15};

constexpr Pin PD0{GPIOD, 0};
constexpr Pin PD1{GPIOD, 1};
constexpr Pin PD2{GPIOD, 2};
constexpr Pin PD3{GPIOD, 3};
constexpr Pin PD4{GPIOD, 4};
constexpr Pin PD5{GPIOD, 5};
constexpr Pin PD6{GPIOD, 6};
constexpr Pin PD7{GPIOD, 7};
constexpr Pin PD8{GPIOD, 8};
constexpr Pin PD9{GPIOD, 9};
constexpr Pin PD10{GPIOD, 10};
constexpr Pin PD11{GPIOD, 11};
constexpr Pin PD12{GPIOD, 12};
constexpr Pin PD13{GPIOD, 13};
constexpr Pin PD14{GPIOD, 14};
constexpr Pin PD15{GPIOD, 15};

constexpr Pin PE0{GPIOE, 0};
constexpr Pin PE1{GPIOE, 1};
constexpr Pin PE2{GPIOE, 2};
constexpr Pin PE3{GPIOE, 3};
constexpr Pin PE4{GPIOE, 4};
constexpr Pin PE5{GPIOE, 5};
constexpr Pin PE6{GPIOE, 6};
constexpr Pin PE7{GPIOE, 7};
constexpr Pin PE8{GPIOE, 8};
constexpr Pin PE9{GPIOE, 9};
constexpr Pin PE10{GPIOE, 10};
constexpr Pin PE11{GPIOE, 11};
constexpr Pin PE12{GPIOE, 12};
constexpr Pin PE13{GPIOE, 13};
constexpr Pin PE14{GPIOE, 14};
constexpr Pin PE15{GPIOE, 15};

constexpr Pin PF0{GPIOF, 0};
constexpr Pin PF1{GPIOF, 1};
constexpr Pin PF2{GPIOF, 2};
constexpr Pin PF3{GPIOF, 3};
constexpr Pin PF4{GPIOF, 4};
constexpr Pin PF5{GPIOF, 5};
constexpr Pin PF6{GPIOF, 6};
constexpr Pin PF7{GPIOF, 7};
constexpr Pin PF8{GPIOF, 8};
constexpr Pin PF9{GPIOF, 9};
constexpr Pin PF10{GPIOF, 10};
constexpr Pin PF11{GPIOF, 11};
constexpr Pin PF12{GPIOF, 12};
constexpr Pin PF13{GPIOF, 13};
constexpr Pin PF14{GPIOF, 14};
constexpr Pin PF15{GPIOF, 15};

constexpr Pin PG0{GPIOG, 0};
constexpr Pin PG1{GPIOG, 1};
constexpr Pin PG2{GPIOG, 2};
constexpr Pin PG3{GPIOG, 3};
constexpr Pin PG4{GPIOG, 4};
constexpr Pin PG5{GPIOG, 5};
constexpr Pin PG6{GPIOG, 6};
constexpr Pin PG7{GPIOG, 7};
constexpr Pin PG8{GPIOG, 8};
constexpr Pin PG9{GPIOG, 9};
constexpr Pin PG10{GPIOG, 10};
constexpr Pin PG11{GPIOG, 11};
constexpr Pin PG12{GPIOG, 12};
constexpr Pin PG13{GPIOG, 13};
constexpr Pin PG14{GPIOG, 14};
constexpr Pin PG15{GPIOG, 15};

constexpr Pin PH0{GPIOH, 0};
constexpr Pin PH1{GPIOH, 1};
constexpr Pin PH2{GPIOH, 2};
constexpr Pin PH3{GPIOH, 3};
constexpr Pin PH4{GPIOH, 4};
constexpr Pin PH5{GPIOH, 5};
constexpr Pin PH6{GPIOH, 6};
constexpr Pin PH7{GPIOH, 7};
constexpr Pin PH8{GPIOH, 8};
constexpr Pin PH9{GPIOH, 9};
constexpr Pin PH10{GPIOH, 10};
constexpr Pin PH11{GPIOH, 11};
constexpr Pin PH12{GPIOH, 12};
constexpr Pin PH13{GPIOH, 13};
constexpr Pin PH14{GPIOH, 14};
constexpr Pin PH15{GPIOH, 15};

} // namespace gpio
