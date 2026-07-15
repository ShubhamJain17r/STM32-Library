#include "stm32f446xx.h"
#include <cstdint>

namespace gpio
{

enum class Mode : std::uint8_t
{
	INPUT = 0,
	OUTPUT,
	ALTERNATE,
	ANALOG
};

enum class OutputType : std::uint8_t
{
	PUSH_PULL = 0,
	OPEN_DRAIN
};

enum class OutputSpeed : std::uint8_t
{
	LOW = 0,
	MEDIUM,
	FAST,
	HIGH
};

enum class Pull : std::uint8_t
{
	NONE = 0,
	UP,
	DOWN
};

class Pin
{
private:
	std::uint8_t pinNumber_;
	GPIO_TypeDef* port_;

	void setMode(Mode);
	void setOutputType(OutputType);
	void setOutputSpeed(OutputSpeed);
	void setPull(Pull);
	void setAlternateFunction(std::uint8_t);

public:
	constexpr Pin(std::uint8_t pinNumber, GPIO_TypeDef* port) :
		pinNumber_(pinNumber), port_(port) {}

	void configureInput(Pull pull) const;
	void configureOutput(OutputType outputType, OutputSpeed outputSpeed, Pull pull) const;
	void configureAternate(OutputType outputType, OutputSpeed outputSpeed, Pull pull, std::uint8_t AFType) const;
	void configureAnalog() const;

};

}	// gpio namespace
