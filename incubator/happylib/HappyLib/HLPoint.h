
#ifndef _HLPOINT_H
#define _HLPOINT_H

//
//	this file contains the 3-tuple classes: PointT and VectorT.
//

// [FIXME] move a whole lot of this to HLPointUtils.h or something
// [FIXME] consider adding a NormalT class, with conversions to / from VectorT

//	COORDINATE SYSTEM CONVENTIONS:
//
//  By default, the coordinate system in this code is Left-Handed, just like LightWave.
//	i.e. X is Right, Y is Up, and Z is Forward.
//  If you #define HLRHS before you include this file, the coordinate system will be Right-Handed,
//  i.e. X is Right, Y is Up, and Z is Backward.
//
//	By default, the cross product in this code uses Left-Hand-Rule.
//	i.e. X cross Y = Z in Left-Handed coordinates, or -Z if HLRHS is #defined.
//	If you #define HLRHR before you include this file, the cross product will use Right-Hand-Rule.
//	i.e. X cross Y = -Z in Left-Handed coordinates, or Z if HLRHS is #defined.
//
//	This allows you to write code for either coordinate system, then easily port it to an
//  application that uses another system by adding the appropriate #defines.
//

//	OPERATOR OVERLOAD CONVENTIONS:
//	There is a number of overloaded operators for each type, to increase writability:

//	ALL TYPES:
//	x = y, x == y and x != y are defined with the expected behavior.
//	x + y, x += y, x - y, x -= y and -x are defined where it makes sense.
//	x * double, x *= double, x / double, x /= double are defined with the expected behavior.

//  pow, exp and log are also available

//	VectorT:
//	!vector				= vector's length
//	~vector				= vector's direction
//	vector1 % vector2	= cross product
//	vector1 %= vector2	= cross product
//	vector1 | vector2	= "distance" between vectors
//	vector1 || vector2	= squared "distance" between vectors
//	vector1 * vector2	= dot product
//	vector1 & nvector2	= part of vector1 in same direction as normal nvector2
//	vector1 &= nvector2	= part of vector1 in same direction as normal nvector2
//	vector1 / nvector2	= part of vector1 orthogonal to normal nvector2
//	vector1 /= nvector2	= part of vector1 orthogonal to normal nvector2

//	PointT:
//	point1 | point2		= distance between points
//	point1 || point2	= squared distance between points
//	point1 >> point2	= normalized direction from point1 to point2
//  point * vector      = product by component
//  point / vector      = quotient by component

//	[NOTE] the following typedefs are defined:

//	typedef VectorT<double> Vector;		(64-bit components)
//	typedef PointT<double> Point;
//	typedef VectorT<float> Vectorf;		(32-bit components)
//	typedef PointT<float> Pointf;
//	typedef VectorT<WeeFloat> Vectori;	(8-bit* components)
//	typedef PointT<WeeFloat> Pointi;
//	* WeeFloats can only take values in the range [0..1]

//	In addition, all types will cast themselves to a pointer of their
//	internal type for easy passing to LightWave's functions.
//	i.e. you can pass a Point into a function expecting a double *,
//	or pass a Pointi into a function expecting an unsigned char *.
//  Vectors and Points can be explicitly cast / constructed from each other,
//  but will not implicitly coerce for safety reasons.

//  Also, for efficiency reasons, these classes do NOT initialize their values to 0.

#ifndef _HLTRIPLE_H
#include "HLTriple.h"
#endif

#ifdef HLRHS
#ifndef HLRHR
#define HLINVERTCROSSPRODUCT	// RHS LHR
#endif	// HLRHR
#else	// HLRHS
#ifdef HLRHR
#define HLINVERTCROSSPRODUCT	// LHS RHR
#endif	// HLRHR
#endif	// HLRHS


//// VectorT class ////

