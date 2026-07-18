#pragma once

#include "stm32f446xx.h"
#include <cstdint>

#include "common/callback.hpp"
#include "gpio/gpio.hpp"

namespace exti {

enum class Edge {
	RISING,
	FALLING,
	BOTH
};

class ExternalInterrupt : private gpio::Pin {
private:
	Edge edge_;

public:
	constexpr ExternalInterrupt(std::uint8_t pinNumber, GPIO_TypeDef *port, Edge edge) :
				pinNumber_(pinNumber), port_(port), edge_(edge) {
		register_instances();
	}

	void setCallback(InterruptEdge, Callback);

	void handleISR();

private:
    void enable_nvic();

    void register_instance();

    std::array<Callback, 3> callbacks_{}; // RISING, FALLING, BOTH

public:
    // Static Dispatcher System
    static std::array<ExternalInterrupt*, 16> active_instances; // For gpio pin 0-15

};

}
