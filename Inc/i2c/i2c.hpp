#pragma once

#include <cstdint>
#include <array>
#include "stm32f446xx.h"

#include "i2c/i2c_types.hpp"

#include "common/registers.hpp"

#include "dma/dma.hpp"
#include "dma/dma_types.hpp"

#include "gpio/gpio.hpp"
#include "gpio/gpio_types.hpp"

namespace i2c
{

class I2cHandler
{
private:
	I2C_TypeDef* i2cBase_;

	dma::DmaStream txDma_;
	dma::DmaStream rxDma_;

	dma::Channel dmaChannel_;

	gpio::Pin sdaPin_;
	gpio::Pin sclPin_;

	std::array<callback::Callback, 3> eventCallbacks_{};
	std::array<callback::Callback, 4> errorCallbacks_{};

public:
	I2cHandler() = delete;
	~I2cHandler() = default;

	explicit I2cHandler(I2C_TypeDef*);

	I2cHandler(const I2cHandler&) = delete;
	I2cHandler& operator=(const I2cHandler&) = delete;

	I2cHandler(I2cHandler&&) noexcept = default;
	I2cHandler& operator=(I2cHandler&&) noexcept = default;

private:
	void enablePeripheral()
	{
		reg::setBit(i2cBase_->CR1, I2C_CR1_PE_Pos);
	}

	void disablePeripheral()
	{
		reg::resetBit(i2cBase_->CR1, I2C_CR1_PE_Pos);
	}

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
		reg::write(i2cBase_->DR, ((address << 1) | static_cast<std::uint32_t>(op)));
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

	void generateStop()
	{
		reg::setBit(i2cBase_->CR1, I2C_CR1_STOP_Pos);
	}

	void configureDMA();

	void configureGpio();

	void configureI2c(std::uint16_t);

	static DmaMapping getHardwareMapping(I2C_TypeDef* i2cBase);
	static GpioMapping getPinMapping(I2C_TypeDef* i2cBase);

	void enableNVIC();

	void registerInstance();

public:
	void init(std::uint16_t speedKHz = 100);

	void transmit(const std::uint8_t* stream, std::uint16_t len);

	void receive(std::uint8_t* stream, std::uint16_t len);

	void enableDmaStream()
	{
		reg::setBit(i2cBase_->CR2, I2C_CR2_DMAEN_Pos);
	}

	void disableDmaStream()
	{
		reg::resetBit(i2cBase_->CR2, I2C_CR2_DMAEN_Pos);
	}

	void setCallback(Event event, callback::Callback func);
	void setCallback(Error error, callback::Callback func);

	void handleEventISR();
	void handleErrorISR();

	static std::array<I2cHandler*, 3> active_instances;
};

} // namespace i2c
