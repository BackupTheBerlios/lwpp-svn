
#ifndef _HLTIMER_H
#define _HLTIMER_H

#ifdef __unix__
#include <sys/time.h>
inline static double ReadClock()
{
	timeval tv;
	gettimeofday(&tv, 0);
	return (double)tv.tv_sec + tv.tv_usec * 0.000001;
}
#else	// __unix__
#ifdef _WIN32
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOSOUND
#define NOCOMM
#define NOKANJI
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#include <windows.h>
inline static double ReadClock()	// [FIXME][NOW] ALL THESE are jittery :(
{
	LONGLONG llTime, llFreq;
    QueryPerformanceCounter((LARGE_INTEGER*)&llTime);	// [FIXME] jitters
    QueryPerformanceFrequency((LARGE_INTEGER*)&llFreq);
	return (double)llTime / (double)llFreq;
}
#else	// _WIN32
#error No support for Timer class on this platform yet.
#endif	// _WIN32
#endif	// __unix__


//// Timer class ////

class Timer 
{
private:
	double _initial;
public:
	Timer() : _initial(ReadClock()) {}
	double getSeconds() const { return ReadClock() - _initial; }
	void reset() { _initial = ReadClock(); }
};

#endif
