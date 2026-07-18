#include "timer/delay.hpp"

#include "common/registers.hpp"
#include "rcc/rcc.hpp"

namespace timer
{

volatile uint32_t tick_count = 0;

void initSysTick()		// 1 ms interrupt
{
	reg::write(SysTick->LOAD, (rcc::SYSCLK_MHz * 1000UL) - 1UL);
	reg::write(SysTick->VAL, 0UL);

	reg::reset(SysTick->CTRL);
	reg::setBit(SysTick->CTRL, 2);
	reg::setBit(SysTick->CTRL, 2);
	reg::setBit(SysTick->CTRL, 2);
}

std::uint32_t getTickCount(void)
{
	return tick_count;
}

}

extern "C"
{

void SysTick_Handler(void)
{
	timer::tick_count++;
}

}
