
#ifndef _HLNEW_H
#define _HLNEW_H


#ifdef HLNEWMT
#ifndef HLNEW
#define HLNEW
#endif
#endif

#ifdef HLNEW

#include "HLPlatform.h"
#include HLIOSTREAM_H
#include <malloc.h>
#include <new.h>

#define MEMBEAMSIZE 1048576		// size of beams
#define MEMBEAMS 4096			// number of beams (MEMBEAMS * MEMBEAMSIZE <= 4 gigs)
#define MAXMEMSIZE 2048			// handles structures up to this size
#define MEMBLOCKSIZE 16384		// size of blocks (beams contain many blocks) must be power of 2; should be small multiple of page size


//// macro for classes to use system memory management ////

#define DONTMEMMANAGE \
inline void * operator new(size_t s) NEW_THROW { return malloc(s); } \
inline void * operator new[](size_t s) NEW_THROW { return malloc(s); } \
inline void operator delete(void * ptr) DELETE_THROW { free(ptr); } \
inline void operator delete[](void * ptr) DELETE_THROW { free(ptr); }


int newHandler(size_t);

class MemoryManager
{
public:
	class MemoryNode
	{
		friend class MemoryManager;
	private:
		MemoryNode * prev;		// pointer to prev node
		MemoryNode * next;		// pointer to next node
		uint8 * ptr;			// pointer to usable memory (objects)
		uint32 size;			// size of objects
		uint32 objects;			// number of objects
		uint32 * oat;			// object allocation table
		uint32 oatSize;			// size of oat (words)
		uint32 freeOat;			// guess of first free oat word (never overguesses)
		uint32 freeBit;			// guess of first free oat bit (never overguesses)
	public:
		void init(uint32);
		inline void * allocEmpty();
		inline void freePtr(void *);
		inline void freeListify();
		void validate(const void *) const;
#ifdef HLNEWERRORS
		uint32 clear();
#endif	// HLNEWERRORS
		DONTMEMMANAGE;
	};
	class MemoryBeam
	{
	private:
		uint8 memory[MEMBEAMSIZE];
	public:
		uint32 owns(void const *) const;
		MemoryBeam();
		~MemoryBeam();
		DONTMEMMANAGE;
	};
	class FreeListHeader
	{
		friend class MemoryManager;
	private:
		FreeListHeader * next;
		uint32 size;
	public:
		void init(uint32);
		void freeListify();
		~FreeListHeader();
	};
	static MemoryNode * head[MAXMEMSIZE+1];			// array of memory node pointers
	static MemoryNode * tail[MAXMEMSIZE+1];			// array of memory node pointers
	static MemoryNode * firstFree[MAXMEMSIZE+1];	// array of first free memory node pointers
	static FreeListHeader * freeList[MAXMEMSIZE+1];	// array of freeLists of objects
	static MemoryNode * freeNodeList;				// freeList of memory nodes
	static MemoryBeam * beam[MEMBEAMS];				// array of memory beam pointers
	static uint32 beams;
	static uint32 initialized;
	static uintp MemoryUsed;						// used for determining when to cleanUp
	static uint32 previousOwner;					// beam that last matched pointer
	static uint32 owns(void const *);
	static void * alloc(size_t);
	static void unAlloc(void *);
	static void * freeListAlloc(size_t);
	static void freeListUnAlloc(void *);
	static void validate(const void *);
	static int freeUnusedNodes();
	static void initialize();						// anytime constructor
	static void unInitialize();						// anytime destructor
	static inline MemoryNode * freeNode();
	DONTMEMMANAGE;
};


//// Global new, new[], delete and delete[] operators ////

inline void * operator new(size_t s) NEW_THROW
{
	return MemoryManager::alloc(s);
}

inline void * operator new[](size_t s) NEW_THROW
{
	// If the class of the array has a destructor, the compiler puts a header in front
	// of the array.  The header is as follows:

	// Intel under VC++6 or GCC:
	//		struct ArrayHeader
	//		{
	//			int arrayCount;
	//		};

	// Sparc under GCC:
	//		struct ArrayHeader
	//		{
	//			int arrayCount;
	//			int pad;
	//		};

	// On a Sparc machine, the object after the pad must start on an 8-byte boundary,
	// since the class may contain a double, and we don't want bus errors.

	// On an Intel machine, this is moot, except that there may be a speed penalty for misaligment.

#ifdef _HL_X86
	return MemoryManager::alloc(s);
#else
	return MemoryManager::alloc(((s+7)/8)*8);
#endif
}

inline void operator delete(void * ptr) DELETE_THROW
{
	MemoryManager::unAlloc(ptr);
}

inline void operator delete[](void * ptr) DELETE_THROW
{
	MemoryManager::unAlloc(ptr);
}

//// macro for classes to use freeListed memory management ////

// [NOTE] Empirical evidence shows that FreeLists are actually SLOWER than the MemoryManager!!
//        Thus they are not actually used.  They are here for testing purposes.

#define HLFREELISTED \
inline void * operator new(size_t s) NEW_THROW { return MemoryManager::freeListAlloc(s); } \
inline void operator delete(void * ptr) DELETE_THROW { MemoryManager::unAlloc(ptr); }

#endif	// HLNEW


#ifndef DONTMEMMANAGE
#define DONTMEMMANAGE ;
#endif

#ifndef HLFREELISTED
#define HLFREELISTED ;
#endif


#endif
