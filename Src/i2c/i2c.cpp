#include "i2c/i2c.hpp"

#include "rcc/rcc.hpp"

namespace i2c
{

void I2cHandler::configureI2c(CommunicationSpeed spd)
{
	rcc::enableI2cClock(i2cBase_);

	disablePeripheral();

	reg::write(i2cBase_->CR2, rcc::SYSCLK_MHz);

	reg::setBitField<1>(i2cBase_->CCR, I2C_CCR_FS_Pos, spd);

	if(spd == CommunicationSpeed::STANDARD_MODE)
	{
		reg::write(i2cBase_->CCR, rcc::SYSCLK_MHz * 5);

		reg::write(i2cBase_->TRISE, rcc::SYSCLK_MHz + 1);
	}
	else
	{
		if(rcc::SYSCLK_MHz % 10 == 0)
		{
			reg::setBit(i2cBase_->CCR, I2C_CCR_DUTY_Pos);

			if(rcc::SYSCLK_MHz % 10 >= 5)
			{
				reg::write(i2cBase_->CCR, (rcc::SYSCLK_MHz / 10) + 1);
			}
			else
			{
				reg::write(i2cBase_->CCR, rcc::SYSCLK_MHz * 5);
			}
		}
		else
		{
			reg::write(i2cBase_->CCR, (rcc::SYSCLK_MHz * 5) / 6);
		}

		reg::write(i2cBase_->TRISE, ((rcc::SYSCLK_MHz * 3) / 10) + 1);
	}

	enablePeripheral();
}

} // namespace i2c
