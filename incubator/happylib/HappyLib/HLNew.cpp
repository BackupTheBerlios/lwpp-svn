
#include <HLDefs.h>		// [NOTE] includes HLNew.h


#ifdef HLNEW			// only implement HLNEW if we've included it

#include <HLBitfield.h>	// for the bitsOnWord function
#ifdef HLNEWERRORS
#include <stdlib.h>
#endif
#include <string.h>
#ifdef HLNEWMT
#include <HLMutex.h>
#endif

uint32 MemoryManager::beams = 0;
uint32 MemoryManager::initialized = 0;
uintp MemoryManager::MemoryUsed = 0;
uint32 MemoryManager::previousOwner = 0;
MemoryManager::MemoryNode * MemoryManager::head[MAXMEMSIZE+1];
MemoryManager::MemoryNode * MemoryManager::tail[MAXMEMSIZE+1];
MemoryManager::MemoryNode * MemoryManager::firstFree[MAXMEMSIZE+1];
MemoryManager::FreeListHeader * MemoryManager::freeList[MAXMEMSIZE+1];
MemoryManager::MemoryNode * MemoryManager::freeNodeList = 0;
MemoryManager::MemoryBeam * MemoryManager::beam[MEMBEAMS];
#ifdef HLNEWMT
Mutex mutex;
#endif


//// MemoryNode implementation ////

void MemoryManager::MemoryNode::validate(const void * p) const
{
	uint32 object, oatNum, bit;
	object = (((uintp)p - (uintp)ptr) / size);

	HLASSERT(object < objects, "MemoryNode::validate: Node does not contain pointer!");
	HLASSERT((uintp)ptr + object * size == (uintp)p, "MemoryNode::validate: Bad pointer alignment!");

	bit = object & HLUINTSIZEMASK;
	oatNum = object >> HLUINTSIZESHIFT;

	HLASSERT(oat[oatNum] & (1 << bit),
		"MemoryNode::validate: Pointer no longer allocated!" << HLSTD::endl
		<< "SIZE: " << size << " NODE: " << this << " OBJECT: " << oatNum << " BIT: " << bit << " BAT: " << oat[oatNum] << HLSTD::endl
		<< (void*)ptr << " <= " << p << " < " << (void*)&ptr[size * objects]);
}

void MemoryManager::MemoryNode::init(uint32 s)
{
	next = 0;
	prev = 0;
	size = s;
	freeOat = 0;
	freeBit = 0;
	uint32 fillBits;

	// compute the number of words that *this takes (header size)
	uint32 wordsThis = (sizeof(*this) + HLUINTBYTES - 1) / HLUINTBYTES;
	uint32 thisSize = HLUINTBYTES * wordsThis;

	// compute number of objects that definitely fit
	objects = (8 * (MEMBLOCKSIZE - thisSize) - HLUINTSIZEMASK) / (1 + 8 * size);
	if (size > HLUINTSIZE)	// shall we try to fit another in there
	{
		// these next two lines sometimes fit another object in, but take some time
		uint32 total2 = size * (objects+1) + HLUINTBYTES * (((objects+1) + HLUINTSIZEMASK) / HLUINTSIZE) + thisSize;
		if (total2 <= MEMBLOCKSIZE) objects ++;
	}

	oatSize = (objects + HLUINTSIZEMASK) >> HLUINTSIZESHIFT;
	oat = (uint32*)this + wordsThis;
	memset(oat, (uint8)0, oatSize*HLUINTBYTES);
	// set the pad bits at the end of the oat
	fillBits = (oatSize << HLUINTSIZESHIFT) - objects;
	if (fillBits) oat[oatSize - 1] = HLUINTBITMASK ^ ((1 << (HLUINTSIZE - fillBits)) - 1);

	ptr = (uint8*)(oat + oatSize);	// compute where the objects start
}

