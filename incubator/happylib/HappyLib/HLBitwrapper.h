
#ifndef _HLBITWRAPPER_H
#define _HLBITWRAPPER_H

#ifndef _HLDEFS_H
#include "HLDefs.h"
#endif


//// BitWrapper class [DOCUMENTME] ////

class BitWrapper
{
private:
	class Bit
	{
	public:
		unsigned int & word;
		unsigned int const mask;
		inline Bit(unsigned int & word, unsigned int mask) : word(word), mask(mask) {}
		inline Bit & operator = (int v)
		{
			if (v) word |= mask;
			else word &= (HLUINTBITMASK ^ mask);
			return *this;
		}
		inline Bit & operator ^= (int v)
		{
			if (v) word ^= mask;
			return *this;
		}
		inline Bit & operator &= (int v)
		{
			if (!v) word &= (HLUINTBITMASK ^ mask);
			return *this;
		}
		inline Bit & operator |= (int v)
		{
			if (v) word |= mask;
			return *this;
		}
		inline operator int () const { return !!(word & mask); }
	};
	unsigned int * table;
public:
	BitWrapper(unsigned int * table) : table(table) {}
	void set(int b)
	{ table[b >> HLUINTSIZESHIFT] |= (1u << (b & HLUINTSIZEMASK)); }
	void clear(int b)
	{ table[b >> HLUINTSIZESHIFT] &= (HLUINTBITMASK ^ (1u << (b & HLUINTSIZEMASK))); }
	int test(int b) const
	{ return (table[b >> HLUINTSIZESHIFT] & (1u << (b & HLUINTSIZEMASK))); }
	Bit operator[] (int b)
	{ return Bit(table[b >> HLUINTSIZESHIFT], 1u << (b & HLUINTSIZEMASK)); }
	int operator[] (int b) const { return test(b); }
	unsigned int getBits(int i, int len) const
	{
		unsigned int val = 0;
		for (int b = i + len - 1; b >= i; b --)
		{
			val <<= 1;
			val |= (*this)[b];
		}
		return val;
	}
	void setBits(int i, int len, unsigned int val)
	{
		for (int b = i; b < i + len; b ++)
		{
			(*this)[b] = val & 1;
			val >>= 1;
		}
	}
};

class BitArrayWrapper	// wrap a subset of the bits of a BitWrapper as an array of Bitsets
{
private:
	class Bitset
	{
	public:
		BitWrapper & bitWrapper;
		int const offset;
		int const length;
		inline Bitset(BitWrapper & bitWrapper, int offset, int length)
			: bitWrapper(bitWrapper), offset(offset), length(length) {}
		inline Bitset & operator = (unsigned int val)
		{
			bitWrapper.setBits(offset, length, val);
			return *this;
		}
	};
	BitWrapper bitWrapper;
	int offset;
	int length;
public:
	BitArrayWrapper(unsigned int * table, int offset, int length)
		: bitWrapper(table), offset(offset), length(length) {}
	unsigned int operator[] (int i) const { return bitWrapper.getBits(offset + length * i, length); }
	Bitset operator[] (int i) { return Bitset(bitWrapper, offset + length * i, length); }
};

#endif
