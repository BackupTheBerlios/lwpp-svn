
#ifndef _HLPLATFORM_H
#define _HLPLATFORM_H


//// Defines for porting issues ////

// This defines the following platform information:

// _HL_MSVC			-	compiling with M$ Visual C++
// _HL_GCC			-	compiling with gcc / g++
// _HL_CODEWARRIOR	-	compiling with CodeWarrior
// _HL_X86			-	running on Intel x86 architecture
// _HL_ALPHA		-	running on Alpha architecture
// _HL_WIN32		-	windows is available
// _HL_UNIX			-	unix is available
// _HL_32BIT		-	running on a 32-bit architecture
// _HL_64BIT		-	running on a 64-bit architecture
// _HL_SILLYENDIEN	-	bytes in a word are in Intel silly backwards order

// [NOTE] x86 architecture is singled out in order to use some asm in Visual C++ for mutexes, etc.
//        *nix system calls are generally fast enough, so asm isn't neccessary under those platforms.

#ifdef _MSC_VER			// check for M$ Visual C++
#define _HL_MSVC
#define _HL_WIN32
#ifdef _M_IX86
#define _HL_X86
#define _HL_32BIT
#endif	// _M_IX86
#else	// _MSC_VER
#ifdef __GNUC__			// check for gcc / g++
#define _HL_GCC
#ifdef __i386__
#define _HL_X86
#define _HL_32BIT
#endif	// __i386__
#ifdef __alpha__
#define _HL_ALPHA
#define _HL_64BIT
#endif	// __alpha_ev4__
#ifdef __unix__
#define _HL_UNIX
#endif	// __unix__
#else	// __GNUC__
#ifdef __MWERKS__		// check for CodeWarrior
#define _HL_CODEWARRIOR
#define _HL_32BIT		// [FIXMELATER] check for Jaguar
#else	// __MWERKS__
#error Unsupported compiler.
#endif	// __MWERKS__
#endif	// __GNUC__
#endif	// _MSC_VER

#ifdef _HL_X86
#define _HL_SILLYENDIEN
#endif

#ifdef _HL_UNIX
#define HLDIRECTORYDELIMITER "/"
#else
#define HLDIRECTORYDELIMITER "\\"
#endif	// _HL_UNIX
// [FIXME] what about mac?

#ifdef _HL_GCC
#define NEW_THROW throw(std::bad_alloc)
#define DELETE_THROW throw()
#else
#define NEW_THROW
#define DELETE_THROW
#endif


//// Define platform-independant types ////

#ifdef _HL_32BIT
typedef char			int8;	// 8 bit singed int
typedef unsigned char	uint8;	// 8 bit unsigned int
typedef short			int16;	// 16 bit singed int
typedef unsigned short	uint16;	// 16 bit unsigned int
typedef int				int32;	// 32 bit signed int
typedef unsigned int	uint32;	// 32 bit unsigned int
typedef unsigned int	uintp;	// unsigned int the size of a pointer
#else
#ifdef _HL_64BIT
typedef char			int8;	// 8 bit singed int
typedef unsigned char	uint8;	// 8 bit unsigned int
typedef short			int16;	// 16 bit singed int
typedef unsigned short	uint16;	// 16 bit unsigned int
typedef int				int32;	// 32 bit signed int
typedef unsigned int	uint32;	// 32 bit unsigned int
typedef unsigned long	uintp;	// unsigned int the size of a pointer
#else
#error Unsupported architecture.
#endif	// _HL_64BIT
#endif	// _HL_32BIT


//// Fill in some cross-platform gaps ////

#ifndef _HL_MSVC
inline int stricmp(char const * dst, char const * src)	// [FIXME] this causes problems with MinGW
{
	int f, l;
	do
	{
		if (((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z')) f -= ('A' - 'a');
		if (((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z')) l -= ('A' - 'a');
	} while (f && (f == l));
	return(f - l);
}
#else
#undef min
#undef max
template<class T> T const & min(T const & a, T const & b) { return a < b ? a : b; }
template<class T> T const & max(T const & a, T const & b) { return a > b ? a : b; }
#endif


//// Allow deprecated headers if defined ////

#ifdef HLDEPRECATEDHEADERS
#define HLIOSTREAM_H <iostream.h>
#define HLFSTREAM_H <fstream.h>
#define HLSTDLIB_H <stdlib.h>
#define HLSTDIO_H <stdio.h>
#define HLSTD
#define HLNOCREATE ios::nocreate
#else	// HLDEPRECATEDHEADERS
#define HLIOSTREAM_H <iostream>
#define HLFSTREAM_H <fstream>
#define HLSTDLIB_H <cstdlib>
#define HLSTDIO_H <cstdio>
#define HLSTD std
#define HLNOCREATE 0
#endif

#endif
