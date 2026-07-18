#include "rcc/rcc.hpp"

#include "common/registers.hpp"

namespace rcc {

void enableClock_GPIO(GPIO_TypeDef *port) {
	// Calculate difference between target port and base GPIOA address
	std::uint32_t port_index = (reinterpret_cast<std::uint32_t>(port) - GPIOA_BASE) / 0x400;
	if (port_index <= 7) { // Validate range (GPIOA to GPIOH)
		reg::setBit(RCC->AHB1ENR, port_index);
	}
}

void enableClock_SYSCFG()
{
	reg::setBit(RCC->APB2ENR, 14);
}

} // namespace rcc
