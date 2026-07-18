#include "rcc/rcc.hpp"

#include "common/registers.hpp"

namespace rcc
{

void enableClock_GPIO(GPIO_TypeDef* port)
{
	if(port == GPIOA)
	{
		reg::setBit(RCC->AHB1ENR, 0);
	}
	else if(port == GPIOB)
	{
		reg::setBit(RCC->AHB1ENR, 1);
	}
	else if(port == GPIOC)
	{
		reg::setBit(RCC->AHB1ENR, 2);
	}
	else if(port == GPIOD)
	{
		reg::setBit(RCC->AHB1ENR, 3);
	}
	else if(port == GPIOE)
	{
		reg::setBit(RCC->AHB1ENR, 4);
	}
	else if(port == GPIOF)
	{
		reg::setBit(RCC->AHB1ENR, 5);
	}
	else if(port == GPIOG)
	{
		reg::setBit(RCC->AHB1ENR, 6);
	}
	else if(port == GPIOH)
	{
		reg::setBit(RCC->AHB1ENR, 7);
	}
}

}
