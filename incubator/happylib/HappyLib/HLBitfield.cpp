
#include <string.h>

#include "HLBitfield.h"

Bitfield::Bitfield() : table(0)
{}

Bitfield::Bitfield(int s) : _size(s)
{
	int intSize = (s + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	table = new unsigned int[intSize];
	memset(table, (unsigned char)0, intSize*sizeof(unsigned int));
}

Bitfield::Bitfield(Bitfield const & bf) : _size(bf._size)
{
	int intSize = (_size + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	table = new unsigned int[intSize];
	memcpy(table, bf.table, intSize*sizeof(unsigned int));
}

Bitfield & Bitfield::operator = (Bitfield const & bf)
{
	if (&bf == this) return *this;
	delete[] table;
	_size = bf._size;
	int intSize = (_size + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	_size = intSize << HLUINTSIZESHIFT;
	table = new unsigned int[intSize];
	memcpy(table, bf.table, intSize*sizeof(unsigned int));
	return *this;
}

Bitfield & Bitfield::operator ^= (Bitfield const & bf)
{
	HLASSERT(bf._size == _size, "Bitfield::operator^=: Bitfield sizes do not match!");
	int intSize = (_size + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	for (int i = 0; i < intSize; i ++) table[i] ^= bf.table[i];
	return *this;
}

Bitfield & Bitfield::operator &= (Bitfield const & bf)
{
	HLASSERT(bf._size == _size, "Bitfield::operator&=: Bitfield sizes do not match!");
	int intSize = (_size + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	for (int i = 0; i < intSize; i ++) table[i] &= bf.table[i];
	return *this;
}

Bitfield & Bitfield::operator |= (Bitfield const & bf)
{
	HLASSERT(bf._size == _size, "Bitfield::operator|=: Bitfield sizes do not match!");
	int intSize = (_size + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	for (int i = 0; i < intSize; i ++) table[i] |= bf.table[i];
	return *this;
}

Bitfield::~Bitfield()
{
	delete[] table;
}

void Bitfield::init(int s)
{
	if (_size != s)
	{
		_size = s;
		delete[] table;
		int intSize = (s + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
		table = new unsigned int[intSize];
		memset(table, (unsigned char)0, intSize*sizeof(unsigned int));
	}
	else
	{
		int intSize = (s + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
		memset(table, (unsigned char)0, intSize*sizeof(unsigned int));
	}
}

int Bitfield::bitsOnWord(unsigned int bits)
{
	int ones = 0;
	for (; bits; bits = bits & (bits-1)) ones ++;
	return ones;
	/*
	[NOTE] this next routine is better when most of the bits are actually on
	unsigned int const k5 = 0x55555555, k3 = 0x33333333, kf0 = 0xf0f0f0f, kff = 0xff00ff;
	j -= ((j >> 1) & k5);
	j = (j & k3) + ((j >> 2) & k3);
	j = (j & kf0) + ((j >> 4) & kf0);
	j += j >> 8;
	return (j + (j >> 16)) & 0x1f;
	*/
}

int Bitfield::bitsOn() const
{
	int ones = 0;
	int i = (_size - 1) >> HLUINTSIZESHIFT;	// word that last bit is in
	for (int t = 0; t <= i; t ++) ones += bitsOnWord(table[t]);
	return ones;
}

int Bitfield::bitsOnBefore(int b) const
{
	if (!b) return 0;
	int ones = 0;
	int i = b >> HLUINTSIZESHIFT;
	for (int t = 0 ; t < i; t ++) ones += bitsOnWord(table[t]);	// ones before current word
	i = table[i];							// current word's bits
	i &= (1u << (b & HLUINTSIZEMASK)) - 1;	// mask away bits after b (and b itself)
	return ones + bitsOnWord(i);			// add on current word's ones
}

HLSTD::ostream & Bitfield::display(HLSTD::ostream & out) const
{
	for (int i = 0; i < _size; i ++) out << (test(i) ? "1" : "0");
	return out;
}

void Bitfield::clearAll()
{
	int i = (_size - 1) >> HLUINTSIZESHIFT;	// word that last bit is in
	for (int t = 0; t < i; t ++) table[t] = 0;
}

void Bitfield::setAll()
{
	int i = (_size - 1) >> HLUINTSIZESHIFT;	// word that last bit is in
	for (int t = 0; t < i; t ++) table[t] = HLUINTBITMASK;
	table[i] = (1u << (_size & HLUINTSIZEMASK)) - 1;
}

void Bitfield::invertAll()
{
	int i = (_size - 1) >> HLUINTSIZESHIFT;	// word that last bit is in
	for (int t = 0; t < i; t ++) table[t] ^= HLUINTBITMASK;
	table[i] ^= (1u << (_size & HLUINTSIZEMASK)) - 1;
}

unsigned int Bitfield::getBits(int i, int len) const
{
	unsigned int val = 0;
	for (int b = i + len - 1; b >= i; b --)
	{
		val <<= 1;
		val |= (*this)[b];
	}
	return val;
}

void Bitfield::setBits(int i, int len, unsigned int val)
{
	for (int b = i; b < i + len; b ++)
	{
		(*this)[b] = val & 1;
		val >>= 1;
	}
}