template <class Obj> class VectorT : public TripleT< Obj, VectorT<Obj> >
{
public:
	enum Axis{X,Y,Z,W};
	VectorT() {}
	VectorT(Obj, Obj, Obj);
	explicit VectorT(Obj const *);
	template <class Obj2, class ThatClass> explicit VectorT(TripleT<Obj2, ThatClass> const & v)
	{
		_val[0] = (Obj)v[0]; _val[1] = (Obj)v[1]; _val[2] = (Obj)v[2];
	}
	template <class Obj2> VectorT(VectorT<Obj2> const & v)
	{
		_val[0] = (Obj)v[0]; _val[1] = (Obj)v[1]; _val[2] = (Obj)v[2];
	}
	double operator ! () const;					// length
	VectorT operator ~ () const;				// direction
	template <class Obj2> VectorT & operator %= (VectorT<Obj2> const & b)	// cross product
	{
		VectorT<Obj> c;
#ifdef HLINVERTCROSSPRODUCT
		c[0] = Obj(b[1]*_val[2] - b[2]*_val[1]);
		c[1] = Obj(b[2]*_val[0] - b[0]*_val[2]);
		c[2] = Obj(b[0]*_val[1] - b[1]*_val[0]);
#else
		c[0] = Obj(b[2]*_val[1] - b[1]*_val[2]);
		c[1] = Obj(b[0]*_val[2] - b[2]*_val[0]);
		c[2] = Obj(b[1]*_val[0] - b[0]*_val[1]);
#endif
		return *this = c;
	}
	template <class Obj2> VectorT & operator &= (VectorT<Obj2> const & b)	// same direction (b must be normal)
	{
		return *this = b * (*this * b);
	}
	template <class Obj2> VectorT & operator /= (VectorT<Obj2> const & b)	// orthogonal direction (b must be normal)
	{
		return *this = *this - b * (*this * b);
	}
	VectorT & operator /= (double b) { *this *= 1.0 / b; return *this; }	// compensate for above overloads
	VectorT & normalize();
	double squaredLength() const;
	void rotate(Axis, double);
	/* [FIXME] deprecate these puppies, or at least change them to use the Matrix applyAlignTo stuff
	template <class Obj2> VectorT right(VectorT<Obj2> const & u) const
	{
#ifdef HLRHR
		return ~(*this % u);
#else
		return ~(u % *this);
#endif
	}
	template <class Obj2> VectorT up(VectorT<Obj2> const & u) const
	{
#ifdef HLRHR
		return ~(right(u) % *this);
#else
		return ~(*this % right(u));
#endif
	}
	template <class Obj2> VectorT forward(VectorT<Obj2> const & u) const
	{
#ifdef HLRHR
		return ~(up(u) % right(u));
#else
		return ~(right(u) % up(u));
#endif
	}
	template <class Obj2> VectorT left(VectorT<Obj2> const & u) const
	{
		return -right(u);
	}
	template <class Obj2> VectorT down(VectorT<Obj2> const & u) const
	{
		return -up(u);
	}
	template <class Obj2> VectorT backward(VectorT<Obj2> const & u) const
	{
		return -forward(u);
	}
	VectorT localX() const
	{
#ifdef INVERTCROSSPRODUCT
		return ~(localZ() % *this);
#else
		return ~(*this % localZ());
#endif
	}
	VectorT localY() const
	{
		return *this;
	}
	VectorT localZ() const
	{
#ifdef INVERTCROSSPRODUCT
		return ~(*this % Xhat());
#else
		return ~(XHat() % *this);
#endif
	}
	template <class Obj2> VectorT & toNormal(VectorT<Obj2> const & norm)
	{
		VectorT<Obj> newpos;
		newpos = Origin();
		newpos += norm.localX() * _val[0];
		newpos += norm.localY() * _val[1];
		newpos += norm.localZ() * _val[2];
		return *this = newpos;
	}
	template <class Obj2> VectorT & fromNormal(VectorT<Obj2> const & norm)
	{
		VectorT<Obj> newpos;
		newpos[0] = *this * norm.localX();
		newpos[1] = *this * norm.localY();
		newpos[2] = *this * norm.localZ();
		return *this = newpos;
	}
	template <class Obj2> VectorT & transNormal(VectorT<Obj2> const & norm1, VectorT<Obj2> const & norm2)
	{
		fromNormal(norm1);
		return toNormal(norm2);
	}
	*/
	// statics
	static VectorT Random();
	static VectorT RandomLobe();
	static VectorT RandomLobe(double);
	static inline VectorT Origin() { return VectorT((Obj)0.0, (Obj)0.0, (Obj)0.0); }
	static inline VectorT XHat() { return VectorT((Obj)1.0, (Obj)0.0, (Obj)0.0); }
	static inline VectorT YHat() { return VectorT((Obj)0.0, (Obj)1.0, (Obj)0.0); }
	static inline VectorT ZHat() { return VectorT((Obj)0.0, (Obj)0.0, (Obj)1.0); }
};

