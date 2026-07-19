#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "dma/dma_types.hpp"

namespace dma {

class DmaStream
{
private:
	DMA_TypeDef* dmaBase_;
	DMA_Stream_TypeDef* stream_;

public:
	DmaStream() = delete;

	DmaStream(DMA_TypeDef* dmaBase, DMA_Stream_TypeDef* stream) : dmaBase_(dmaBase), stream_(stream) {}

	~DmaStream() = default;

	DmaStream(const DmaStream&) = delete;
	DmaStream& operator=(const DmaStream&) = delete;

	DmaStream(DmaStream&&) noexcept = default;
	DmaStream& operator=(DmaStream&&) noexcept = default;

private:

	void disableStream();
	void clearFlags();

	void configurePeripheralAddress(const volatile std::uint32_t& periphAddr);
	void configureMemoryAddress(const std::uint32_t& memoryAddr);
	void configureNDTR(std::uint16_t N);

	void configureChannel(Channel);

	void configureFlowController(FlowController);

	void setPriority(Priority);

	void configureDirectMode();
	void configureCircularMode();
	void configureDoubleBufferMode();

	void configureDirection(TransferDirection);

	void configureDataSize(DataSize);

	void configurePeripheralIncrement(bool);
	void configureMemoryIncrement(bool);

	void enableStream();

public:
	void init(const StreamConfig&);
};

} // namespace dma
