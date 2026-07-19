#pragma once

#include "stm32f446xx.h"
#include <cstdint>
#include <array>
#include "common/callback.hpp"
#include "gpio/gpio.hpp"

namespace exti {

enum class Edge : std::uint8_t {
    RISING,
    FALLING,
    BOTH
};

class ExternalInterrupt {
public:
    ExternalInterrupt() = delete;

    ExternalInterrupt(const gpio::Pin& pin, Edge edge)
    	    : pinNumber_(pin.getPinNumber()), port_(pin.getPort()), edge_(edge) {
    	    registerInstance();
    	}

    ExternalInterrupt(const ExternalInterrupt&) = delete;
    ExternalInterrupt& operator=(const ExternalInterrupt&) = delete;

    ExternalInterrupt(ExternalInterrupt&&) noexcept = default;
    ExternalInterrupt& operator=(ExternalInterrupt&&) noexcept = default;

public:


    void init();
    void setCallback(Edge triggerEdge, callback::Callback func);
    void handleISR() const;

private:
    void enableNVIC() const;
    void registerInstance();

    std::uint8_t pinNumber_;
    GPIO_TypeDef* port_;
    Edge edge_;

    std::array<callback::Callback, 2> callbacks_{};

public:
    static std::array<ExternalInterrupt*, 16> active_instances;
};

} // namespace exti