typedef VectorT<double> Vector;
typedef VectorT<float> Vectorf;
typedef VectorT<WeeFloat> Vectori;


//// PointT class ////

template <class Obj> class PointT : public TripleT< Obj, PointT<Obj> >
{
public:
	enum Axis{X,Y,Z};
	PointT() {}
	PointT(Obj, Obj, Obj);
	explicit PointT(Obj const *);
	template <class Obj2, class ThatClass> explicit PointT(TripleT<Obj2, ThatClass> const & p)
	{
		_val[0] = (Obj)p[0]; _val[1] = (Obj)p[1]; _val[2] = (Obj)p[2];
	}
	template <class Obj2> PointT(PointT<Obj2> const & p)
	{
		_val[0] = (Obj)p[0]; _val[1] = (Obj)p[1]; _val[2] = (Obj)p[2];
	}
	template <class Obj2> void move(VectorT<Obj2> const & b, double r)
	{
		_val[0] += Obj(b[0] * r);
		_val[1] += Obj(b[1] * r);
		_val[2] += Obj(b[2] * r);
	}
	template <class Obj2> void moveTo(PointT<Obj2> const & b, double r)
	{
		VectorT<Obj> dir = *this >> b;
		r = Min(r, *this | b);
		move(dir, r);
	}
	template <class Obj2> void nudge(VectorT<Obj2> const & d) { move(d, NUDGE); }
	void rotate(Axis, double);
	// [FIXME] maybe move some of these to HLPointUtils.h
	VectorT<Obj> randVector(int seed = 0) const	// location-based pseudo-random unit vector
	{
		// random vector on a sphere
		double y = 2.0 * rand(2*seed) - 1.0;
		double t = 2.0 * Pi * rand(2*seed+1);
		double w = sqrt(1.0 - y*y);
		return VectorT<Obj>(Obj(w * cos(t)), Obj(y),  Obj(w * sin(t)));
	}
	VectorT<Obj> randLobe(int seed = 0) const	// location-based pseudo-random unit vector
	{
		// random vector on a cosine lobe
		double y = sqrt(Max(rand(2*seed), 0.0000001));
		double t = 2.0 * Pi * rand(2*seed+1);
		double w = sqrt(1.0 - y*y);
		return VectorT<Obj>(Obj(w * cos(t)), Obj(y),  Obj(w * sin(t)));
	}
	VectorT<Obj> randLobe(double exponent, int seed = 0) const	// location-based pseudo-random unit vector
	{
		// random vector on a cosine lobe
		double y = pow(Max(rand(2*seed), 0.0000001), 1.0 / (1.0 + exponent));
		double w = sqrt(1.0 - y*y);
		double t = 2.0 * Pi * rand(2*seed+1);
		return VectorT<Obj>(Obj(w * cos(t)), Obj(y),  Obj(w * sin(t)));
	}
	/* [FIXME] deprecate or not?
	template <class Obj2> void toCoordSys(VectorT<Obj2> const & vx, VectorT<Obj2> const & vy, VectorT<Obj2> const & vz)
	{
		*this = PointT<Obj>(v0 * _val[0] + v1 * _val[1] + v2 * _val[2]);
	}
	template <class Obj2> void fromCoordSys(VectorT<Obj2> const & vx, VectorT<Obj2> const & vy, VectorT<Obj2> const & vz)
	{
#ifdef INVERTCROSSPRODUCT
		double denom;
		denom = vx * ~(vy % vz);
		if (denom == 0.0) return;	// degenerate coordinate system
		PointT<Obj> newpos;
		newpos[0] = VectorT<Obj>(*this) * ~(vy % vz);
		newpos[1] = VectorT<Obj>(*this) * ~(vz % vx);
		newpos[2] = VectorT<Obj>(*this) * ~(vx % vy);
		newpos /= denom;
		*this = newpos;
#else
		double denom;
		denom = vx * ~(vz % vy);
		if (denom == 0.0) return;	// degenerate coordinate system
		PointT<Obj> newpos;
		newpos[0] = VectorT<Obj>(*this) * ~(vz % vy);
		newpos[1] = VectorT<Obj>(*this) * ~(vx % vz);
		newpos[2] = VectorT<Obj>(*this) * ~(vy % vx);
		newpos /= denom;
		*this = newpos;
#endif
	}
	*/
	// statics
	// [FIXME] maybe move some of these to HLPointUtils.h or HLRandomUtils.h
	static PointT RandomSphere()
	{
		// random point on a sphere
		double y = 2.0 * Frand() - 1.0;
		double t = 2.0 * 3.1415926535897932384626433832795 * Frand();
		double w = sqrt(1.0 - y*y);
		return PointT<Obj>(Obj(w * cos(t)), Obj(y),  Obj(w * sin(t)));
	}
	static PointT RandomLobe()
	{
		// random point on a cosine lobe
		double y = sqrt(Max(Frand(), 0.0000001));
		double t = 2.0 * 3.1415926535897932384626433832795 * Frand();
		double w = sqrt(1.0 - y*y);
		return PointT<Obj>(Obj(w * cos(t)), Obj(y),  Obj(w * sin(t)));
	}
	static PointT RandomLobe(double exponent)
	{
		// random point on a cosine lobe
		double y = pow(Max(Frand(), 0.0000001), 1.0 / (1.0 + exponent));
		double t = 2.0 * 3.1415926535897932384626433832795 * Frand();
		double w = sqrt(1.0 - y*y);
		return PointT<Obj>(Obj(w * cos(t)), Obj(y),  Obj(w * sin(t)));
	}
	static inline PointT Origin() { return PointT((Obj)0.0, (Obj)0.0, (Obj)0.0); }
};

