#pragma once

#include <cstdint>

namespace reg
{

// -------------------------------- helper functions ------------------------------------

inline uint32_t singleBitMask(uint8_t bit)
{
	return (1U << bit);
}

inline uint32_t bitMask(uint8_t width)
{
	return (1U << width) - 1UL;
}

inline uint32_t bitMask(uint8_t width, uint8_t shift)
{
	return bitMask(width) << shift;
}

// ------------------------------- register operations ----------------------------------

inline uint32_t read(const volatile uint32_t& reg)
{
	return reg;
}

inline void write(volatile uint32_t& reg, uint32_t value)
{
	reg = value;
}

inline void writeORedValue(volatile uint32_t& reg, uint32_t value)
{
	reg |= value;
}

inline void writeORedValue(volatile uint32_t& reg, uint32_t value, uint8_t shift)
{
	reg |= bitMask(value, shift);
}

inline void reset(volatile uint32_t& reg)
{
	reg = 0;
}

// ------------------------ single bit operations ------------------------

inline void setBit(volatile uint32_t& reg, uint8_t bit)
{
	reg |= singleBitMask(bit);
}

inline void resetBit(volatile uint32_t& reg, uint8_t bit)
{
	reg &= ~singleBitMask(bit);
}

inline bool readBit(const volatile uint32_t& reg, uint8_t bit)
{
	return (reg & singleBitMask(bit)) != 0;
}

// ------------------------- multi bit operations --------------------------

inline void setBits(volatile uint32_t& reg, uint32_t mask)
{
	reg |= mask;
}

inline void setBits(volatile uint32_t& reg, uint8_t width, uint8_t shift)
{
	reg |= bitMask(width, shift);
}

inline void resetBits(volatile uint32_t& reg, uint32_t mask)
{
	reg &= ~mask;
}

inline void resetBits(volatile uint32_t& reg, uint8_t width, uint8_t shift)
{
	reg &= ~bitMask(width, shift);
}

// ------------------------- bit field operations --------------------------

inline void setNBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint8_t width, uint32_t value)
{
	resetBits(reg, width, startBit);
	writeORedValue(reg, value, startBit);
}

inline void setBitValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 1, value);
}

inline void setTwoBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 2, value);
}

inline void setThreeBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 3, value);
}

inline void setFourBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 4, value);
}

inline void setFiveBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 5, value);
}

inline void setSixBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 6, value);
}

inline void setSevenBitFieldValue(volatile uint32_t& reg, uint8_t startBit, uint32_t value)
{
	setNBitFieldValue(reg, startBit, 7, value);
}

} // reg namespace
