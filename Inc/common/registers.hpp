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

inline void setShiftedValue(volatile std::uint32_t &reg, std::uint32_t value) noexcept {
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

inline bool readBit(const volatile std::uint32_t &reg, std::uint8_t bit) noexcept {
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

inline std::uint32_t readBits(const volatile std::uint32_t &reg, std::uint8_t width, std::uint8_t shift) noexcept
{
	return (reg & bitMask(width, shift) >> shift);
}

// ------------------------- bit field operations --------------------------

template<typename T>
inline void setNBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		std::uint8_t width, T value) noexcept {
	resetBits(reg, width, startBit);
	setShiftedValue(reg, value, startBit);
}

template<typename T>
inline void setBitValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 1, value);
}

template<typename T>
inline void setTwoBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 2, value);
}

template<typename T>
inline void setThreeBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 3, value);
}

template<typename T>
inline void setFourBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 4, value);
}

template<typename T>
inline void setFiveBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 5, value);
}

template<typename T>
inline void setSixBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 6, value);
}

template<typename T>
inline void setSevenBitFieldValue(volatile std::uint32_t &reg, std::uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 7, value);
}

} // reg namespace
