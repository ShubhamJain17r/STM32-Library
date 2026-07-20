#include "rcc/rcc.hpp"
#include "common/registers.hpp"

namespace rcc {

void enableGpioClock(const GPIO_TypeDef *port) {
    if (port < GPIOA || port > GPIOH) {
        return;
    }

    std::uint32_t port_index = (reinterpret_cast<std::uintptr_t>(port) - GPIOA_BASE) / 0x400;
    reg::setBit(RCC->AHB1ENR, port_index);
}

void enableDmaClock(const DMA_TypeDef* dmaBase)
{
	if(dmaBase == DMA1)
	{
		reg::setBit(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN_Pos);
	}
	else if(dmaBase == DMA2)
	{
		reg::setBit(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN_Pos);
	}
}

} // namespace rcc
