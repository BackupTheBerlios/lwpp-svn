
#ifndef _HLBITFIELD_H
#define _HLBITFIELD_H

#ifndef _HLDEFS_H
#include "HLDefs.h"
#endif
#include HLIOSTREAM_H

#define HLUINTSIZESHIFT 5			// log2 of the number of bits in an unsigned int
#define HLUINTSIZE 0x20				// number of bits in an unsigned int
#define HLUINTBYTES 0x04			// number of bytes in an unsigned int
#define HLUINTSIZEMASK 0x1F			// number of bits in an unsigned int minus one
#define HLUINTBITMASK 0xFFFFFFFFU	// largest value that fits in an unsigned int (all bits on)

//// Bitfield class [DOCUMENTME] ////

class Bitfield
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
	int _size;
public:
	Bitfield();
	Bitfield(int);
	Bitfield(Bitfield const &);
	Bitfield & operator = (Bitfield const &);
	Bitfield & operator ^= (Bitfield const &);
	Bitfield & operator &= (Bitfield const &);
	Bitfield & operator |= (Bitfield const &);
	Bitfield operator ~ () const { Bitfield b(*this); b.invertAll(); return b; }
	~Bitfield();
	void init(int);
	unsigned int size() const { return _size; }
	void set(int);
	void clear(int);
	int test(int) const;
	void setAll();
	void clearAll();
	void invertAll();
	static int bitsOnWord(unsigned int);
	int bitsOn() const;
	int bitsOnBefore(int) const;
	Bit operator[] (int b)
	{
		HLASSERT(b < _size, "Bitfield::operator[]: bit out of range!");
		return Bit(table[b >> HLUINTSIZESHIFT], 1u << (b & HLUINTSIZEMASK));
	}
	int operator[] (int b) const { return test(b); }
	HLSTD::ostream & display(HLSTD::ostream &) const;
	unsigned int getBits(int i, int len) const;
	void setBits(int i, int len, unsigned int val);
};

inline HLSTD::ostream & operator << (HLSTD::ostream & out, Bitfield const & bf) { return bf.display(out); }

inline Bitfield operator ^ (Bitfield a, Bitfield const & b) { return a ^= b; }
inline Bitfield operator & (Bitfield a, Bitfield const & b) { return a &= b; }
inline Bitfield operator | (Bitfield a, Bitfield const & b) { return a |= b; }

inline void Bitfield::set(int b)
{
	HLASSERT(b < _size, "Bitfield::set: bit out of range!");
	table[b >> HLUINTSIZESHIFT] |= (1u << (b & HLUINTSIZEMASK));
}

inline void Bitfield::clear(int b)
{
	HLASSERT(b < _size, "Bitfield::clear: bit out of range!");
	table[b >> HLUINTSIZESHIFT] &= (HLUINTBITMASK ^ (1u << (b & HLUINTSIZEMASK)));
}

inline int Bitfield::test(int b) const
{
	HLASSERT(b < _size, "Bitfield::test: bit out of range!");
	return (table[b >> HLUINTSIZESHIFT] & (1u << (b & HLUINTSIZEMASK))) != 0;
}


//// Bitsetfield class [DOCUMENTME] ////

class Bitsetfield
{
private:
	class Bitset
	{
	public:
		Bitfield & bitfield;
		int const offset;
		int const length;
		inline Bitset(Bitfield & bitfield, int offset, int length)
			: bitfield(bitfield), offset(offset), length(length) {}
		inline Bitset & operator = (unsigned int val)
		{
			bitfield.setBits(offset, length, val);
			return *this;
		}
		operator unsigned int() const { return bitfield.getBits(offset, length); }
	};
	Bitfield bitfield;
public:
	const int count;
	const int length;
	Bitsetfield(int count, int length)
		: bitfield(count * length), count(count), length(length) {}
	unsigned int operator[] (int i) const { return bitfield.getBits(length * i, length); }
	Bitset operator[] (int i) { return Bitset(bitfield, length * i, length); }
};

#endif
