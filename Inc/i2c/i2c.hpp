#pragma once

#include <cstdint>
#include "stm32f446xx.h"

#include "i2c/i2c_types.hpp"

#include "common/registers.hpp"

namespace i2c
{

class I2cHandler
{
private:
	I2C_TypeDef* i2cBase_;

public:
	I2cHandler() = delete;
	~I2cHandler() = default;

	I2cHandler(I2C_TypeDef* i2cBase) : i2cBase_(i2cBase) {}

	I2cHandler(const I2cHandler&) = delete;
	I2cHandler operator=(const I2cHandler&) = delete;

	I2cHandler(const I2cHandler&&) = default;
	I2cHandler operator=(const I2cHandler&&) = default;

private:
	void generateStart()
	{
		reg::waitUntilSet(i2cBase_->SR2, I2C_SR2_BUSY_Pos);

		reg::setBit(i2cBase_->CR1, I2C_CR1_START_Pos);
	}

	void generateRepeatedStart()
	{
		reg::setBit(i2cBase_->CR1, I2C_CR1_START_Pos);
	}

	void sendTargetAddress(std::uint8_t address, Operation op)
	{
		reg::write(i2cBase_->DR, ((address << 1) | op));
	}

	void clearAddrFlag()
	{
		(void)reg::read(i2cBase_->SR1);
		(void)reg::read(i2cBase_->SR2);
	}

	void clearSB()
	{
		(void)reg::read(i2cBase_->SR1);
	}

	void sendData(const std::uint8_t* stream, std::uint16_t len);

	void generateStop()
	{
		reg::setBit(i2cBase_->CR1, I2C_CR1_STOP_Pos);
	}

	void configureDMA();

	void configureI2c();

public:
	void init(const I2cConfig&);

};

} // namespace i2c
