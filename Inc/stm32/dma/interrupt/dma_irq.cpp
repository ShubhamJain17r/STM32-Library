/**
 * @file dma_irq.cpp
 * @brief Hardware IRQ vector handlers for DMA1 and DMA2 streams
 */

#include "stm32/dma/interrupt/dma_interrupt.hpp"

extern "C"
{

void DMA1_Stream0_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream0);
}

void DMA1_Stream1_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream1);
}

void DMA1_Stream2_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream2);
}

void DMA1_Stream3_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream3);
}

void DMA1_Stream4_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream4);
}

void DMA1_Stream5_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream5);
}

void DMA1_Stream6_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream6);
}

void DMA1_Stream7_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma1, dma::StreamId::Stream7);
}

void DMA2_Stream0_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream0);
}

void DMA2_Stream1_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream1);
}

void DMA2_Stream2_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream2);
}

void DMA2_Stream3_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream3);
}

void DMA2_Stream4_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream4);
}

void DMA2_Stream5_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream5);
}

void DMA2_Stream6_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream6);
}

void DMA2_Stream7_IRQHandler()
{
    dma::interrupt::DmaEvent::handleInterrupt(dma::Controller::Dma2, dma::StreamId::Stream7);
}

} // extern "C"