typedef PointT<double> Point;
typedef PointT<float> Pointf;
typedef PointT<WeeFloat> Pointi;


//// non-member functions ////

template <class Obj, class Obj2> VectorT<Obj> operator % (VectorT<Obj> const & a, VectorT<Obj2> const & b) { return VectorT<Obj>(a) %= b; }// cross product
template <class Obj, class Obj2> VectorT<Obj> operator & (VectorT<Obj> const & a, VectorT<Obj2> const & b) { return VectorT<Obj>(a) &= b; }// same direction
template <class Obj, class Obj2> VectorT<Obj> operator / (VectorT<Obj> const & a, VectorT<Obj2> const & b) { return VectorT<Obj>(a) /= b; }// orthogonal direction
template <class Obj, class Obj2> double operator | (VectorT<Obj> const &, VectorT<Obj2> const &);					// "distance"
template <class Obj, class Obj2> double operator || (VectorT<Obj> const &, VectorT<Obj2> const &);					// squared "distance"
template <class Obj, class Obj2> double operator * (VectorT<Obj> const &, VectorT<Obj2> const &);					// dot product
template <class Obj, class Obj2> VectorT<Obj> reflect(VectorT<Obj> const &, VectorT<Obj2> const &);					// reflection vector given N, I
template <class Obj, class Obj2> VectorT<Obj> safeReflect(VectorT<Obj> const &, VectorT<Obj> const &n, VectorT<Obj2> const &);	// reflection vector given N, GN, I

template <class Obj, class Obj2> VectorT<Obj> operator - (PointT<Obj> const & a, PointT<Obj2> const & b);			// difference
template <class Obj, class Obj2> double operator | (PointT<Obj> const &, PointT<Obj2> const &);						// distance
template <class Obj, class Obj2> double operator || (PointT<Obj> const &, PointT<Obj2> const &);					// squared distance
template <class Obj, class Obj2> VectorT<Obj> operator >> (PointT<Obj> const &, PointT<Obj2> const &);				// direction from one to other
template <class Obj, class Obj2, class Obj3> VectorT<Obj> TriNormDir(PointT<Obj> const &, PointT<Obj2> const &, PointT<Obj3> const &);	// normal given CW points
template <class Obj, class Obj2, class Obj3> VectorT<Obj> TriNorm(PointT<Obj> const &, PointT<Obj2> const &, PointT<Obj3> const &);		// normal given CW points
template <class Obj, class Obj2, class Obj3> int Collinear(PointT<Obj> const &, PointT<Obj2> const &, PointT<Obj3> const &);

template <class Obj, class Obj2> VectorT<Obj> operator - (PointT<Obj> const & a, PointT<Obj2> const & b)
{
	return VectorT<Obj>((Obj)(a[0] - b[0]), (Obj)(a[1] - b[1]), (Obj)(a[2] - b[2]));
}

template <class Obj, class Obj2> double operator | (VectorT<Obj> const & a, VectorT<Obj2> const & b)
{
	return !(a - b);
}

template <class Obj, class Obj2> double operator || (VectorT<Obj> const & a, VectorT<Obj2> const & b)
{
	return (a - b).squaredLength();
}

