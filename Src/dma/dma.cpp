#include "dma/dma.hpp"

#include "common/registers.hpp"

namespace dma
{

void DmaStream::disableStream()
{
	reg::resetBit(stream_->CR, DMA_SxCR_EN_Pos);
}

void DmaStream::clearFlags()
{

}

void DmaStream::configureChannel(Channel ch)
{
	reg::setBitField<3>(stream_->CR, DMA_SxCR_CHSEL_Pos, ch);
}

void DmaStream::configureFlowController(FlowController fc)
{
	reg::setBitField<1>(stream_->CR, DMA_SxCR_PFCTRL_Pos, fc);
}

void DmaStream::configurePriority(Priority pl)
{
	reg::setBitField<2>(stream_->CR, DMA_SxCR_PL_Pos, pl);
}

void DmaStream::setDirectMode()
{
	reg::resetBit(stream_->FCR, DMA_SxFCR_DMDIS_Pos);
}

void DmaStream::setCircularMode()
{
	reg::setBit(stream_->CR, DMA_SxCR_CIRC_Pos);
}

void DmaStream::setDoubleBufferMode()
{
	reg::setBit(stream_->CR, DMA_SxCR_DBM_Pos);
}

void DmaStream::configureDirection(TransferDirection dr)
{
	reg::setBitField<2>(stream_->CR, DMA_SxCR_DIR_Pos, dr);
}

void DmaStream::configureMemoryDataSize(DataSize sz)
{
	reg::setBitField<2>(stream_->CR, DMA_SxCR_MSIZE_Pos, sz);
}

void DmaStream::configurePeripheralDataSize(DataSize sz)
{
	reg::setBitField<2>(stream_->CR, DMA_SxCR_MSIZE_Pos, sz);
}

void DmaStream::configurePeripheralIncrement(bool increment)
{
	reg::setBitField<1>(stream_->CR, DMA_SxCR_PINC_Pos, increment);
}

void DmaStream::configureMemoryIncrement(bool increment)
{
	reg::setBitField<1>(stream_->CR, DMA_SxCR_MINC_Pos, increment);
}

void DmaStream::enableStream()
{
	reg::setBit(stream_->CR, DMA_SxCR_EN_Pos);
}

void DmaStream::init(const StreamConfig& config)
{
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

} // namespace dma
