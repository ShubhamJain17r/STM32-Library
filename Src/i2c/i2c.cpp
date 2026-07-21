#include "i2c/i2c.hpp"

#include "rcc/rcc.hpp"

namespace
{
	std::uint8_t getI2cId(I2C_TypeDef* i2cBase)
	{
		if(i2cBase == I2C1) return 0;
		if(i2cBase == I2C2) return 1;
		if(i2cBase == I2C3) return 2;
		return -1;
	}
}

namespace i2c
{

std::array<I2cHandler*, 3> I2cHandler::active_instances = {nullptr};

I2cHandler::I2cHandler(I2C_TypeDef* i2cBase)
	:	i2cBase_(i2cBase),
		txDma_(getHardwareMapping(i2cBase).txDmaBase,
			   getHardwareMapping(i2cBase).txStream),
		rxDma_(getHardwareMapping(i2cBase).rxDmaBase,
			   getHardwareMapping(i2cBase).rxStream),
		dmaChannel_(getHardwareMapping(i2cBase).channel),
		sdaPin_(getPinMapping(i2cBase).sdaPort,
				getPinMapping(i2cBase).sdaPin),
		sclPin_(getPinMapping(i2cBase).sclPort,
				getPinMapping(i2cBase).sclPin){}

void I2cHandler::configureI2c(std::uint16_t speedKHz)
{
    rcc::enableI2cClock(i2cBase_);

    disablePeripheral();

    reg::write(i2cBase_->CR2, rcc::PCLK1_MHz);

    constexpr std::uint32_t pclkHz = rcc::PCLK1;

    std::uint32_t ccr = 0;
    std::uint32_t trise = 0;
    std::uint32_t ccrReg = 0;

    if (speedKHz <= 100)
    {

        ccr = pclkHz / (2U * speedKHz * 1000U);

        if (ccr < 4U)
            ccr = 4U;

        trise = rcc::PCLK1_MHz + 1U;

        ccrReg = ccr;
    }
    else
    {
        ccr = pclkHz / (3U * speedKHz * 1000U);

        if (ccr == 0U)
            ccr = 1U;

        trise = ((rcc::PCLK1_MHz * 300U) / 1000U) + 1U;

        ccrReg = ccr;
        ccrReg |= reg::singleBitMask(I2C_CCR_FS_Pos);   // Fast mode
    }

    reg::write(i2cBase_->CCR, ccrReg);
    reg::write(i2cBase_->TRISE, trise);

    reg::setBit(i2cBase_->CR2, I2C_CR2_ITEVTEN_Pos);
    reg::setBit(i2cBase_->CR2, I2C_CR2_ITERREN_Pos);

    enablePeripheral();
}

void I2cHandler::configureDMA()
{
	txDma_.init(
		{
			dmaChannel_,
			dma::TransferDirection::MEMORY_TO_PERIPHERAL,
			dma::FlowController::DMA,
			dma::Priority::MEDIUM,
			dma::DataSize::BYTE,
			false,
			true
		}
	);

	rxDma_.init(
		{
			dmaChannel_,
			dma::TransferDirection::PERIPHERAL_TO_MEMORY,
			dma::FlowController::DMA,
			dma::Priority::MEDIUM,
			dma::DataSize::BYTE,
			false,
			true
		}
	);
}

void I2cHandler::configureGpio()
{
	sdaPin_.configureAlternate(gpio::AlternateFunction::AF4, gpio::OutputType::OPEN_DRAIN, gpio::OutputSpeed::LOW, gpio::Pull::UP);
	sclPin_.configureAlternate(gpio::AlternateFunction::AF4, gpio::OutputType::OPEN_DRAIN, gpio::OutputSpeed::LOW, gpio::Pull::UP);
}

DmaMapping I2cHandler::getHardwareMapping(I2C_TypeDef* i2cBase) {
    if (i2cBase == I2C1) {
		return {
			DMA1,
			DMA1_Stream6,
			DMA1,
			DMA1_Stream0,
			dma::Channel::CH1
		};
    }
    else if (i2cBase == I2C2) {
        return {
            DMA1,
            DMA1_Stream7, // I2C2_TX -> Channel 7, Stream 7
            DMA1,
            DMA1_Stream2, // I2C2_RX -> Channel 7, Stream 2
            dma::Channel::CH7
        };
    }
    else if(i2cBase == I2C3){
			return {
			DMA1,
			DMA1_Stream4, // I2C3_TX -> Channel 3, Stream 4
			DMA1,
			DMA1_Stream2, // I2C3_RX -> Channel 3, Stream 2
			dma::Channel::CH3
		};
    }

	return {};
}

GpioMapping I2cHandler::getPinMapping(I2C_TypeDef* i2cBase)
{
	if(i2cBase == I2C1)
	{
		return {
			GPIOB,
			7,
			GPIOB,
			6
		};
	}
	else if(i2cBase == I2C2)
	{
		return {
			GPIOB,
			11,
			GPIOB,
			10
		};
	}
	else if(i2cBase == I2C3)
	{
		return {
			GPIOB,
			4,
			GPIOA,
			8
		};
	}

	return {};
}

void I2cHandler::init(std::uint16_t speedKHz)
{
	configureGpio();
	configureI2c(speedKHz);
	configureDMA();
}

void I2cHandler::transmit(const std::uint8_t* stream, std::uint16_t len)
{
	txDma_.setupTransaction(
			reinterpret_cast<std::uint32_t>(&i2cBase_->DR),
			stream,
			len
	);

	txDma_.enableStream();

	enableDmaStream();

	generateStart();
}

void I2cHandler::receive(std::uint8_t* stream, std::uint16_t len)
{
	rxDma_.setupTransaction(
			reinterpret_cast<std::uint32_t>(&i2cBase_->DR),
			stream,
			len
	);

	rxDma_.enableStream();

	enableDmaStream();

	generateStart();
}

void I2cHandler::setCallback(Event event, callback::Callback func) {
    eventCallbacks_[static_cast<std::size_t>(event)] = func;
}

void I2cHandler::setCallback(Error error, callback::Callback func) {
    errorCallbacks_[static_cast<std::size_t>(error)] = func;
}

void I2cHandler::registerInstance()
{
	active_instances[getI2cId(i2cBase_)] = this;
}

void enableNVIC()
{
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
	NVIC_EnableIRQ(I2C2_EV_IRQn);
	NVIC_EnableIRQ(I2C2_ER_IRQn);
	NVIC_EnableIRQ(I2C3_EV_IRQn);
	NVIC_EnableIRQ(I2C3_ER_IRQn);
}

void handleEventISR()
{

}

} // namespace i2c
