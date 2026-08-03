#include "stm32f446xx.h"
#include <cstdint>
//
//#include "stm32/gpio/gpio.hpp"
//
//using namespace gpio;
//
inline void delay(std::uint32_t t)
{
	for(std::uint32_t i = 0; i < t; ++i);
}
//
//int main()
//{
//	DigitalOutput led(PA5,
//	{
//			.outputType = OutputType::PUSH_PULL,
////			.outputSpeed = OutputSpeed::LOW,
//			.pull = Pull::DOWN,
////			.initialState = PinState::HIGH
//	});
//
//	while(1)
//	{
//		led.write(PinState::HIGH);
//		delay(1000000);
//		led.write(PinState::LOW);
//		delay(1000000);
//
//		led.high();
//		delay(1000000);
//		led.low();
//		delay(1000000);
//
//		if(led.read() == PinState::LOW)
//		{
//			led.high();
//		}
//
//		delay(2000000);
//
//		if(led.read() == PinState::HIGH)
//		{
//			led.low();
//		}
//
//		delay(6000000);
//	}
//}

#include "stm32/uart/uart.hpp"
#include "stm32/gpio/gpio.hpp"

using namespace uart;
using namespace gpio;
int main()
{
	Uart2 Serial;
//
//	DigitalOutput led(PA5);
//
//	while(1)
//	{
//		Serial.write("Hello World\r\n");
//		delay(1000000);
//
//		if(Serial.available())
//		{
//			char c = Serial.read();
//
//			Serial.write(c);
//
//			if(c == 'n')
//			{
//				led.high();
//			}
//		}
//	}

	char c = Serial.read();

	Serial.write(c);

	Serial.write("Hello World\r\n");

}
