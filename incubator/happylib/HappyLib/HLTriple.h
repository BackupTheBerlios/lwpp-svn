
#ifndef _HLTRIPLE_H
#define _HLTRIPLE_H

//
//	this file contains the 3-tuple baseclass TripleT
//

//	OPERATOR OVERLOAD CONVENTIONS:
//	There is a number of overloaded operators, to increase writability:

//	x = y, x == y and x != y are defined with the expected behavior.
//	x + y, x += y, x - y, x -= y and -x are defined where it makes sense.
//	x * double, x *= double, x / double, x /= double are defined with the expected behavior.

//  fabs, pow, exp and log are also available

//	[NOTE] the following typedefs are defined:

//	typedef TripleConcrete<double> Triple;		(64-bit components)
//	typedef TripleConcrete<float> Triplef;		(32-bit components)
//	typedef TripleConcrete<WeeFloat> Triplei;	(8-bit* components)
//	* WeeFloats can only take values in the range [0..1]

//	In addition, a TripleT will cast themselves to a pointer of its
//	internal type for easy passing to LightWave's functions.
//	i.e. you can pass a TripleT<double> into a function expecting a double *

//  Also, for efficiency reasons, these classes do NOT initialize their values to 0.

#ifndef _HLUTILS_H
#include "HLUtils.h"
#endif
#ifndef _HLDEFS_H
#include "HLDefs.h"
#endif
#ifndef _HLWEEFLOAT_H
#include "HLWeefloat.h"
#endif

#ifdef _WIN32
#ifndef _INC_MATH
#include <math.h>
#endif
#else // _WIN32
#ifndef _MATH_H
#include <math.h>
#endif
#endif // _WIN32
#include HLIOSTREAM_H


//// baseclass for 3-tuple classes ////

template < class Obj, class ThisClass > class TripleT
{
public:
	Obj _val[3];
	// accessors
	template <class Obj2> void get(Obj2 * f) const
	{
		f[0] = (Obj2)_val[0]; f[1] = (Obj2)_val[1]; f[2] = (Obj2)_val[2];
	}
	template <class Obj2> void get(Obj2 & x, Obj2 & y, Obj2 & z) const
	{
		x = (Obj2)_val[0]; y = (Obj2)_val[1]; z = (Obj2)_val[2];
	}
	int intGet(int) const;
	void intGet(int &, int &, int &) const;
	void intGet(unsigned char &, unsigned char &, unsigned char &) const;
	void intGet(int *) const;
	void intGet(unsigned char *) const;
	// mutators
	template <class Obj2> void set(Obj2 const * f)
	{
		_val[0] = (Obj)f[0]; _val[1] = (Obj)f[1]; _val[2] = (Obj)f[2];
	}
	template <class Obj2> void set(Obj2 x, Obj2 y, Obj2 z)
	{
		_val[0] = (Obj)x; _val[1] = (Obj)y; _val[2] = (Obj)z;
	}
	void intSet(int, int, int);
	void intSet(int *);
	void intSet(unsigned char *);
	// operators
	ThisClass operator - () const;	// negative
	template <class Obj2, class ThatClass> ThisClass & operator += (TripleT<Obj2, ThatClass> const & c)
	{
		_val[0] += (Obj)c[0]; _val[1] += (Obj)c[1]; _val[2] += (Obj)c[2];
		return *(ThisClass*)this;
	}
	template <class Obj2, class ThatClass> ThisClass & operator -= (TripleT<Obj2, ThatClass> const & c)
	{
		_val[0] -= (Obj)c[0]; _val[1] -= (Obj)c[1]; _val[2] -= (Obj)c[2];
		return *(ThisClass*)this;
	}
	template <class Obj2, class ThatClass> ThisClass & operator *= (TripleT<Obj2, ThatClass> const & b)	// non-dot product
	{
		_val[0] *= (Obj)b[0]; _val[1] *= (Obj)b[1]; _val[2] *= (Obj)b[2];
		return *(ThisClass*)this;
	}
	template <class Obj2, class ThatClass> ThisClass & operator /= (TripleT<Obj2, ThatClass> const & b)	// non-dot quotient
	{
		_val[0] /= (Obj)b[0]; _val[1] /= (Obj)b[1]; _val[2] /= (Obj)b[2];
		return *(ThisClass*)this;
	}
	ThisClass & operator *= (double);			// multiply
	ThisClass & operator /= (double b) { *this *= 1.0 / b; return *(ThisClass*)this; }
	operator Obj*() { return _val; }
	operator const Obj*() const { return _val; }
	// functions
	void max(double);
	void min(double);
	ThisClass & clamp(double, double);
	double rand(int seed = 0) const;
	void display(HLSTD::ostream &) const;
};

