
//// Mutex implementation (platform-independant stuff) ////

Mutex::Mutex()
{
	((RealMutex*)this)->create();
}

Mutex::~Mutex()
{
	((RealMutex*)this)->destroy();
}

int Mutex::lockIfConvenient()
{
	return ((RealMutex*)this)->lockIfConvenient();
}

void Mutex::lock()
{
	((RealMutex*)this)->lock();
}

void Mutex::unlock()
{
	((RealMutex*)this)->unlock();
}


//// MultiMutex implementation ////

MultiMutex::MultiMutex(int users) : users(users) { mutex.init(users); }

MultiMutex::~MultiMutex() {}

void MultiMutex::lockRead(int & user)
{
	// get one of the mutexes for the user
	for (user = -1; user == -1;) for (int m = 0; m < users; m ++) if (mutex[m].lockIfConvenient())
	{
		user = m;
		return;
	}
}

void MultiMutex::lockWrite(int & user)
{
	// get mutex 0, which is the designated Writer mutex
	if (user != 0)
	{
		mutex[user].unlock();	// free whatever the user used to have
		mutex[0].lock();		// get writer mutex
		user = 0;
	}

	// get every mutex (already have 0)
	int got = 1, m;
	Array<int> flag(users);
	for (m = 1; m < users; m ++) flag[m] = 0;
	while (got < users)	// get all mutexes in no particular order
	{
		for (m = 1; m < users; m ++) if (!flag[m])
		{
			if (mutex[m].lockIfConvenient())
			{
				flag[m] = 1;
				got ++;
			}
		}
	}
}

void MultiMutex::unlockRead(int & user)
{
	mutex[user].unlock();
}

void MultiMutex::unlockWrite(int & user)
{
	// unlock the other mutexes besides the writer,
	// which retains its value to encourage coherence in object usage
	HLASSERT(user == 0, "MultiMutex::unlockWrite called but user doesn't own write!");
	for (int m = 1; m < users; m ++) mutex[m].unlock();
}
