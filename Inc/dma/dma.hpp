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
	OperationalMode activeMode_;

public:
	DmaStream() = delete;

	DmaStream(DMA_TypeDef* dmaBase, DMA_Stream_TypeDef* stream, OperationalMode activeMode = OperationalMode::NORMAL) : dmaBase_(dmaBase), stream_(stream), activeMode_(activeMode) {}

	~DmaStream() = default;

	DmaStream(const DmaStream&) = delete;
	DmaStream& operator=(const DmaStream&) = delete;

	DmaStream(DmaStream&&) noexcept = default;
	DmaStream& operator=(DmaStream&&) noexcept = default;

private:

	void disableStream();
	void clearFlags();

	void configureChannel(Channel);

	void configureFlowController(FlowController);

	void configurePriority(Priority);

	void setDirectMode();
	void setCircularMode();
	void setDoubleBufferMode();

	void configureDirection(TransferDirection);

	void configureMemoryDataSize(DataSize);
	void configurePeripheralDataSize(DataSize);

	void configurePeripheralIncrement(bool);
	void configureMemoryIncrement(bool);

	void enableStream();

public:
	void init(const StreamConfig&);

	void setupTransaction(std::uint32_t periphAddr,
	                       std::uint32_t mem0Addr,
	                       std::uint16_t dataLength,
	                       std::uint32_t mem1Addr = 0) noexcept;
};

} // namespace dma
