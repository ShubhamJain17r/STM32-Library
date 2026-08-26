/**
 * @file registers.hpp
 * @brief Zero-overhead, type-safe register manipulation inline templates
 */

#pragma once

#include <cstdint>

namespace reg
{

/**
 * @brief Sets specific bits in a volatile register (reg |= mask).
 */
template<typename T, typename M>
inline void setBits(volatile T& reg, M mask) noexcept
{
    reg |= static_cast<T>(mask);
}

/**
 * @brief Clears specific bits in a volatile register (reg &= ~mask).
 */
template<typename T, typename M>
inline void clearBits(volatile T& reg, M mask) noexcept
{
    reg &= ~static_cast<T>(mask);
}

/**
 * @brief Modifies bits in a register (clears clearMask, sets setMask).
 */
template<typename T, typename M, typename V>
inline void modifyBits(volatile T& reg, M clearMask, V setMask) noexcept
{
    reg = (reg & ~static_cast<T>(clearMask)) | static_cast<T>(setMask);
}

/**
 * @brief Writes a value to a bitfield defined by a clear mask and bit position.
 */
template<typename T, typename M, typename V>
inline void writeField(volatile T& reg, M clearMask, V value, std::uint8_t pos) noexcept
{
    reg = (reg & ~static_cast<T>(clearMask)) | ((static_cast<T>(value) << pos) & static_cast<T>(clearMask));
}

/**
 * @brief Reads bits matching a mask from a volatile register.
 */
template<typename T, typename M>
inline T readBits(const volatile T& reg, M mask) noexcept
{
    return reg & static_cast<T>(mask);
}

/**
 * @brief Checks if all bits in mask are set in the register.
 */
template<typename T, typename M>
inline bool isBitSet(const volatile T& reg, M mask) noexcept
{
    return (reg & static_cast<T>(mask)) == static_cast<T>(mask);
}

/**
 * @brief Checks if any bit in mask is set in the register.
 */
template<typename T, typename M>
inline bool isAnyBitSet(const volatile T& reg, M mask) noexcept
{
    return (reg & static_cast<T>(mask)) != 0;
}

/**
 * @brief Writes a direct value into a volatile register.
 */
template<typename T, typename V>
inline void write(volatile T& reg, V value) noexcept
{
    reg = static_cast<T>(value);
}

/**
 * @brief Reads a direct value from a volatile register.
 */
template<typename T>
inline T read(const volatile T& reg) noexcept
{
    return reg;
}

} // namespace reg