#ifdef HLNEWERRORS
uint32 MemoryManager::MemoryNode::clear()
{
	// clear the pad bits at the end of the oat
	uint32 fillBits = (oatSize << HLUINTSIZESHIFT) - objects;
	if (fillBits) oat[oatSize - 1] ^= HLUINTBITMASK ^ ((1 << (HLUINTSIZE - fillBits)) - 1);
	uint32 uncleared = 0;
	for (uint32 t = 0; t < oatSize; t++) uncleared += Bitfield::bitsOnWord(oat[t]);
	return uncleared;
}
#endif


//// MemoryBeam implementation ////

uint32 MemoryManager::MemoryBeam::owns(void const * ptr) const
{
	return ((uintp)ptr >= (uintp)memory && (uintp)ptr < (uintp)&memory[MEMBEAMSIZE]);
}

MemoryManager::MemoryBeam::MemoryBeam()
{
#ifdef HLNEWERRORS
	HLASSERT(!(MEMBLOCKSIZE & (MEMBLOCKSIZE-1)),
		"MEMBLOCKSIZE must be a power of 2.  Can't you read comments?");
#endif
	// locate the first usable node
	MemoryNode * ptr = (MemoryNode *)(((uintp)this + MEMBLOCKSIZE - 1) & ~(MEMBLOCKSIZE - 1));
	// add the usable nodes to the freelist
	for (; (uintp)ptr + MEMBLOCKSIZE <= (uintp)(this + 1); ptr = (MemoryNode *)((uintp)ptr + MEMBLOCKSIZE))	// loop through usable nodes
	{
		ptr->freeListify();	// add the node to the freelist
	}
}

MemoryManager::MemoryBeam::~MemoryBeam() {}


//// FreeListHeader implementation ////

MemoryManager::FreeListHeader::~FreeListHeader()
{
	delete next;
}


//// MemoryManager implementation ////

uint32 MemoryManager::owns(void const * ptr)
{
	// find owner beam, first checking the previously matching owner
	if (!initialized) return 0;
	if (!ptr) return 0;
	uint32 b;
	for (b = previousOwner; b < beams; b --) if (beam[b]->owns(ptr))
	{
		previousOwner = b;
		return 1;
	}
	for (b = previousOwner + 1; b < beams; b ++) if (beam[b]->owns(ptr))
	{
		previousOwner = b;
		return 1;
	}
	return 0;
}

void * MemoryManager::alloc(size_t s)
{
	if (s > MAXMEMSIZE)
	{
		return malloc(s);	// too big to handle in nodes
	}
	if (!s)
	{
		return 0;			// wants no bytes!
	}

#ifdef HLNEWMT
	mutex.lock();
#endif

	if (!initialized) initialize();

	// ask existing nodes for the pointer
	MemoryNode * n;
	void * ptr = 0;
	for (n = firstFree[s]; n; n = n->next)
	{
		if ((ptr = n->allocEmpty())) break;
	}

	// make a new node if no pointer returned
	if (!ptr)
	{
		n = freeNode();		// get a node from the freeLists
		n->init(s);			// tell it what size to handle
		if (!MemoryManager::head[s])
		{
			MemoryManager::head[s] = MemoryManager::tail[s] = n;
		}
		else
		{
			n->prev = MemoryManager::tail[s];
			MemoryManager::tail[s] = MemoryManager::tail[s]->next = n;
		}
		ptr = n->allocEmpty();
	}

	// set the firstFree pointer
	firstFree[s] = n;

#ifdef HLNEWMT
	mutex.unlock();
#endif

	return ptr;
}

void MemoryManager::unAlloc(void * ptr)
{
	if (!ptr) return;

#ifdef HLNEWMT
	mutex.lock();
#endif
	
	// do we own the memory?
	if (!owns(ptr))
	{
		free(ptr);
#ifdef HLNEWMT
		mutex.unlock();
#endif
		return;
	}
	if (!initialized) initialize();
	MemoryNode * node = (MemoryNode *)((uintp)ptr & ~(MEMBLOCKSIZE-1));
	node->freePtr(ptr);
	if (!MemoryUsed) unInitialize();	// our job here is done

#ifdef HLNEWMT
	mutex.unlock();
#endif
}

