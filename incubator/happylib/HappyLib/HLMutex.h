
#ifndef _HLMUTEX_H
#define _HLMUTEX_H

#ifndef _HLTEMPLATES_H
#include "HLTemplates.h"
#endif


//// Mutex class [DOCUMENTME] ////

class Mutex
{
private:
	int _data[4];
public:
	Mutex();
	~Mutex();
	int lockIfConvenient();
	void lock();
	void unlock();
};

class MultiMutex
{
private:
	Array<Mutex> mutex;
	int users;
public:
	MultiMutex(int);
	~MultiMutex();
	void lockRead(int &);
	void lockWrite(int &);	// [NOTE] must have read lock to get write lock
	void unlockRead(int &);
	void unlockWrite(int &);
};

template <class Obj> class Monitor
{
private:
	Obj obj;
	MultiMutex mutex;
public:
	Monitor(int u = 8) : mutex(u) {}
	~Monitor() {}
	Obj const * lockRead(int & m) { mutex.lockRead(m); return &obj; }
	Obj * lockWrite(int & m) { mutex.lockWrite(m); return &obj; }
	void unlockRead(int & m) { mutex.unlockRead(m); }
	void unlockWrite(int & m) { mutex.unlockWrite(m); }
};

class MutexLock
{
private:
	Mutex & m;
public:
	MutexLock(Mutex & m) : m(m) { m.lock(); }
	~MutexLock() { m.unlock(); }
};

#define MutexOut(X) { Mutex istreamMutex; istreamMutex.lock(); cout << X; istreamMutex.unlock(); }

#endif
