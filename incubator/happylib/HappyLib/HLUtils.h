
#ifndef _HLUTILS_H
#define _HLUTILS_H

#ifdef _WIN32
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif
#ifndef _INC_MATH
#include <math.h>
#endif
#else // _WIN32
#ifndef _STDLIB_H
#include <stdlib.h>
#endif
#ifndef _MATH_H
#include <math.h>
#endif
#endif // _WIN32


//// Useful little inlines ////

template <class T> inline T Min(T a, T b) { return b < a ? b : a; }
template <class T> inline T Max(T a, T b) { return a < b ? b : a; }
template <class T> inline T MinAbs(T a, T b) { return fabs(b) < fabs(a) ? b : a; }
template <class T> inline T MaxAbs(T a, T b) { return fabs(a) < fabs(b) ? b : a; }
template <class T> inline void Swap(T & x, T & y) { T t = x; x = y; y = t; }
template <class T> inline void Order(T & x, T & y) { if (y < x) { T t = x; x = y; y = t; } }
template <class T1, class T2> inline T2 Clamped(T1 a, T2 b, T2 c) { return a < b ? b : c < a ? c : a; }
template <class T, class X, class Y> inline void Clamp(T & a, X b, Y c) { if (a < (T)b) a = (T)b; else if ((T)c < a) a = (T)c; }
template <class T> inline T Rad(T deg) { return (T)(deg * 0.017453292519943); }
template <class T> inline T Deg(T rad) { return (T)(rad * 57.29577951308238); }
template <class T1, class T2> inline T1 SignedPow(T1 x, T2 e) { return T1(x >= 0.0 ? pow(x, e) : -pow(-x, e)); }
template <class T1, class T2> inline T1 Lerp(T1 a, T1 b, T2 d) { return T1(a + (b - a) * d); }
inline double Frand() { return (double)rand()/(double)RAND_MAX; }
inline double FrandEx() { return (double)rand()/(double)(RAND_MAX+1); }
inline int Even(int a) { return !(a & 1); }
inline int Sign(double a) { return a > 0 ? 1 : -1; }
template <class T> inline int Round(T a) { return a > 0 ? int(a + (T)0.5) : int(a - (T)0.5); }
inline int RoundFrac(int n, int d) { return n >= 0 ? (n + d/2) / d : (n - d/2) / d; }	// assumes d > 0
inline int RoundFracP(int n, int d) { return (n + d/2) / d; }	// assumes n > 0 and d > 0
inline double SafePositiveFraction(double num, double denom, double epsilon)
{
	if (fabs(denom) < epsilon) return 1.0 / epsilon;
	return num / denom;
}
inline int OverMod(int a, int m) { return a < m ? a : a - m; }
inline int UnderMod(int a, int m) { return a >= 0 ? a : a + m; }
inline int CyclicMod(int a, int m) { return a >= 0 ? a % m : m - 1 + (a+1) % m; }	// c++ uses symmetric mod
inline int Log2(int x) { int l = 0, y = 1; for (; y < x; y *= 2) ++ l; return l; }
inline int LogBase(int x, int b) { int l = 0, y = 1; for (; y < x; y *= b) ++ l; return l; }
template <class T> inline T Sq(T d) { return d*d; }
template <class T> inline T Cu(T d) { return d*d*d; }
template <class From, class To> inline To ForceCast(From obj)
{
	// [FIXME] this is really ForceAssign...
	union { From from; To to; } bob;
	bob.from = obj;
	return bob.to;
};
template <class T> class Container
{
public:
	T * obj;
	Container() : obj(0) {}
	Container(T * obj) : obj(obj) {}
	~Container() { delete obj; }
	operator T * () { return obj; }
	T * operator = (T * o) { delete obj; obj = o; return o; }
};

#endif
