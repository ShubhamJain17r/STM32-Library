/**
 * @file dma_request_traits.hpp
 * @brief STM32F446 peripheral DMA request routing traits (RM0390 Tables 27 & 28)
 */

#pragma once

#include "stm32/dma/core/dma_types.hpp"

namespace dma
{

/**
 * @brief Standard Hardware Peripheral DMA Requests on STM32F446
 */
enum class Request
{
    // USART / UART
    Usart1_Tx,
    Usart1_Rx,
    Usart2_Tx,
    Usart2_Rx,
    Usart3_Tx,
    Usart3_Rx,
    Uart4_Tx,
    Uart4_Rx,
    Uart5_Tx,
    Uart5_Rx,
    Usart6_Tx,
    Usart6_Rx,

    // SPI
    Spi1_Tx,
    Spi1_Rx,
    Spi2_Tx,
    Spi2_Rx,
    Spi3_Tx,
    Spi3_Rx,
    Spi4_Tx,
    Spi4_Rx,

    // I2C
    I2c1_Tx,
    I2c1_Rx,
    I2c2_Tx,
    I2c2_Rx,
    I2c3_Tx,
    I2c3_Rx,

    // ADC
    Adc1,
    Adc2,
    Adc3
};

template<Request R>
struct RequestTraits;

// ---------------------------------------------------------------------------
// USART / UART Request Mappings
// ---------------------------------------------------------------------------

template<>
struct RequestTraits<Request::Usart1_Tx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream7;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Usart1_Rx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream2;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Usart2_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream6;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Usart2_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream5;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Usart3_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream3;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Usart3_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream1;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Uart4_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream4;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Uart4_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream2;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Uart5_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream7;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Uart5_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream0;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Usart6_Tx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream6;
    static constexpr ChannelId  channel    = ChannelId::Channel5;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Usart6_Rx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream1;
    static constexpr ChannelId  channel    = ChannelId::Channel5;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

// ---------------------------------------------------------------------------
// SPI Request Mappings
// ---------------------------------------------------------------------------

template<>
struct RequestTraits<Request::Spi1_Tx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream3;
    static constexpr ChannelId  channel    = ChannelId::Channel3;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Spi1_Rx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream0;
    static constexpr ChannelId  channel    = ChannelId::Channel3;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Spi2_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream4;
    static constexpr ChannelId  channel    = ChannelId::Channel0;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Spi2_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream3;
    static constexpr ChannelId  channel    = ChannelId::Channel0;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Spi3_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream5;
    static constexpr ChannelId  channel    = ChannelId::Channel0;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Spi3_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream2;
    static constexpr ChannelId  channel    = ChannelId::Channel0;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Spi4_Tx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream1;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::Spi4_Rx>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream0;
    static constexpr ChannelId  channel    = ChannelId::Channel4;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

// ---------------------------------------------------------------------------
// I2C Request Mappings
// ---------------------------------------------------------------------------

template<>
struct RequestTraits<Request::I2c1_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream6;
    static constexpr ChannelId  channel    = ChannelId::Channel1;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::I2c1_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream0;
    static constexpr ChannelId  channel    = ChannelId::Channel1;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::I2c2_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream7;
    static constexpr ChannelId  channel    = ChannelId::Channel7;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::I2c2_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream2;
    static constexpr ChannelId  channel    = ChannelId::Channel7;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::I2c3_Tx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream4;
    static constexpr ChannelId  channel    = ChannelId::Channel3;
    static constexpr Direction  direction  = Direction::MemoryToPeripheral;
};

template<>
struct RequestTraits<Request::I2c3_Rx>
{
    static constexpr Controller controller = Controller::Dma1;
    static constexpr StreamId   stream     = StreamId::Stream2;
    static constexpr ChannelId  channel    = ChannelId::Channel3;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

// ---------------------------------------------------------------------------
// ADC Request Mappings
// ---------------------------------------------------------------------------

template<>
struct RequestTraits<Request::Adc1>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream0;
    static constexpr ChannelId  channel    = ChannelId::Channel0;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Adc2>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream2;
    static constexpr ChannelId  channel    = ChannelId::Channel1;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

template<>
struct RequestTraits<Request::Adc3>
{
    static constexpr Controller controller = Controller::Dma2;
    static constexpr StreamId   stream     = StreamId::Stream1;
    static constexpr ChannelId  channel    = ChannelId::Channel2;
    static constexpr Direction  direction  = Direction::PeripheralToMemory;
};

} // namespace dma
