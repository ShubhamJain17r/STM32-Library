#include "stm32/common/registers.hpp"
#include "stm32/exti/exti_manager.hpp"

extern "C"
{

void EXTI0_IRQHandler()
{
    exti::ExtiManager::handleInterrupt(0);
}

void EXTI1_IRQHandler()
{
	exti::ExtiManager::handleInterrupt(1);
}

void EXTI2_IRQHandler()
{
	exti::ExtiManager::handleInterrupt(2);
}

void EXTI3_IRQHandler()
{
	exti::ExtiManager::handleInterrupt(3);
}

void EXTI4_IRQHandler()
{
	exti::ExtiManager::handleInterrupt(4);
}

void EXTI9_5_IRQHandler()
{
    for(int line = 5; line <= 9; ++line)
    {
        if(reg::isAnyBitSet(EXTI->PR, 1U << line))
        {
            exti::ExtiManager::handleInterrupt(static_cast<std::uint8_t>(line));
        }
    }
}

void EXTI15_10_IRQHandler()
{
    for(int line = 10; line <= 15; ++line)
    {
        if(reg::isAnyBitSet(EXTI->PR, 1U << line))
        {
            exti::ExtiManager::handleInterrupt(static_cast<std::uint8_t>(line));
        }
    }
}

} // extern "C"
