
#ifndef _HLIOUTILS_H
#define _HLIOUTILS_H

inline unsigned short get2i(HLSTD::istream & in)	// endian safe; Intel byte order
{ char b0, b1; in.get(b0); in.get(b1); return (uint)b0 | ((uint)b1 << 8); }
inline unsigned short get2(HLSTD::istream & in)	// endian safe; Sensible byte order
{ char b0, b1; in.get(b0); in.get(b1); return (uint)b1 | ((uint)b0 << 8); }
inline unsigned int get4i(HLSTD::istream & in)	// endian safe; Intel byte order
{
	char b0, b1, b2, b3; in.get(b0); in.get(b1); in.get(b2); in.get(b3);
	return (uint)b0 | ((uint)b1 << 8) | ((uint)b2 << 16) | ((uint)b3 << 24);
}
inline unsigned int get4(HLSTD::istream & in)	// endian safe; Sensible byte order
{
	char b0, b1, b2, b3; in.get(b0); in.get(b1); in.get(b2); in.get(b3);
	return (uint)b3 | ((uint)b2 << 8) | ((uint)b1 << 16) | ((uint)b0 << 24);
}
inline void put2i(HLSTD::ostream & out, unsigned short b)	// endian safe; Intel byte order
{ out.put(byte(b & 0xff)); out.put(byte((b >> 8) & 0xff)); }
inline void put2(HLSTD::ostream & out, unsigned short b)	// endian safe; Sensible byte order
{ out.put(byte((b >> 8) & 0xff)); out.put(byte(b & 0xff)); }
inline void put4i(HLSTD::ostream & out, unsigned short b)	// endian safe; Intel byte order
{ out.put(byte(b & 0xff)); out.put(byte((b >> 8) & 0xff)); out.put(byte((b >> 16) & 0xff)); out.put(byte((b >> 24) & 0xff)); }
inline void put4(HLSTD::ostream & out, unsigned short b)	// endian safe; Sensible byte order
{ out.put(byte((b >> 24) & 0xff)); out.put(byte((b >> 16) & 0xff)); out.put(byte((b >> 8) & 0xff)); out.put(byte(b & 0xff)); }
#ifdef _WIN32
inline void Swap4(char * d, int n)
{ while (n --) { Swap(d[0], d[3]); Swap(d[1], d[2]); d += 4; } }
#define SwapM4(p) Swap4((char*)p,1)
#else
#define SwapM4(p)
#endif

#endif

