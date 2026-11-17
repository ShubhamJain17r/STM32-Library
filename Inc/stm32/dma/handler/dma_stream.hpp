/**
 * @file dma_stream.hpp
 * @brief High-level C++17 type-safe DMA Stream driver for STM32F446
 */

#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <cstddef>

#include "stm32/common/systick/systick.hpp"
#include "stm32/common/callback/callback.hpp"
#include "stm32/dma/core/dma_types.hpp"
#include "stm32/dma/core/dma_traits.hpp"
#include "stm32/dma/core/dma_config.hpp"
#include "stm32/dma/core/dma_helper.hpp"
#include "stm32/dma/interrupt/dma_interrupt.hpp"

namespace dma
{

template<Controller C, StreamId S>
class DmaStream
{
public:
    using StreamTraits = Traits<C, S>;

    static constexpr Controller controllerId = C;
    static constexpr StreamId   streamId     = S;

    ~DmaStream() = default;

    DmaStream(const DmaStream&)            = delete;
    DmaStream& operator=(const DmaStream&) = delete;
    DmaStream(DmaStream&&)                 = delete;
    DmaStream& operator=(DmaStream&&)      = delete;

    /**
     * @brief Constructs and initializes a DMA Stream with given configuration.
     */
    explicit DmaStream(const DmaStreamConfig& config = DmaStreamConfig{}) noexcept
        : config_(config)
    {
        StreamTraits::enableClock();
        init(config_);
    }

    /**
     * @brief Configures stream hardware with specified parameters.
     */
    void init(const DmaStreamConfig& config) noexcept
    {
        config_ = config;

        // 1. Disable stream safely
        helper::disableStream(StreamTraits::stream());

        // 2. Clear all pending interrupt/status flags
        helper::clearAllFlags(StreamTraits::controller(), S);

        // 3. Program CR and FCR bitfields
        helper::applyConfiguration(StreamTraits::stream(), config_);

        // 4. Enable stream IRQ in NVIC
        interrupt::enableIRQ(StreamTraits::irq);
    }

    /**
     * @brief Initiates a data transfer between memory and peripheral (or vice-versa).
     * @param src Pointer to source data / register
     * @param dst Pointer to destination data / register
     * @param length Number of data items (in units of data size)
     */
    void startTransfer(const void* src, void* dst, std::size_t length) noexcept
    {
        auto* stream = StreamTraits::stream();
        auto* ctrl   = StreamTraits::controller();

        helper::disableStream(stream);
        helper::clearAllFlags(ctrl, S);

        if(config_.direction == Direction::PeripheralToMemory)
        {
            helper::setPeripheralAddress(stream, reinterpret_cast<std::uint32_t>(src));
            helper::setMemory0Address(stream, reinterpret_cast<std::uint32_t>(dst));
        }
        else // MemoryToPeripheral or MemoryToMemory
        {
            helper::setMemory0Address(stream, reinterpret_cast<std::uint32_t>(src));
            helper::setPeripheralAddress(stream, reinterpret_cast<std::uint32_t>(dst));
        }

        helper::setItemCount(stream, static_cast<std::uint16_t>(length));
        helper::enableStream(stream);
    }

    /**
     * @brief Initiates a high-speed Memory-to-Memory block transfer.
     * @note M2M transfers are supported ONLY on DMA2 on STM32F446.
     */
    void startMemoryToMemory(const void* src, void* dst, std::size_t length,
                            DataSize size = DataSize::Word) noexcept
    {
        static_assert(C == Controller::Dma2,
            "Memory-to-Memory transfers are physically supported only on DMA2 on STM32F446.");

        config_ = DmaStreamConfig::memoryToMemory(size);
        init(config_);

        auto* stream = StreamTraits::stream();
        auto* ctrl   = StreamTraits::controller();

        helper::disableStream(stream);
        helper::clearAllFlags(ctrl, S);

        // For M2M transfers per RM0390:
        // PAR holds source address, M0AR holds destination address
        helper::setPeripheralAddress(stream, reinterpret_cast<std::uint32_t>(src));
        helper::setMemory0Address(stream, reinterpret_cast<std::uint32_t>(dst));
        helper::setItemCount(stream, static_cast<std::uint16_t>(length));

        helper::enableStream(stream);
    }

