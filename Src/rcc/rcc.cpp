#include "rcc/rcc.hpp"

#include "common/registers.hpp"

namespace rcc {

inline void enableClock_GPIO(GPIO_TypeDef *port) {
	if (port < GPIOA || port > GPIOH) {
		return;
	}

	std::uint32_t port_index = (reinterpret_cast<std::uintptr_t>(port) - GPIOA_BASE) / 0x400;

	if (port_index <= 7) { // Validate range (GPIOA to GPIOH)
		reg::setBit(RCC->AHB1ENR, port_index);
	}
}

inline void enableClock_SYSCFG()
{
	reg::setBit(RCC->APB2ENR, 14);
}

} // namespace rcc
