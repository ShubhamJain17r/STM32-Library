#include "i2c/i2c.hpp"

#include "rcc/rcc.hpp"

namespace i2c
{

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

    enablePeripheral();
}

void I2cHandler::configureDMA()
{

}

void I2cHandler::configureGpio()
{

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


} // namespace i2c
