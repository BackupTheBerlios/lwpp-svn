
#ifndef _HLDEFS_H
#define _HLDEFS_H


//// Compile-time switches ////
// [NOTE] You can comment these out and use your compiler's command line to activate them.

//#define HLERRORS					// enables various run-time error checks (for testing)
//#define HLLOGS					// enables run-time logging macros (for testing)
//#define HLNEW						// enables the memory manager (for lots of small objects)
//#define HLNEWMT					// enables thread-safety in the memory manager
//#define HLNEWERRORS				// enables memory manager run-time error checks (for testing)
#ifndef HLLOGNAME
#define HLLOGNAME "C:\\LW75c\\HDI_LOG\\HDI_Log.txt"	// name of file to append logs to
#endif


//// Defines for porting issues ////

#include "HLPlatform.h"


//// Handy typedefs while we wait for C99 ////

typedef uint32 uint;
typedef uint8 byte;


//// Other stuff ////

#ifdef NULL							// C++ NULL; include this before someone else clobbers NULL
#undef NULL							// [NOTE] In C++, we use 0 instead of NULL, but this is here
#endif								//        so that the LW SDK will compile.
#define NULL 0


//// Optimization macros ////

#ifndef __assume
#define HLASSUME(expr) ((void)0)
#else
#define HLASSUME(expr) __assume(expr)
#endif
// [EXAMPLE] HLASSUME(a != 0);	// This can help the optimizer


//// Logging macros ////

#ifndef HLLOGS
#define LOG(msg) ((void)0)
#define COUT(expr) ((void)0)
#define CLOG(expr) ((void)0)
#define CLEARLOG ((void)0)
#else
#include HLIOSTREAM_H
#include HLFSTREAM_H
#define LOG(msg) { HLSTD::ofstream(HLLOGNAME, HLSTD::ios::app) << msg; }
#define COUT(expr) { HLSTD::cout << #expr << " = " << expr << HLSTD::endl; }
#define CLOG(expr) { HLSTD::ofstream(HLLOGNAME, HLSTD::ios::app) << #expr << " = " << expr << HLSTD::endl; }
#define CLEARLOG (HLSTD::ofstream(HLLOGNAME) << "")
#endif
// [EXAMPLE] LOG("Some message");
// [EXAMPLE] COUT(someExpression);
// [EXAMPLE] CLOG(someExpression);


//// Debugging macros ////

#ifndef HLERRORS
#define HLASSERT(expr, msg) HLASSUME(expr)
#else
#include HLIOSTREAM_H
#ifdef HLLOGS
#include HLFSTREAM_H
#define HLASSERT(expr, msg) { if (!(expr)) { HLSTD::ofstream(HLLOGNAME, HLSTD::ios::app) << "ERROR: " << msg << HLSTD::endl; *(int*)0 = 0; } }
#else
#define HLASSERT(expr, msg) { if (!(expr)) { HLSTD::cerr << "ERROR: " << msg << HLSTD::endl; *(int*)0 = 0; } }
#endif
#endif
// [EXAMPLE] HLASSERT(a == b, "Hey! " << a << " != " << b << "!");


//// Memory management ////

#include "HLNew.h"

#endif
