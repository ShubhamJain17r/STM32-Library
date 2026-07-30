#include "stm32f446xx.h"
#include <cstdint>

#include "stm32/gpio/gpio.hpp"

using namespace gpio;

int main()
{
	DigitalOutput led(PA5);
	while(1);
}
