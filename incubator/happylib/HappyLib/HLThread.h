
#ifndef _HLTHREAD_H
#define _HLTHREAD_H


//// Thread baseclass [DOCUMENTME] ////

class Thread
{
private:
	int _data[10];
public:
	static void sleep(int ms);
	Thread();
	virtual ~Thread();
	virtual void doEverything();
	int isRunning() const;		// status: is the thread running?
	int supposedToQuit() const;	// status: has quit been signalled?
	int start();				// start the execution (runs doEverything) return success
	void lock() const;			// lock thread data (meant for public data)
	void unlock() const;		// unlock thread data (meant for public data)
	void pause() const;			// pause execution of the thread
	void unpause() const;		// unpause execution of the thread
	void quitNonBlocking();		// tell thread to quit but don't wait for it
	void quitBlocking();		// tell thread to quit and wait for it
};

#endif
