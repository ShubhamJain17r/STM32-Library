#pragma once

#include <cstdint>

namespace reg {

// -------------------------------- helper functions ------------------------------------

constexpr uint32_t singleBitMask(uint8_t bit) noexcept {
	return (1U << bit);
}

constexpr uint32_t bitMask(uint8_t width) noexcept {
	return (1U << width) - 1UL;
}

constexpr uint32_t bitMask(uint8_t width, uint8_t shift) noexcept {
	return bitMask(width) << shift;
}

// ------------------------------- register operations ----------------------------------

inline uint32_t read(const volatile uint32_t &reg) noexcept {
	return reg;
}

inline void write(volatile uint32_t &reg, uint32_t value) noexcept {
	reg = value;
}

inline void writeORedValue(volatile uint32_t &reg, uint32_t value) noexcept {
	reg |= value;
}

inline void writeORedValue(volatile uint32_t &reg, uint32_t value,
		uint8_t shift) noexcept {
	reg |= (value << shift);
}

inline void reset(volatile uint32_t &reg) noexcept {
	reg = 0;
}

// ------------------------ single bit operations ------------------------

inline void setBit(volatile uint32_t &reg, uint8_t bit) noexcept {
	reg |= singleBitMask(bit);
}

inline void resetBit(volatile uint32_t &reg, uint8_t bit) noexcept {
	reg &= ~singleBitMask(bit);
}

inline bool readBit(const volatile uint32_t &reg, uint8_t bit) noexcept {
	return (reg & singleBitMask(bit)) != 0;
}

inline void toggleBit(volatile uint32_t &reg, uint8_t bit) noexcept {
	reg ^= singleBitMask(bit);
}

// ------------------------- multi bit operations --------------------------

inline void setBits(volatile uint32_t &reg, uint32_t mask) noexcept {
	reg |= mask;
}

inline void setBits(volatile uint32_t &reg, uint8_t width,
		uint8_t shift) noexcept {
	reg |= bitMask(width, shift);
}

inline void resetBits(volatile uint32_t &reg, uint32_t mask) noexcept {
	reg &= ~mask;
}

inline void resetBits(volatile uint32_t &reg, uint8_t width,
		uint8_t shift) noexcept {
	reg &= ~bitMask(width, shift);
}

// ------------------------- bit field operations --------------------------

template<typename T>
inline void setNBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		uint8_t width, T value) noexcept {
	resetBits(reg, width, startBit);
	writeORedValue(reg, value, startBit);
}

template<typename T>
inline void setBitValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 1, value);
}

template<typename T>
inline void setTwoBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 2, value);
}

template<typename T>
inline void setThreeBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 3, value);
}

template<typename T>
inline void setFourBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 4, value);
}

template<typename T>
inline void setFiveBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 5, value);
}

template<typename T>
inline void setSixBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 6, value);
}

template<typename T>
inline void setSevenBitFieldValue(volatile uint32_t &reg, uint8_t startBit,
		T value) noexcept {
	setNBitFieldValue(reg, startBit, 7, value);
}

} // reg namespace