    /**
     * @brief Configures double-buffering mode with two memory target buffers.
     */
    void setDoubleBuffer(const void* periph, void* mem0, void* mem1, std::size_t length) noexcept
    {
        auto* stream = StreamTraits::stream();
        auto* ctrl   = StreamTraits::controller();

        helper::disableStream(stream);
        helper::clearAllFlags(ctrl, S);

        config_.doubleBuffer = DoubleBufferMode::Enabled;
        config_.mode         = Mode::Circular;
        helper::applyConfiguration(stream, config_);

        helper::setPeripheralAddress(stream, reinterpret_cast<std::uint32_t>(periph));
        helper::setMemory0Address(stream, reinterpret_cast<std::uint32_t>(mem0));
        helper::setMemory1Address(stream, reinterpret_cast<std::uint32_t>(mem1));
        helper::setItemCount(stream, static_cast<std::uint16_t>(length));

        helper::enableStream(stream);
    }

    /**
     * @brief Aborts the current transfer and disables the stream.
     */
    void abort() noexcept
    {
        helper::disableStream(StreamTraits::stream());
        helper::clearAllFlags(StreamTraits::controller(), S);
    }

    /**
     * @brief Enables the stream.
     */
    void enable() noexcept
    {
        helper::enableStream(StreamTraits::stream());
    }

    /**
     * @brief Disables the stream.
     */
    void disable() noexcept
    {
        helper::disableStream(StreamTraits::stream());
    }

    /**
     * @brief Checks if a transfer is actively ongoing.
     */
    bool isBusy() const noexcept
    {
        return helper::isStreamEnabled(StreamTraits::stream()) &&
               !helper::isTransferComplete(StreamTraits::controller(), S);
    }

    /**
     * @brief Checks if the stream is currently enabled.
     */
    bool isEnabled() const noexcept
    {
        return helper::isStreamEnabled(StreamTraits::stream());
    }

    /**
     * @brief Returns remaining items to be transferred (reads NDTR).
     */
    std::uint16_t remainingItems() const noexcept
    {
        return helper::getRemainingItems(StreamTraits::stream());
    }

    /**
     * @brief Blocks until the current transfer finishes or times out.
     */
    bool waitForCompletion(std::uint32_t timeoutMs = 1000) noexcept
    {
        systick::Timeout timer(timeoutMs);

        while(!helper::isTransferComplete(StreamTraits::controller(), S))
        {
            if(helper::isAnyError(StreamTraits::controller(), S))
            {
                helper::clearErrorFlags(StreamTraits::controller(), S);
                return false;
            }
            if(timer.hasExpired())
            {
                return false;
            }
        }

        helper::clearTransferComplete(StreamTraits::controller(), S);
        return true;
    }

    /**
     * @brief Attaches a user callback for Transfer Complete (TC).
     */
    void attachCompleteCallback(stm32::Callback cb) noexcept
    {
        interrupt::DmaEvent::setUserCallback(C, S, interrupt::Event::TransferComplete, cb);
    }

    /**
     * @brief Attaches a user callback for Half Transfer (HT).
     */
    void attachHalfTransferCallback(stm32::Callback cb) noexcept
    {
        helper::setHalfTransferInterrupt(StreamTraits::stream(), cb != nullptr);
        interrupt::DmaEvent::setUserCallback(C, S, interrupt::Event::HalfTransfer, cb);
    }

    /**
     * @brief Attaches a user callback for Transfer Errors.
     */
    void attachErrorCallback(stm32::Callback cb) noexcept
    {
        interrupt::DmaEvent::setUserCallback(C, S, interrupt::Event::Error, cb);
    }

    /**
     * @brief Developer-level callback hook used by internal peripheral drivers.
     */
    static void setDeveloperCallback(interrupt::Event ev, stm32::Callback cb) noexcept
    {
        interrupt::DmaEvent::setDeveloperCallback(C, S, ev, cb);
    }

private:
    DmaStreamConfig config_;
};

} // namespace dma
