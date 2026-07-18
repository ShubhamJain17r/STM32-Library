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
    // Accept a configuration pin handle directly
    constexpr ExternalInterrupt(const gpio::Pin& pin, Edge edge)
        : pinNumber_(pin.getPinNumber()), port_(pin.getPort()), edge_(edge) {
        register_instance();
    }

    void init();

    // Explicit API to bind functions to explicit edges
    void setCallback(Edge triggerEdge, callback::Callback func);
    void handleISR() const;

private:
    void enableNVIC() const;
    constexpr void register_instance();

    std::uint8_t pinNumber_;
    GPIO_TypeDef* port_;
    Edge edge_;

    // Explicit allocations for distinct edge triggers
    std::array<callback::Callback, 2> callbacks_{};

public:
    static std::array<ExternalInterrupt*, 16> active_instances;
};

} // namespace exti
