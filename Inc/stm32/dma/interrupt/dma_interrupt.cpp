/**
 * @file dma_interrupt.cpp
 * @brief DMA interrupt dispatch implementation
 */

#include "stm32/dma/interrupt/dma_interrupt.hpp"
#include "stm32/dma/core/dma_helper.hpp"

namespace dma::interrupt
{

void DmaEvent::setDeveloperCallback(Controller c, StreamId s, Event ev, stm32::Callback cb) noexcept
{
    const auto cIdx = static_cast<std::size_t>(c);
    const auto sIdx = static_cast<std::size_t>(s);

    if(cIdx < 2 && sIdx < 8)
    {
        switch(ev)
        {
            case Event::TransferComplete:
                developerCallbacks_[cIdx][sIdx].onComplete = cb;
                break;
            case Event::HalfTransfer:
                developerCallbacks_[cIdx][sIdx].onHalf = cb;
                break;
            case Event::Error:
                developerCallbacks_[cIdx][sIdx].onError = cb;
                break;
        }
    }
}

void DmaEvent::setUserCallback(Controller c, StreamId s, Event ev, stm32::Callback cb) noexcept
{
    const auto cIdx = static_cast<std::size_t>(c);
    const auto sIdx = static_cast<std::size_t>(s);

    if(cIdx < 2 && sIdx < 8)
    {
        switch(ev)
        {
            case Event::TransferComplete:
                userCallbacks_[cIdx][sIdx].onComplete = cb;
                break;
            case Event::HalfTransfer:
                userCallbacks_[cIdx][sIdx].onHalf = cb;
                break;
            case Event::Error:
                userCallbacks_[cIdx][sIdx].onError = cb;
                break;
        }
    }
}

void DmaEvent::handleInterrupt(Controller c, StreamId s) noexcept
{
    const auto cIdx = static_cast<std::size_t>(c);
    const auto sIdx = static_cast<std::size_t>(s);
    DMA_TypeDef* dma = (c == Controller::Dma1) ? DMA1 : DMA2;

    // 1. Check for transfer errors (TEIF, DMEIF, FEIF)
    if(helper::isAnyError(dma, s))
    {
        helper::clearErrorFlags(dma, s);

        if(developerCallbacks_[cIdx][sIdx].onError)
        {
            developerCallbacks_[cIdx][sIdx].onError();
        }
        if(userCallbacks_[cIdx][sIdx].onError)
        {
            userCallbacks_[cIdx][sIdx].onError();
        }
    }

    // 2. Check for Half Transfer Complete (HTIF)
    if(helper::isHalfTransfer(dma, s))
    {
        helper::clearHalfTransfer(dma, s);

        if(developerCallbacks_[cIdx][sIdx].onHalf)
        {
            developerCallbacks_[cIdx][sIdx].onHalf();
        }
        if(userCallbacks_[cIdx][sIdx].onHalf)
        {
            userCallbacks_[cIdx][sIdx].onHalf();
        }
    }

    // 3. Check for Transfer Complete (TCIF)
    if(helper::isTransferComplete(dma, s))
    {
        helper::clearTransferComplete(dma, s);

        if(developerCallbacks_[cIdx][sIdx].onComplete)
        {
            developerCallbacks_[cIdx][sIdx].onComplete();
        }
        if(userCallbacks_[cIdx][sIdx].onComplete)
        {
            userCallbacks_[cIdx][sIdx].onComplete();
        }
    }
}

} // namespace dma::interrupt