template <class Obj> class TripleConcrete : public TripleT< Obj, TripleConcrete<Obj> >
{
public:
	// contructors
	TripleConcrete() {}
	explicit TripleConcrete(Obj const * f)
	{
		_val[0] = f[0]; _val[1] = f[1]; _val[2] = f[2];
	}
	TripleConcrete(double x, double y, double z)
	{
		_val[0] = (Obj)x; _val[1] = (Obj)y; _val[2] = (Obj)z;
	}
	template <class Obj2, class ThatClass> TripleConcrete(TripleT<Obj2, ThatClass> const & v)
	{
		_val[0] = (Obj)v[0]; _val[1] = (Obj)v[1]; _val[2] = (Obj)v[2];
	}
};

typedef TripleConcrete<double> Triple;
typedef TripleConcrete<float> Triplef;
typedef TripleConcrete<WeeFloat> Triplei;


//// non-member functions ////

template <class Obj, class ThisClass, class Obj2, class ThatClass>
ThisClass operator + (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b) { return ThisClass(a) += b; }
template <class Obj, class ThisClass, class Obj2, class ThatClass>
ThisClass operator - (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b) { return ThisClass(a) -= b; }
template <class Obj, class ThisClass, class Obj2, class ThatClass>
ThisClass operator * (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b) { return ThisClass(a) *= b; }
template <class Obj, class ThisClass, class Obj2, class ThatClass>
ThisClass operator / (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b) { return ThisClass(a) /= b; }
template <class Obj, class ThisClass>
ThisClass operator * (TripleT<Obj, ThisClass> const & a, double b) { return ThisClass(a) *= b; }	// product
template <class Obj, class ThisClass>
ThisClass operator / (TripleT<Obj, ThisClass> const & a, double b) { return ThisClass(a) /= b; }	// quotient

template <class Obj, class ThisClass> ThisClass fabs(TripleT<Obj, ThisClass> const & p)
{
	return ThisClass(fabs(p[0]), fabs(p[1]), fabs(p[2]));
}

template <class Obj, class ThisClass> ThisClass pow(TripleT<Obj, ThisClass> const & p, double pw)
{
	return ThisClass(pow(p[0], pw), pow(p[1], pw), pow(p[2], pw));
}

template <class Obj, class ThisClass> ThisClass exp(TripleT<Obj, ThisClass> const & p)
{
	return ThisClass(exp(p[0]), exp(p[1]), exp(p[2]));
}

template <class Obj, class ThisClass> ThisClass log(TripleT<Obj, ThisClass> const & p)
{
	return ThisClass(log(p[0]), log(p[1]), log(p[2]));
}

template <class Obj, class ThisClass> HLSTD::ostream & operator << (HLSTD::ostream & out, TripleT<Obj, ThisClass> const & p)
{
	p.display(out);
	return out;
}

template <class Obj, class ThisClass, class Obj2, class ThatClass> bool operator < (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b)
{
	if (a[0] < b[0]) return true;
	if (a[0] > b[0]) return false;
	if (a[1] < b[1]) return true;
	if (a[1] > b[1]) return false;
	return a[2] < b[2];
}

