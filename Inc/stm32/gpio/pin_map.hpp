#pragma once

#include "stm32f446xx.h"
#include <cstdint>

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

	constexpr std::uint8_t index() const
	{
		return (portIndex(port) << 4) | number;
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

} // namespace gpio
