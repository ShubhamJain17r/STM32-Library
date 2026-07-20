#include "dma/dma.hpp"

#include "rcc/rcc.hpp"

namespace
{

std::uint32_t getDmaId(DMA_TypeDef* dmaBase)
{
	return (dmaBase == DMA2) ? 1 : 0;
}

std::uint32_t getStreamId(DMA_TypeDef* dmaBase, DMA_Stream_TypeDef* stream)
{
	return (reinterpret_cast<std::uint32_t>(stream) - reinterpret_cast<std::uint32_t>(dmaBase) - 0x10) / 0x18;
}

IRQn_Type getIrqNumber(DMA_TypeDef* dmaBase, DMA_Stream_TypeDef* stream) {
        // 1. Calculate DMA Controller ID: DMA1 -> 0, DMA2 -> 1
        std::uint32_t dmaId = getDmaId(dmaBase);

        // 2. Calculate Stream ID (0 to 7) based on standard register step sizes
        // Each stream is offset by 0x18 bytes from the previous one
        std::uint32_t streamId = getStreamId(dmaBase, stream);

        if (streamId > 7) {
            // Guardrail check against an invalid pointer
            return static_cast<IRQn_Type>(0);
        }

        // 3. Compact lookup matrix [DMA_ID][STREAM_ID]
        static const IRQn_Type irqTable[2][8] = {
            {
                DMA1_Stream0_IRQn, DMA1_Stream1_IRQn, DMA1_Stream2_IRQn, DMA1_Stream3_IRQn,
                DMA1_Stream4_IRQn, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn, DMA1_Stream7_IRQn
            },
            {
                DMA2_Stream0_IRQn, DMA2_Stream1_IRQn, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn,
                DMA2_Stream4_IRQn, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn, DMA2_Stream7_IRQn
            }
        };

        return irqTable[dmaId][streamId];
    }

} // empty namespace

namespace dma
{

std::array<std::array<DmaStream*, 8>, 2> DmaStream::active_instances = {nullptr};

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

	enableNVIC();
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

void DmaStream::setCallback(Event event, callback::Callback func)
{
	callbacks_[static_cast<std::size_t>(event)] = func;
}

void DmaStream::enableNVIC() const
{
    NVIC_EnableIRQ(getIrqNumber(dmaBase_, stream_));
}

void DmaStream::registerInstance()
{
	active_instances[getDmaId(dmaBase_)][getStreamId(dmaBase_, stream_)] = this;
}

} // namespace dma

extern "C"
{

void DMA1_Stream0_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][0])
	{
		dma::DmaStream::active_instances[0][0]->handleISR();
	}
}

void DMA1_Stream1_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][1])
	{
		dma::DmaStream::active_instances[0][1]->handleISR();
	}
}

void DMA1_Stream2_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][2])
	{
		dma::DmaStream::active_instances[0][2]->handleISR();
	}
}

void DMA1_Stream3_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][3])
	{
		dma::DmaStream::active_instances[0][3]->handleISR();
	}
}

void DMA1_Stream4_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][4])
	{
		dma::DmaStream::active_instances[0][4]->handleISR();
	}
}

void DMA1_Stream5_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][5])
	{
		dma::DmaStream::active_instances[0][5]->handleISR();
	}
}

void DMA1_Stream6_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][6])
	{
		dma::DmaStream::active_instances[0][6]->handleISR();
	}
}

void DMA1_Stream7_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[0][7])
	{
		dma::DmaStream::active_instances[0][7]->handleISR();
	}
}

void DMA2_Stream0_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][0])
	{
		dma::DmaStream::active_instances[1][0]->handleISR();
	}
}

void DMA2_Stream1_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][1])
	{
		dma::DmaStream::active_instances[1][1]->handleISR();
	}
}

void DMA2_Stream2_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][2])
	{
		dma::DmaStream::active_instances[1][2]->handleISR();
	}
}

void DMA2_Stream3_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][3])
	{
		dma::DmaStream::active_instances[1][3]->handleISR();
	}
}

void DMA2_Stream4_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][4])
	{
		dma::DmaStream::active_instances[1][4]->handleISR();
	}
}

void DMA2_Stream5_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][5])
	{
		dma::DmaStream::active_instances[1][5]->handleISR();
	}
}

void DMA2_Stream6_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][6])
	{
		dma::DmaStream::active_instances[1][6]->handleISR();
	}
}

void DMA2_Stream7_IRQHandler(void)
{
	if(dma::DmaStream::active_instances[1][7])
	{
		dma::DmaStream::active_instances[1][7]->handleISR();
	}
}

} // extern "C"