void * MemoryManager::freeListAlloc(size_t s)
{
	if (!initialized) initialize();
	if (!s) return 0;					// wants no bytes!

	FreeListHeader * header = 0;

	if (freeList[s])	// we already got one
	{
		header = freeList[s];
		freeList[s] = freeList[s]->next;
	}
	else				// got to make a new FreeListHeader
	{
		header = (FreeListHeader *)alloc(s + sizeof(FreeListHeader));
		header->init(s);
	}

	return (void*)(header + 1);		// return the address after the header
}

void MemoryManager::freeListUnAlloc(void * ptr)
{
	if (!initialized) initialize();
	if (!ptr) return;
	((FreeListHeader*)ptr - 1)->freeListify();	// freeListify the header
}

void MemoryManager::validate(const void * ptr)
{
	if (!initialized) initialize();
	if (!ptr) return;

	// do we own the memory?
	HLASSERT(owns(ptr), "MemoryManager::validate: MemoryManager does not own pointer!");

	MemoryNode const * node = (MemoryNode const *)((uintp)ptr & ~(MEMBLOCKSIZE-1));
	node->validate(ptr);
}

int MemoryManager::freeUnusedNodes()
{
	if (!initialized) initialize();
	uint32 collected = 0;

	// clear the freelists
	for (uint32 f = 1; f <= MAXMEMSIZE; f ++) if (freeList[f])
	{
		delete freeList[f];	// recursively deletes
		freeList[f] = 0;
		collected = 1;
	}

	// [FIXME] free unused nodes
	
	// [FIXME] release empty memory beams

	return collected;
}

void MemoryManager::initialize()
{
	if (initialized) return;

	initialized = 1;
	memset(head, (uint8)0, sizeof(MemoryNode*)*(MAXMEMSIZE+1));
	memset(tail, (uint8)0, sizeof(MemoryNode*)*(MAXMEMSIZE+1));
	memset(firstFree, (uint8)0, sizeof(MemoryNode*)*(MAXMEMSIZE+1));
	memset(freeList, (uint8)0, sizeof(uint8*)*(MAXMEMSIZE+1));
	memset(beam, (uint8)0, sizeof(MemoryBeam*)*(MEMBEAMS));
	//_set_new_handler(newHandler);
	//_set_new_mode(1);
}

void MemoryManager::unInitialize()
{
	if (!initialized) return;

	// clear the freelists
	for (uint32 f = 1; f <= MAXMEMSIZE; f ++) if (freeList[f])
	{
		delete freeList[f];	// recursively deletes
		freeList[f] = 0;
	}

	// check for memory leaks
#ifdef HLNEWERRORS
	for (uint32 s = 1; s <= MAXMEMSIZE; s ++)
	{
		uint32 uncleared = 0;
		for (MemoryNode * n = head[s]; n; n = n->next)
		{
			uncleared += n->clear();	// test to see that node is clear
		}
		if (uncleared)	// this shouldn't happen, so report an error
		{	
			HLSTD::cerr << "ERROR: MemoryManager::unInitialize: " << uncleared << " object"
				<< ((uncleared > 1) ? "s" : "") << " of size " << s << " have not been freed!" << HLSTD::endl;
		}
	}
#endif

	// delete the beams
	for (uint32 t = 0; t < MEMBEAMS; t ++) delete beam[t];

	//_set_new_mode(0);
	//_set_new_handler(0);

	initialized = 0;
}

int newHandler(size_t size)	// [FIXME][LATER] this isn't properly enabled on non-VC systems
{
	// try to free up some space
	if (MemoryManager::freeUnusedNodes()) return 1;

	// try something else

	// no luck.  oh no!
	HLSTD::cerr << "ERROR: Out of memory! Argh!" << HLSTD::endl;
	return 0;
}

