#include "dma/dma.hpp"

#include "rcc/rcc.hpp"

namespace dma
{

void DmaStream::clearAllFlags() noexcept
{
	DMA_Stream_TypeDef* stream0_base = reinterpret_cast<DMA_Stream_TypeDef*>(reinterpret_cast<std::uintptr_t>(dmaBase_) + 0x010U);
    std::uint8_t stream_idx = stream_ - stream0_base;

    constexpr std::uint32_t ALL_FLAGS_MASK = 0x3DU;

    std::uint8_t bit_shift = 0;
    switch (stream_idx & 0x03U) {
        case 0: bit_shift = 0;  break; // Stream 0 or 4
        case 1: bit_shift = 6;  break; // Stream 1 or 5
        case 2: bit_shift = 16; break; // Stream 2 or 6
        case 3: bit_shift = 22; break; // Stream 3 or 7
        default: return;
    }

    const std::uint32_t clear_value = ALL_FLAGS_MASK << bit_shift;

    if (stream_idx < 4)
    {
        reg::write(dmaBase_->LIFCR, clear_value);
    }
	else
	{
    		reg::write(dmaBase_->HIFCR, clear_value);
    }
}

void DmaStream::init(const StreamConfig& config)
{
	rcc::enableDmaClock(dmaBase_);

	configureChannel(config.channel);
	configureFlowController(config.flowController);
	configurePriority(config.priority);
	setDirectMode();

	configureDirection(config.direction);

	configurePeripheralDataSize(config.dataSize);
	configureMemoryDataSize(config.dataSize);

	configurePeripheralIncrement(config.peripheralIncrement);
	configureMemoryIncrement(config.memoryIncrement);

	if(activeMode_ == OperationalMode::CIRCULAR)
	{
		setCircularMode();
	}
	else if(activeMode_ == OperationalMode::DOUBLE_BUFFER)
	{
		setDoubleBufferMode();
	}
}

void DmaStream::setupTransaction(std::uint32_t periphAddr,
                       std::uint32_t mem0Addr,
                       std::uint16_t dataLength,
                       std::uint32_t mem1Addr) noexcept
{
	reg::write(stream_->PAR, periphAddr);
	reg::write(stream_->M0AR, mem0Addr);
	reg::write(stream_->NDTR, dataLength);

	if((activeMode_ == OperationalMode::DOUBLE_BUFFER) && mem1Addr != 0)
	{
		reg::write(stream_->M1AR, mem1Addr);
	}
}

} // namespace dma
