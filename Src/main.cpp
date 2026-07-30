#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/gpio.hpp"

using namespace gpio;

inline void delay(std::uint32_t t)
{
	for(std::uint32_t i = 0; i < t; ++i);
}

int main()
{
	DigitalOutput led(PA5);

	while(1)
	{
		led.write(PinState::HIGH);
		delay(1000000);
		led.write(PinState::LOW);
		delay(1000000);

		led.high();
		delay(1000000);
		led.low();
		delay(1000000);

		if(led.read() == PinState::LOW)
		{
			led.high();
		}

		delay(2000000);

		if(led.read() == PinState::HIGH)
		{
			led.low();
		}

		delay(6000000);

	}
}
