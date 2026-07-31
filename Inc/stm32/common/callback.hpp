#pragma once

#include <cstdint>

using Callback = void(*)();

constexpr Callback NullCallback = nullptr;