template <class Obj, class ThisClass, class Obj2, class ThatClass> bool operator == (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

template <class Obj, class ThisClass, class Obj2, class ThatClass> bool operator != (TripleT<Obj, ThisClass> const & a, TripleT<Obj2, ThatClass> const & b) { return !(a == b); }


//// TripleT implementation ////

template <class Obj, class ThisClass> int TripleT<Obj, ThisClass>::intGet(int i) const
{
	return int(_val[i] * 255.0 + 0.5);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intGet(int & i0, int & i1, int & i2) const
{
	i0 = int(_val[0] * 255.0 + 0.5);
	i1 = int(_val[1] * 255.0 + 0.5);
	i2 = int(_val[2] * 255.0 + 0.5);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intGet(unsigned char & i0, unsigned char & i1, unsigned char & i2) const
{
	i0 = (unsigned char)(_val[0] * 255.0 + 0.5);
	i1 = (unsigned char)(_val[1] * 255.0 + 0.5);
	i2 = (unsigned char)(_val[2] * 255.0 + 0.5);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intGet(int * i) const
{
	i[0] = int(_val[0] * 255.0 + 0.5);
	i[1] = int(_val[1] * 255.0 + 0.5);
	i[2] = int(_val[2] * 255.0 + 0.5);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intGet(unsigned char * i) const
{
	i[0] = (unsigned char)(_val[0] * 255.0 + 0.5);
	i[1] = (unsigned char)(_val[1] * 255.0 + 0.5);
	i[2] = (unsigned char)(_val[2] * 255.0 + 0.5);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intSet(int r, int g, int b)		// chars, really
{
	_val[0] = (Obj)(r * 0.00392156862745098039215686274509804);
	_val[1] = (Obj)(g * 0.00392156862745098039215686274509804);
	_val[2] = (Obj)(b * 0.00392156862745098039215686274509804);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intSet(int * i)		// chars, really
{
	_val[0] = (Obj)(i[0] * 0.00392156862745098039215686274509804);
	_val[1] = (Obj)(i[1] * 0.00392156862745098039215686274509804);
	_val[2] = (Obj)(i[2] * 0.00392156862745098039215686274509804);
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::intSet(unsigned char * i)
{
	_val[0] = (Obj)(i[0] * 0.00392156862745098039215686274509804);
	_val[1] = (Obj)(i[1] * 0.00392156862745098039215686274509804);
	_val[2] = (Obj)(i[2] * 0.00392156862745098039215686274509804);
}

template <class Obj, class ThisClass> ThisClass TripleT<Obj, ThisClass>::operator - () const
{
	TripleT<Obj, ThisClass> v;
	v[0] = -_val[0];
	v[1] = -_val[1];
	v[2] = -_val[2];
	return v;
}

template <class Obj, class ThisClass> ThisClass & TripleT<Obj, ThisClass>::operator *= (double r)
{
	_val[0] *= (Obj)r;
	_val[1] *= (Obj)r;
	_val[2] *= (Obj)r;
	return *(ThisClass*)this;
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::max(double m)
{
	if (_val[0] > m) _val[0] = (Obj)m;
	if (_val[1] > m) _val[1] = (Obj)m;
	if (_val[2] > m) _val[2] = (Obj)m;
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::min(double m)
{
	if (_val[0] < m) _val[0] = (Obj)m;
	if (_val[1] < m) _val[1] = (Obj)m;
	if (_val[2] < m) _val[2] = (Obj)m;
}

template <class Obj, class ThisClass> ThisClass & TripleT<Obj, ThisClass>::clamp(double mn, double mx)
{
	if (_val[0] > mx) _val[0] = (Obj)mx;
	else if (_val[0] < mn) _val[0] = (Obj)mn;
	if (_val[1] > mx) _val[1] = (Obj)mx;
	else if (_val[1] < mn) _val[1] = (Obj)mn;
	if (_val[2] > mx) _val[2] = (Obj)mx;
	else if (_val[2] < mn) _val[2] = (Obj)mn;
	return *(ThisClass*)this;
}

template <class Obj, class ThisClass> void TripleT<Obj, ThisClass>::display(HLSTD::ostream & out) const
{
	out << "<" << _val[0] << "," << _val[1] << "," << _val[2] << ">";
}

template <class Obj, class ThisClass> double TripleT<Obj, ThisClass>::rand(int seed) const	// pseudo-random number between 0.0 and 1.0
{
	// [FIXME] make this machine-independant.  Right now intel causes differences.
	// [FIXME] also, use a good algorithm instead of this one.
	Triplef pos = *this * cos(seed);	// [FIXME] using cos is expensive and lame
	unsigned char * c0 = (unsigned char*)(&pos[0]);
	unsigned char * c1 = (unsigned char*)(&pos[1]);
	unsigned char * c2 = (unsigned char*)(&pos[2]);
	unsigned char x0 = c0[0]*1 ^ c0[1]*5 ^ c0[2]*13 ^ c0[3]*23;
	unsigned char x1 = c1[0]*2 ^ c1[1]*7 ^ c1[2]*17 ^ c1[3]*29;
	unsigned char x2 = c2[0]*3 ^ c2[1]*11 ^ c2[2]*19 ^ c2[3]*31;
	double k = (x0 ^ x1 ^ x2) / 255.0;
	return k;
}

#endif
