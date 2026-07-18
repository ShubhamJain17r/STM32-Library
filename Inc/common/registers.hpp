#pragma once

#include <cstdint>

namespace reg {

// -------------------------------- helper functions ------------------------------------

constexpr std::uint32_t singleBitMask(std::uint8_t bit) noexcept {
	return (1U << bit);
}

constexpr std::uint32_t bitMask(std::uint8_t width) noexcept {
	return (1U << width) - 1UL;
}

constexpr std::uint32_t bitMask(std::uint8_t width, std::uint8_t shift) noexcept {
	return bitMask(width) << shift;
}

// ------------------------------- register operations ----------------------------------

inline std::uint32_t read(const volatile std::uint32_t &reg) noexcept {
	return reg;
}

inline void write(volatile std::uint32_t &reg, std::uint32_t value) noexcept {
	reg = value;
}

inline void setShiftedValue(volatile std::uint32_t &reg,
		std::uint32_t value) noexcept {
	reg |= value;
}

inline void setShiftedValue(volatile std::uint32_t &reg, std::uint32_t value,
		std::uint8_t shift) noexcept {
	reg |= (value << shift);
}

inline void reset(volatile std::uint32_t &reg) noexcept {
	reg = 0;
}

// ------------------------ single bit operations ------------------------

inline void setBit(volatile std::uint32_t &reg, std::uint8_t bit) noexcept {
	reg |= singleBitMask(bit);
}

inline void resetBit(volatile std::uint32_t &reg, std::uint8_t bit) noexcept {
	reg &= ~singleBitMask(bit);
}

inline bool readBit(const volatile std::uint32_t &reg,
		std::uint8_t bit) noexcept {
	return (reg & singleBitMask(bit)) != 0;
}

inline void toggleBit(volatile std::uint32_t &reg, std::uint8_t bit) noexcept {
	reg ^= singleBitMask(bit);
}

// ------------------------- multi bit operations --------------------------

inline void setBits(volatile std::uint32_t &reg, std::uint32_t mask) noexcept {
	reg |= mask;
}

inline void setBits(volatile std::uint32_t &reg, std::uint8_t width,
		std::uint8_t shift) noexcept {
	reg |= bitMask(width, shift);
}

inline void resetBits(volatile std::uint32_t &reg, std::uint32_t mask) noexcept {
	reg &= ~mask;
}

inline void resetBits(volatile std::uint32_t &reg, std::uint8_t width,
		std::uint8_t shift) noexcept {
	reg &= ~bitMask(width, shift);
}

inline std::uint32_t readBits(const volatile std::uint32_t &reg,
		std::uint8_t width, std::uint8_t shift) noexcept {
	return (reg & bitMask(width, shift)) >> shift;
}

// ------------------------- bit field operations --------------------------

template<std::uint8_t Width, typename T>
inline void setBitField(volatile std::uint32_t &reg, std::uint8_t shift,
		T value) noexcept {
	resetBits(reg, Width, shift);
	setShiftedValue(reg, static_cast<std::uint32_t>(value), shift);
}

} // reg namespace
