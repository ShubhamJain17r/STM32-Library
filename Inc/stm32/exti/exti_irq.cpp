#include "stm32/common/registers/registers.hpp"
#include "stm32/exti/exti_manager.hpp"

extern "C"
{

void EXTI0_IRQHandler(void)
{
	if(exti::isPending(0))
	{
		exti::ExtiManager::handleInterrupt(0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(exti::isPending(1))
	{
		exti::ExtiManager::handleInterrupt(1);
	}
}

void EXTI2_IRQHandler(void)
{
	if(exti::isPending(2))
	{
		exti::ExtiManager::handleInterrupt(2);
	}
}

void EXTI3_IRQHandler(void)
{
	if(exti::isPending(3))
	{
		exti::ExtiManager::handleInterrupt(3);
	}
}

void EXTI4_IRQHandler(void)
{
	if(exti::isPending(4))
	{
		exti::ExtiManager::handleInterrupt(4);
	}
}

void EXTI9_5_IRQHandler(void)
{
	for(std::uint8_t line = 5; line <= 9; ++line)
	{
		if(exti::isPending(line))
		{
			exti::ExtiManager::handleInterrupt(line);
		}
	}
}

void EXTI15_10_IRQHandler(void)
{
	for(std::uint8_t line = 10; line <= 15; ++line)
	{
		if(exti::isPending(line))
		{
			exti::ExtiManager::handleInterrupt(line);
		}
	}
}

} // extern "C"
