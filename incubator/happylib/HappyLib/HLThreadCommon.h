
//// Thread implementation (platform-independant stuff) ////

void Thread::sleep(int ms) { RealThread::sleep(ms); }

Thread::Thread() { ((RealThread*)this)->construct(); }

Thread::~Thread() { if (isRunning()) quitBlocking(); }

void Thread::doEverything() {}

int Thread::isRunning() const { return ((RealThread*)this)->isRunning(); }

int Thread::supposedToQuit() const { return ((RealThread*)this)->supposedToQuit(); }

int Thread::start() { return ((RealThread*)this)->start(); }

void Thread::lock() const { ((RealThread*)this)->lock(); }

void Thread::unlock() const { ((RealThread*)this)->unlock(); }

void Thread::pause() const { ((RealThread*)this)->pause(); }

void Thread::unpause() const { ((RealThread*)this)->unpause(); }

void Thread::quitNonBlocking() { ((RealThread*)this)->quitNonBlocking(); }

void Thread::quitBlocking() { ((RealThread*)this)->quitBlocking(); }