template <class Obj, class Obj2> VectorT<Obj> reflect(VectorT<Obj> const & n, VectorT<Obj2> const & i)
{
	return n*2*(n*i)-i;
}

template <class Obj, class Obj2> VectorT<Obj> safeReflect(VectorT<Obj> const & n, VectorT<Obj> const & gn, VectorT<Obj2> const & i)
{
	VectorT<Obj> r = n*2*(n*i)-i;
	if (r * gn <= 0.0)	// reflection ray penetrates geometric norm
		return ~Lerp(~(gn % (r % gn)), gn, 0.000001);	// nearly parallel to the surface, but still above
	return r;
}

template <class Obj, class Obj2> double operator * (VectorT<Obj> const & a, VectorT<Obj2> const & b)	// dot product
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

template <class Obj, class Obj2> double operator | (PointT<Obj> const & a, PointT<Obj2> const & b)
{
	return !(a - b);
}

template <class Obj, class Obj2> double operator || (PointT<Obj> const & a, PointT<Obj2> const & b)
{
	return (a - b).squaredLength();
}

template <class Obj, class Obj2> VectorT<Obj> operator >> (PointT<Obj> const & from, PointT<Obj2> const & to)
{
	return ~(to - from);
}

template <class Obj, class Obj2, class Obj3> VectorT<Obj> TriNormDir(PointT<Obj> const & a, PointT<Obj2> const & b, PointT<Obj3> const & c)
{
#ifdef HLRHR
	return (c - b) % (b - a);
#else
	return (b - a) % (c - b);
#endif
}

template <class Obj, class Obj2, class Obj3> VectorT<Obj> TriNorm(PointT<Obj> const & a, PointT<Obj2> const & b, PointT<Obj3> const & c)
{
	return ~TriNormDir(a, b, c);
}

template <class Obj, class Obj2, class Obj3> int Collinear(PointT<Obj> const & a, PointT<Obj2> const & b, PointT<Obj3> const & c)
{
	return !((c - b) % (b - a)) == 0.0;
}


//// VectorT implementation ////

template <class Obj> VectorT<Obj>::VectorT(Obj x, Obj y, Obj z)
{
	_val[0] = x; _val[1] = y; _val[2] = z;
}

template <class Obj> VectorT<Obj>::VectorT(Obj const * f)
{
	_val[0] = f[0]; _val[1] = f[1]; _val[2] = f[2];
}

template <class Obj> double VectorT<Obj>::operator ! () const
{
	return sqrt(Sq(_val[0]) + Sq(_val[1]) + Sq(_val[2]));
}

template <class Obj> double VectorT<Obj>::squaredLength() const
{
	return Sq(_val[0]) + Sq(_val[1]) + Sq(_val[2]);
}

template <class Obj> VectorT<Obj> & VectorT<Obj>::normalize()
{
	double sqlen = squaredLength();
	if (sqlen > 0.0) return *this /= sqrt(sqlen);
	return *this = YHat();
}

template <class Obj> VectorT<Obj> VectorT<Obj>::operator ~ () const
{
	double sqlen = squaredLength();
	if (sqlen > 0.0) return *this / sqrt(sqlen);
	return YHat();
}

template <class Obj> VectorT<Obj> VectorT<Obj>::Random()
{
	// random unit vector
	return PointT<Obj>::RandomSphere();
}

template <class Obj> VectorT<Obj> VectorT<Obj>::RandomLobe()
{
	// random unit vector
	return PointT<Obj>::RandomLobe();
}

template <class Obj> VectorT<Obj> VectorT<Obj>::RandomLobe(double exponent)
{
	// random unit vector
	return PointT<Obj>::RandomLobe(exponent);
}


//// PointT implementation ////

template <class Obj> PointT<Obj>::PointT(Obj x, Obj y, Obj z)
{
	_val[0] = x; _val[1] = y; _val[2] = z;
}

template <class Obj> PointT<Obj>::PointT(Obj const * f)
{
	_val[0] = f[0]; _val[1] = f[1]; _val[2] = f[2];
}

template <class Obj> void PointT<Obj>::rotate(Axis axis, double th)
{
	double t;
	switch (axis)	// [FIXME]
	{
	case X:
		break;
	case Y:
		t = _val[0] * cos(th) + _val[2] * sin(th);
		_val[2] = _val[2] * cos(th) - _val[0] * sin(th);
		_val[0] = t;
		break;
	case Z:
		break;
	}
}

#endif
