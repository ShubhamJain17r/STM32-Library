#pragma once

#include "stm32f446xx.h"
#include <cstdint>

namespace gpio
{

enum class Port : uint8_t
{
    A,
    B,
    C,
    D,
    E,
    H
};

struct Pin
{
    Port port;
    uint8_t number;
};

constexpr Pin PA0{Port::A, 0};

} // namespace gpio