void * MemoryManager::MemoryNode::allocEmpty()
{
	uint32 oatNum=0, bit=0, t, bitPos=0, objNum=0;
	
	// see if freebat has free bits
	if (oat[freeOat] != HLUINTBITMASK)
	{
		int bitPos = 1 << freeBit;
		int bit;
		// see if freebit is free
		if (!(oat[freeOat] & bitPos)) bit = freeBit;
		else for (bit = 0, bitPos = 1; oat[freeOat] & bitPos; bit ++, bitPos <<= 1);
		// set bit
		oat[freeOat] |= bitPos;
		int objNum = (freeOat << HLUINTSIZESHIFT) | bit;
		// guess next freebit
		if (freeBit < HLUINTSIZE-2) freeBit++;
		MemoryUsed += size;
		// return corresponding pointer
		return (void *)&ptr[objNum * size];
	}

	// look through the rest of the oats
	for (t = freeOat + 1; t < oatSize; t++)
	{
		if (oat[t] != HLUINTBITMASK)
		{
			// locate first free bit within oat
			for (bit = 0, bitPos = 1; oat[t] & bitPos; bit ++, bitPos <<= 1);
			objNum = ((oatNum = t) << HLUINTSIZESHIFT) | bit;
			break;
		}
	}
	if (t == oatSize)	// node full
	{
		freeOat = oatSize - 1;
		return 0;
	}

	// set bit
	oat[oatNum] |= bitPos;

	// guess next freebit
	freeBit = bit + 1;
	// guess next free oat
	if (freeBit < HLUINTSIZE) freeOat = oatNum;
	else
	{
		freeBit = 0;
		freeOat = oatNum + 1;
		if (freeOat >= oatSize) freeOat = oatSize - 1;
	}

	MemoryUsed += size;

	// return corresponding pointer
	return (void *)&ptr[objNum * size];
}

void MemoryManager::MemoryNode::freePtr(void * p)
{
	uint32 object, oatNum, bit;
	object = (((uintp)p - (uintp)ptr) / size);

#ifdef HLNEWERRORS
	HLASSERT(object < objects, "MemoryNode::freePtr: Node does not contain pointer!");
	HLASSERT((uintp)ptr + object * size == (uintp)p, "MemoryNode::freePtr: Bad pointer alignment!");
#endif	// HLNEWERRORS

	bit = object & HLUINTSIZEMASK;
	oatNum = object >> HLUINTSIZESHIFT;

#ifdef HLNEWERRORS
	HLASSERT(oat[oatNum] & (1 << bit),
		"MemoryNode::freePtr: Pointer already freed!" << HLSTD::endl
		<< "SIZE: " << size << " NODE: " << this << " OBJECT: " << oatNum << " BIT: " << bit << " BAT: " << oat[oatNum] << HLSTD::endl
		<< (void*)ptr << " <= " << p << " < " << (void*)&ptr[size * objects]);
#endif	// HLNEWERRORS

	// clear corresponding bit and update firstFree
	oat[oatNum] ^= 1 << bit;
	if (oatNum < freeOat) freeOat = oatNum;
	if (bit < freeBit) freeBit = bit;

	if (this != MemoryManager::firstFree[size])	// move to end if not firstFree
	{
		// remove the node from the list
		if (prev) prev->next = next;
		else MemoryManager::head[size] = next;
		if (next) next->prev = prev;
		else MemoryManager::tail[size] = prev;
		// add to end of list
		prev = MemoryManager::tail[size];
		next = 0;
		if (!MemoryManager::head[size])
		{
			MemoryManager::head[size] = MemoryManager::tail[size] = this;
		}
		else
		{
			MemoryManager::tail[size] = MemoryManager::tail[size]->next = this;
		}
	}

	MemoryUsed -= size;
}

MemoryManager::MemoryNode * MemoryManager::freeNode()
{
	if (!freeNodeList) beam[beams ++] = new MemoryBeam;
	MemoryNode * r = freeNodeList;
	freeNodeList = freeNodeList->next;
	return r;
}

void MemoryManager::MemoryNode::freeListify()
{
	prev = 0;
	next = freeNodeList;
	freeNodeList = this;
}

void MemoryManager::FreeListHeader::init(uint32 s)
{
	next = 0;
	size = s;
}

void MemoryManager::FreeListHeader::freeListify()
{
	next = freeList[size];
	freeList[size] = this;
}

#endif


