#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <array>

#include "dma/dma_types.hpp"
#include "common/registers.hpp"
#include "common/callback.hpp"

namespace dma {

class DmaStream
{
private:
	DMA_TypeDef* dmaBase_;
	DMA_Stream_TypeDef* stream_;
	OperationalMode activeMode_;

	std::array<callback::Callback, 5> callbacks_{};

public:
	DmaStream() = delete;

	DmaStream(DMA_TypeDef* dmaBase, DMA_Stream_TypeDef* stream, OperationalMode activeMode = OperationalMode::NORMAL) : dmaBase_(dmaBase), stream_(stream), activeMode_(activeMode) {
		registerInstance();
	}

	~DmaStream() = default;

	DmaStream(const DmaStream&) = delete;
	DmaStream& operator=(const DmaStream&) = delete;

	DmaStream(DmaStream&&) noexcept = default;
	DmaStream& operator=(DmaStream&&) noexcept = default;

private:
	void clearAllFlags() noexcept;

    void enableNVIC() const;

    void registerInstance();

	inline void configureChannel(Channel ch) const
	{
		reg::setBitField<3>(stream_->CR, DMA_SxCR_CHSEL_Pos, ch);
	}

	inline void configureFlowController(FlowController fc) const
	{
		reg::setBitField<1>(stream_->CR, DMA_SxCR_PFCTRL_Pos, fc);
	}

	inline void configurePriority(Priority pl) const
	{
		reg::setBitField<2>(stream_->CR, DMA_SxCR_PL_Pos, pl);
	}

	inline void setDirectMode() const
	{
		reg::resetBit(stream_->FCR, DMA_SxFCR_DMDIS_Pos);
	}

	inline void setCircularMode() const
	{
		reg::setBit(stream_->CR, DMA_SxCR_CIRC_Pos);
	}

	inline void setDoubleBufferMode() const
	{
		reg::setBit(stream_->CR, DMA_SxCR_DBM_Pos);
	}

	inline void configureDirection(TransferDirection dr) const
	{
		reg::setBitField<2>(stream_->CR, DMA_SxCR_DIR_Pos, dr);
	}

	inline void configureMemoryDataSize(DataSize sz) const
	{
		reg::setBitField<2>(stream_->CR, DMA_SxCR_MSIZE_Pos, sz);
	}

	inline void configurePeripheralDataSize(DataSize sz) const
	{
		reg::setBitField<2>(stream_->CR, DMA_SxCR_MSIZE_Pos, sz);
	}

	inline void configurePeripheralIncrement(bool increment) const
	{
		reg::setBitField<1>(stream_->CR, DMA_SxCR_PINC_Pos, increment);
	}

	inline void configureMemoryIncrement(bool increment) const
	{
		reg::setBitField<1>(stream_->CR, DMA_SxCR_MINC_Pos, increment);
	}

public:
	void init(const StreamConfig&) const;

	void setupTransaction(std::uint32_t periphAddr,
	                       std::uint32_t mem0Addr,
	                       std::uint16_t dataLength,
	                       std::uint32_t mem1Addr = 0) noexcept;

	inline void enableStream() const
	{
		reg::setBit(stream_->CR, DMA_SxCR_EN_Pos);
	}

	inline void disableStream() const
	{
		reg::resetBit(stream_->CR, DMA_SxCR_EN_Pos);

		reg::waitUntilReset(stream_->CR, DMA_SxCR_EN_Pos);
	}

	void setCallback(Event event, callback::Callback func);

	void handleISR() const;

	static std::array<std::array<DmaStream*, 8>, 2> active_instances;
};

} // namespace dma
