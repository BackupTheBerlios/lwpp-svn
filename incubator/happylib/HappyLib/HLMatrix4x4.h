
#ifndef _HLMATRIX4X4_H
#define _HLMATRIX4X4_H

//
//	This file contains the Matrix4X4T class, which is a 4x4 matrix type.
//

//	MATRIX CONVENTIONS:

//	VectorT and PointT are conidered column vectors, i.e. a 1x4 Matrix (1x3 + implicit w).
//	The w component of PointT is implicitly 1.
//	The w component of VectorT is implicitly 0.

//	Matrix4X4T is row-major, i.e. [[xx xy xz xw] [yx yy yz yw] [zx zy zz zw] [wx wy wz ww]].
//  Note that a 3x3 Matrix is implicitly [[xx xy xz 0] [yx yy yz 0] [zx zy zz 0] [0 0 0 1]].

//	Thus to transform a point by a matrix we use newPoint = Mat * oldPoint.

#ifndef _HLPOINT_H
#include "HLPoint.h"
#endif
#ifndef _HLCOLOR_H
#include "HLColor.h"
#endif

#include <float.h>	// [FIXME] for _finite

enum MatCell4{X4X,X4Y,X4Z,X4W, Y4X,Y4Y,Y4Z,Y4W, Z4X,Z4Y,Z4Z,Z4W, W4X,W4Y,W4Z,W4W};

template <class Obj> class Matrix4X4T
{
public:
	union
	{
		Obj _mat[16];
		Obj _mtx[4][4];
	};

	enum Axis{X,Y,Z,W};

	Matrix4X4T() {}
	Matrix4X4T(Obj const * m) { for (int i = 0; i < 16; i ++) _mat[i] = m[i]; }
	Matrix4X4T(double, double, double, double, double, double, double, double,
		double, double, double, double, double, double, double, double);
	Matrix4X4T(VectorT<Obj> const & vx, VectorT<Obj> const & vy, VectorT<Obj> const & vz)
	{
		// create a transformation matrix from Right, Up, Forward
		_mat[X4X] = vx[X]; _mat[Y4X] = vx[Y]; _mat[Z4X] = vx[Z]; _mat[W4X] = 0.0;
		_mat[X4Y] = vy[X]; _mat[Y4Y] = vy[Y]; _mat[Z4Y] = vy[Z]; _mat[W4Y] = 0.0;
		_mat[X4Z] = vz[X]; _mat[Y4Z] = vz[Y]; _mat[Z4Z] = vz[Z]; _mat[W4Z] = 0.0;
		_mat[X4W] = 0.0; _mat[Y4W] = 0.0; _mat[Z4W] = 0.0; _mat[W4W] = 1.0;
	}
	Matrix4X4T(VectorT<Obj> const & vx, VectorT<Obj> const & vy, VectorT<Obj> const & vz, PointT<Obj> const & pos)
	{
		// create a transformation matrix from Right, Up, Forward, Position
		_mat[X4X] = vx[X]; _mat[Y4X] = vx[Y]; _mat[Z4X] = vx[Z]; _mat[W4X] = 0.0;
		_mat[X4Y] = vy[X]; _mat[Y4Y] = vy[Y]; _mat[Z4Y] = vy[Z]; _mat[W4Y] = 0.0;
		_mat[X4Z] = vz[X]; _mat[Y4Z] = vz[Y]; _mat[Z4Z] = vz[Z]; _mat[W4Z] = 0.0;
		_mat[X4W] = pos[X]; _mat[Y4W] = pos[Y]; _mat[Z4W] = pos[Z]; _mat[W4W] = 1.0;
	}
	template<class Obj2> Matrix4X4T(Matrix4X4T<Obj2> const & m)
	{
		for (int i = 0; i < 16; i ++) _mat[i] = (Obj)m._mat[i];
	}
	template<class Obj2> Matrix4X4T & operator = (Matrix4X4T<Obj2> const & m)
	{
		for (int i = 0; i < 16; i ++) _mat[i] = (Obj)m._mat[i];
		return *this;
	}
	void set(Obj const * m) { for (int i = 0; i < 16; i ++) _mat[i] = m[i]; }
	void get(Obj * m) const { for (int i = 0; i < 16; i ++) m[i] = _mat[i]; }
	template <class Obj2> Obj2 wTrans(PointT<Obj2> const & src, PointT<Obj2> & dest) const
	{
		// wTrans performs Point = Mat * Point, but returns w instead of dividing through by w.
		dest[X] = Obj2(src[X]*_mat[X4X] + src[Y]*_mat[X4Y] + src[Z]*_mat[X4Z] + _mat[X4W]);
		dest[Y] = Obj2(src[X]*_mat[Y4X] + src[Y]*_mat[Y4Y] + src[Z]*_mat[Y4Z] + _mat[Y4W]);
		dest[Z] = Obj2(src[X]*_mat[Z4X] + src[Y]*_mat[Z4Y] + src[Z]*_mat[Z4Z] + _mat[Z4W]);
		return Obj2(src[X]*_mat[W4X] + src[Y]*_mat[W4Y] + src[Z]*_mat[W4Z] + _mat[W4W]);
		// [NOTE] src[W] is implicitly 1
	}
	template <class Obj2> VectorT<Obj2> transposeTrans(VectorT<Obj2> const & v) const
	{
		// transforms v by the transpose
		// [NOTE] for surface normals use ~inverseMat.transposeTrans(norm)
		return VectorT<Obj2>(
			Obj2(v[X]*_mat[X4X] + v[Y]*_mat[Y4X] + v[Z]*_mat[Z4X]),
			Obj2(v[X]*_mat[X4Y] + v[Y]*_mat[Y4Y] + v[Z]*_mat[Z4Y]),
			Obj2(v[X]*_mat[X4Z] + v[Y]*_mat[Y4Z] + v[Z]*_mat[Z4Z]));
	}
	PointT<Obj> origin() const
	{
		return PointT<Obj>(_mat[X4W], _mat[Y4W], _mat[Z4W]) / _mat[W4W];
	}
	VectorT<Obj> xHat() const { return VectorT<Obj>(_mat[X4X], _mat[Y4X], _mat[Z4X]); }
	VectorT<Obj> yHat() const { return VectorT<Obj>(_mat[X4Y], _mat[Y4Y], _mat[Z4Y]); }
	VectorT<Obj> zHat() const { return VectorT<Obj>(_mat[X4Z], _mat[Y4Z], _mat[Z4Z]); }
	void setXHat(VectorT<Obj> const & v) { _mat[X4X] = v[0]; _mat[Y4X] = v[1], _mat[Z4X] = v[2]; }
	void setYHat(VectorT<Obj> const & v) { _mat[X4Y] = v[0]; _mat[Y4Y] = v[1], _mat[Z4Y] = v[2]; }
	void setZHat(VectorT<Obj> const & v) { _mat[X4Z] = v[0]; _mat[Y4Z] = v[1], _mat[Z4Z] = v[2]; }
	double determinant() const;	// determinant of the matrix
	Matrix4X4T transposed() const;	// transposed matrix
	Matrix4X4T adjoint() const;	// upper-left part of the adjoint matrix (use determinant to get the rest)
	Matrix4X4T inverse() const;	// inverse of the matrix (adjoint() / determinant())
	Matrix4X4T safeInverse() const;	// inverse of the matrix (adjoint() / determinant())
	void transpose() { *this = transposed(); }	// transpose the matrix
	void invert() { *this = inverse(); }		// invert the matrix
	Matrix4X4T & operator += (Matrix4X4T const &);
	Matrix4X4T & operator -= (Matrix4X4T const &);
	Matrix4X4T & operator *= (double);
	Matrix4X4T & operator *= (Matrix4X4T const &);
	Matrix4X4T & operator &= (Matrix4X4T const &);
	Matrix4X4T & operator /= (Matrix4X4T const &);
	int operator == (Matrix4X4T const &) const;
	int operator != (Matrix4X4T const & m) const { return !operator==(m); }
	operator Obj*() { return _mat; }
	operator const Obj*() const { return _mat; }
	Obj & operator () (int r, int c) { return _mat[r * 4 + c]; }
	const Obj & operator () (int r, int c) const { return _mat[r * 4 + c]; }
	void display(HLSTD::ostream &) const;
	// statics
	static Matrix4X4T Identity()
	{
		return Matrix4X4T(VectorT<Obj>::XHat(), VectorT<Obj>::YHat(), VectorT<Obj>::ZHat(), PointT<Obj>::Origin());
	}
	static Matrix4X4T HPB(VectorT<Obj> const & hpb);
	static Matrix4X4T Scale(VectorT<Obj> const & scale);
	static Matrix4X4T AlignTo(VectorT<Obj> const & from, VectorT<Obj> const & to);
	static Matrix4X4T PointAt(VectorT<Obj> const & to);
	static Matrix4X4T Translate(VectorT<Obj> const & offset);
};

typedef Matrix4X4T<double> Matrix4X4;
typedef Matrix4X4T<float> Matrix4X4f;
typedef Matrix4X4T<WeeFloat> Matrix4X4i;


//// non-member functions ////

template <class Obj, class Obj2> VectorT<Obj> operator * (Matrix4X4T<Obj2> const &, VectorT<Obj> const &);				// transform vector
template <class Obj, class Obj2> PointT<Obj> operator * (Matrix4X4T<Obj2> const &, PointT<Obj> const &);				// transform point
template <class Obj, class Obj2> ColorT<Obj> operator * (Matrix4X4T<Obj2> const & m, ColorT<Obj> const & a) { return ColorT<Obj>(m * VectorT<Obj>(a)); }

template <class Obj> HLSTD::ostream & operator << (HLSTD::ostream & out, Matrix4X4T<Obj> const & m)
{
	m.display(out);
	return out;
}

template <class Obj, class Obj2> VectorT<Obj> operator * (Matrix4X4T<Obj2> const & m, VectorT<Obj> const & v)
{
	// [NOTE] This is NOT the function you want for surface normals, which are not really directions!
	enum Axis{X,Y,Z,W};
	return VectorT<Obj>(Obj(v[X]*m[X4X] + v[Y]*m[X4Y] + v[Z]*m[X4Z]),
						Obj(v[X]*m[Y4X] + v[Y]*m[Y4Y] + v[Z]*m[Y4Z]),
						Obj(v[X]*m[Z4X] + v[Y]*m[Z4Y] + v[Z]*m[Z4Z]));
	// [NOTE] if m is such that w != 0, the result would no longer be a legal vector!
	//        so be careful that the w column of your matrix looks liks [0 0 0 ?]
}

template <class Obj, class Obj2> PointT<Obj> operator * (Matrix4X4T<Obj2> const & m, PointT<Obj> const & pt)
{
	PointT<Obj> tpt;
	Obj w = m.wTrans(pt, tpt);
	if (w != 0.0) return tpt / w;
	else HLASSERT(0, "Matrix4X4T * PointT: Degenerate matrix:\n" << m);
	return tpt;
}

template <class Obj> Matrix4X4T<Obj> operator + (Matrix4X4T<Obj> a, Matrix4X4T<Obj> const & b) { return a += b; }
template <class Obj> Matrix4X4T<Obj> operator - (Matrix4X4T<Obj> a, Matrix4X4T<Obj> const & b) { return a -= b; }
template <class Obj> Matrix4X4T<Obj> operator * (Matrix4X4T<Obj> a, Matrix4X4T<Obj> const & b) { return a *= b; }
template <class Obj> Matrix4X4T<Obj> operator & (Matrix4X4T<Obj> a, Matrix4X4T<Obj> const & b) { return a &= b; }
template <class Obj> Matrix4X4T<Obj> operator / (Matrix4X4T<Obj> a, Matrix4X4T<Obj> const & b) { return a /= b; }
template <class Obj> Matrix4X4T<Obj> operator * (Matrix4X4T<Obj> a, double b) { return a *= b; }
template <class Obj> Matrix4X4T<Obj> operator / (Matrix4X4T<Obj> a, double b) { return a *= 1.0 / b; }


//// Matrix4X4T implementation ////

template <class Obj>
Matrix4X4T<Obj>::Matrix4X4T(double m0, double m1, double m2, double m3, double m4, double m5, double m6, double m7,
								 double m8, double m9, double m10, double m11, double m12, double m13, double m14, double m15)
{
	_mat[X4X] = (Obj)m0; _mat[X4Y] = (Obj)m1; _mat[X4Z] = (Obj)m2; _mat[X4W] = (Obj)m3;
	_mat[Y4X] = (Obj)m4; _mat[Y4Y] = (Obj)m5; _mat[Y4Z] = (Obj)m6; _mat[Y4W] = (Obj)m7;
	_mat[Z4X] = (Obj)m8; _mat[Z4Y] = (Obj)m9; _mat[Z4Z] = (Obj)m10; _mat[Z4W] = (Obj)m11;
	_mat[W4X] = (Obj)m12; _mat[W4Y] = (Obj)m13; _mat[W4Z] = (Obj)m14; _mat[W4W] = (Obj)m15;
}

template <class Obj> Matrix4X4T<Obj> & Matrix4X4T<Obj>::operator += (Matrix4X4T const & m)
{
	for (int i = 0; i < 16; i ++) _mat[i] += m[i];
	return *this;
}

template <class Obj> Matrix4X4T<Obj> & Matrix4X4T<Obj>::operator -= (Matrix4X4T const & m)
{
	for (int i = 0; i < 16; i ++) _mat[i] -= m[i];
	return *this;
}

template <class Obj> Matrix4X4T<Obj> & Matrix4X4T<Obj>::operator &= (Matrix4X4T const & m)
{
	for (int i = 0; i < 16; i ++) _mat[i] *= m[i];
	return *this;
}

template <class Obj> Matrix4X4T<Obj> & Matrix4X4T<Obj>::operator /= (Matrix4X4T const & m)
{
	for (int i = 0; i < 16; i ++) _mat[i] /= m[i];
	return *this;
}

template <class Obj> Matrix4X4T<Obj> & Matrix4X4T<Obj>::operator *= (double d)
{
	for (int i = 0; i < 16; i ++) _mat[i] *= d;
	return *this;
}

template <class Obj> Matrix4X4T<Obj> & Matrix4X4T<Obj>::operator *= (Matrix4X4T const & b)
{
	Matrix4X4T c;
	int ci = 0;
	for (int i = 0; i < 4; i ++) for (int j = 0; j < 4; j ++)
	{
		c[ci] = 0.0;
		int ai = i * 4;
		int bi = j;
		for (int k = 0; k < 4; k ++)
		{
			c[ci] += _mat[ai ++] * b[bi];
			bi += 4;
		}
		ci ++;
	}
	return *this = c;
}

template <class Obj> int Matrix4X4T<Obj>::operator == (Matrix4X4T<Obj> const & m) const
{
	for (int i = 0; i < 16; i ++) if (_mat[i] != m._mat[i]) return 0;
	return 1;
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::transposed() const
{
	// [OPTIMIZEME] unroll and return-value-optimize
	Matrix4X4T b;
	for (int i = 0; i < 4; i ++) for (int j = 0; j < 4; j ++) b(j, i) = (*this)(i, j);
	return b;
}

template <class Obj> double Matrix4X4T<Obj>::determinant() const
{
	// [OPTIMIZEME] A really good compiler should optimize this stuff pretty well,
	//              but last I checked I'm not using a really good compiler ;)
	return _mat[X4X]*_mat[Y4Y]*_mat[Z4Z]*_mat[W4W]-_mat[X4X]*_mat[Y4Y]*_mat[Z4W]*_mat[W4Z]
		-_mat[X4X]*_mat[Z4Y]*_mat[Y4Z]*_mat[W4W]+_mat[X4X]*_mat[Z4Y]*_mat[Y4W]*_mat[W4Z]
		+_mat[X4X]*_mat[W4Y]*_mat[Y4Z]*_mat[Z4W]-_mat[X4X]*_mat[W4Y]*_mat[Y4W]*_mat[Z4Z]
		-_mat[Y4X]*_mat[X4Y]*_mat[Z4Z]*_mat[W4W]+_mat[Y4X]*_mat[X4Y]*_mat[Z4W]*_mat[W4Z]
		+_mat[Y4X]*_mat[Z4Y]*_mat[X4Z]*_mat[W4W]-_mat[Y4X]*_mat[Z4Y]*_mat[X4W]*_mat[W4Z]
		-_mat[Y4X]*_mat[W4Y]*_mat[X4Z]*_mat[Z4W]+_mat[Y4X]*_mat[W4Y]*_mat[X4W]*_mat[Z4Z]
		+_mat[Z4X]*_mat[X4Y]*_mat[Y4Z]*_mat[W4W]-_mat[Z4X]*_mat[X4Y]*_mat[Y4W]*_mat[W4Z]
		-_mat[Z4X]*_mat[Y4Y]*_mat[X4Z]*_mat[W4W]+_mat[Z4X]*_mat[Y4Y]*_mat[X4W]*_mat[W4Z]
		+_mat[Z4X]*_mat[W4Y]*_mat[X4Z]*_mat[Y4W]-_mat[Z4X]*_mat[W4Y]*_mat[X4W]*_mat[Y4Z]
		-_mat[W4X]*_mat[X4Y]*_mat[Y4Z]*_mat[Z4W]+_mat[W4X]*_mat[X4Y]*_mat[Y4W]*_mat[Z4Z]
		+_mat[W4X]*_mat[Y4Y]*_mat[X4Z]*_mat[Z4W]-_mat[W4X]*_mat[Y4Y]*_mat[X4W]*_mat[Z4Z]
		-_mat[W4X]*_mat[Z4Y]*_mat[X4Z]*_mat[Y4W]+_mat[W4X]*_mat[Z4Y]*_mat[X4W]*_mat[Y4Z];
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::adjoint() const
{
	// [OPTIMIZEME] A really good compiler should optimize this stuff pretty well,
	//              but last I checked I'm not using a really good compiler ;)
	return Matrix4X4T(
	_mat[Y4Y]*_mat[Z4Z]*_mat[W4W]-_mat[Y4Y]*_mat[Z4W]*_mat[W4Z]-_mat[Z4Y]*_mat[Y4Z]*_mat[W4W]
		+_mat[Z4Y]*_mat[Y4W]*_mat[W4Z]+_mat[W4Y]*_mat[Y4Z]*_mat[Z4W]-_mat[W4Y]*_mat[Y4W]*_mat[Z4Z],
	-_mat[X4Y]*_mat[Z4Z]*_mat[W4W]+_mat[X4Y]*_mat[Z4W]*_mat[W4Z]+_mat[Z4Y]*_mat[X4Z]*_mat[W4W]
		-_mat[Z4Y]*_mat[X4W]*_mat[W4Z]-_mat[W4Y]*_mat[X4Z]*_mat[Z4W]+_mat[W4Y]*_mat[X4W]*_mat[Z4Z],
	_mat[X4Y]*_mat[Y4Z]*_mat[W4W]-_mat[X4Y]*_mat[Y4W]*_mat[W4Z]-_mat[Y4Y]*_mat[X4Z]*_mat[W4W]
		+_mat[Y4Y]*_mat[X4W]*_mat[W4Z]+_mat[W4Y]*_mat[X4Z]*_mat[Y4W]-_mat[W4Y]*_mat[X4W]*_mat[Y4Z],
	-_mat[X4Y]*_mat[Y4Z]*_mat[Z4W]+_mat[X4Y]*_mat[Y4W]*_mat[Z4Z]+_mat[Y4Y]*_mat[X4Z]*_mat[Z4W]
		-_mat[Y4Y]*_mat[X4W]*_mat[Z4Z]-_mat[Z4Y]*_mat[X4Z]*_mat[Y4W]+_mat[Z4Y]*_mat[X4W]*_mat[Y4Z],
	-_mat[Z4X]*_mat[Y4W]*_mat[W4Z]-_mat[W4X]*_mat[Y4Z]*_mat[Z4W]-_mat[Y4X]*_mat[Z4Z]*_mat[W4W]
		+_mat[Y4X]*_mat[Z4W]*_mat[W4Z]+_mat[Z4X]*_mat[Y4Z]*_mat[W4W]+_mat[W4X]*_mat[Y4W]*_mat[Z4Z],
	_mat[X4X]*_mat[Z4Z]*_mat[W4W]-_mat[X4X]*_mat[Z4W]*_mat[W4Z]-_mat[Z4X]*_mat[X4Z]*_mat[W4W]
		+_mat[Z4X]*_mat[X4W]*_mat[W4Z]+_mat[W4X]*_mat[X4Z]*_mat[Z4W]-_mat[W4X]*_mat[X4W]*_mat[Z4Z],
	 -_mat[X4X]*_mat[Y4Z]*_mat[W4W]+_mat[X4X]*_mat[Y4W]*_mat[W4Z]+_mat[Y4X]*_mat[X4Z]*_mat[W4W]
		-_mat[Y4X]*_mat[X4W]*_mat[W4Z]-_mat[W4X]*_mat[X4Z]*_mat[Y4W]+_mat[W4X]*_mat[X4W]*_mat[Y4Z],
	_mat[X4X]*_mat[Y4Z]*_mat[Z4W]-_mat[X4X]*_mat[Y4W]*_mat[Z4Z]-_mat[Y4X]*_mat[X4Z]*_mat[Z4W]
		+_mat[Y4X]*_mat[X4W]*_mat[Z4Z]+_mat[Z4X]*_mat[X4Z]*_mat[Y4W]-_mat[Z4X]*_mat[X4W]*_mat[Y4Z],
	_mat[Y4X]*_mat[Z4Y]*_mat[W4W]-_mat[Y4X]*_mat[Z4W]*_mat[W4Y]-_mat[Z4X]*_mat[Y4Y]*_mat[W4W]
		+_mat[Z4X]*_mat[Y4W]*_mat[W4Y]+_mat[W4X]*_mat[Y4Y]*_mat[Z4W]-_mat[W4X]*_mat[Y4W]*_mat[Z4Y],
	-_mat[X4X]*_mat[Z4Y]*_mat[W4W]+_mat[X4X]*_mat[Z4W]*_mat[W4Y]+_mat[Z4X]*_mat[X4Y]*_mat[W4W]
		-_mat[Z4X]*_mat[X4W]*_mat[W4Y]-_mat[W4X]*_mat[X4Y]*_mat[Z4W]+_mat[W4X]*_mat[X4W]*_mat[Z4Y],
	 _mat[X4X]*_mat[Y4Y]*_mat[W4W]-_mat[X4X]*_mat[Y4W]*_mat[W4Y]-_mat[Y4X]*_mat[X4Y]*_mat[W4W]
		+_mat[Y4X]*_mat[X4W]*_mat[W4Y]+_mat[W4X]*_mat[X4Y]*_mat[Y4W]-_mat[W4X]*_mat[X4W]*_mat[Y4Y],
	-_mat[X4X]*_mat[Y4Y]*_mat[Z4W]+_mat[X4X]*_mat[Y4W]*_mat[Z4Y]+_mat[Y4X]*_mat[X4Y]*_mat[Z4W]
		-_mat[Y4X]*_mat[X4W]*_mat[Z4Y]-_mat[Z4X]*_mat[X4Y]*_mat[Y4W]+_mat[Z4X]*_mat[X4W]*_mat[Y4Y],
	-_mat[Y4X]*_mat[Z4Y]*_mat[W4Z]+_mat[Y4X]*_mat[Z4Z]*_mat[W4Y]+_mat[Z4X]*_mat[Y4Y]*_mat[W4Z]
		-_mat[Z4X]*_mat[Y4Z]*_mat[W4Y]-_mat[W4X]*_mat[Y4Y]*_mat[Z4Z]+_mat[W4X]*_mat[Y4Z]*_mat[Z4Y],
	_mat[X4X]*_mat[Z4Y]*_mat[W4Z]-_mat[X4X]*_mat[Z4Z]*_mat[W4Y]-_mat[Z4X]*_mat[X4Y]*_mat[W4Z]
		+_mat[Z4X]*_mat[X4Z]*_mat[W4Y]+_mat[W4X]*_mat[X4Y]*_mat[Z4Z]-_mat[W4X]*_mat[X4Z]*_mat[Z4Y],
	-_mat[X4X]*_mat[Y4Y]*_mat[W4Z]+_mat[X4X]*_mat[Y4Z]*_mat[W4Y]+_mat[Y4X]*_mat[X4Y]*_mat[W4Z]
		-_mat[Y4X]*_mat[X4Z]*_mat[W4Y]-_mat[W4X]*_mat[X4Y]*_mat[Y4Z]+_mat[W4X]*_mat[X4Z]*_mat[Y4Y],
	_mat[X4X]*_mat[Y4Y]*_mat[Z4Z]-_mat[X4X]*_mat[Y4Z]*_mat[Z4Y]-_mat[Y4X]*_mat[X4Y]*_mat[Z4Z]
		+_mat[Y4X]*_mat[X4Z]*_mat[Z4Y]+_mat[Z4X]*_mat[X4Y]*_mat[Y4Z]-_mat[Z4X]*_mat[X4Z]*_mat[Y4Y]);
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::inverse() const
{
	double det = determinant();
	HLASSERT(det != 0.0, "Matrix4X4T::inverse(): Singular matrix:\n" << *this);
	return adjoint() / det;
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::safeInverse() const
{
	double det = determinant();
	if (!det) return adjoint() * 1e14;
	return adjoint() / det;
}

template <class Obj> void Matrix4X4T<Obj>::display(HLSTD::ostream & out) const
{
	out << endl;
	for (int i = 0; i < 4; i ++)
	{
		out << "[ ";
		for (int j = 0; j < 4; j ++)
		{
			out << (*this)(i,j) << " ";
		}
		out << "]" << endl;
	}	
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::HPB(VectorT<Obj> const & hpb)
{
	// [OPTIMIZEME] just return the whole kitten kaboodle
	Matrix4X4T<Obj> m(VectorT<Obj>(cos(hpb[2]), -sin(hpb[2]), 0), VectorT<Obj>(sin(hpb[2]), cos(hpb[2]), 0), VectorT<Obj>(0, 0, 1));
	m *= Matrix4X4T<Obj>(VectorT<Obj>(1, 0, 0), VectorT<Obj>(0, cos(hpb[1]), -sin(hpb[1])), VectorT<Obj>(0, sin(hpb[1]), cos(hpb[1])));
	m *= Matrix4X4T<Obj>(VectorT<Obj>(cos(hpb[0]), 0, -sin(hpb[0])), VectorT<Obj>(0, 1, 0), VectorT<Obj>(sin(hpb[0]), 0, cos(hpb[0])));
	return m;
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::Scale(VectorT<Obj> const & scale)
{
	return Matrix4X4T<Obj>(VectorT<Obj>(scale[0], 0, 0), VectorT<Obj>(0, scale[1], 0), VectorT<Obj>(0, 0, scale[2]));
}

/*
// [FIXME] incorporate this yummy "rotate around an axis" code into its own function:
	rcos = cos(phi);
	rsin = sin(phi);
	matrix[0][0] =      rcos + u*u*(1-rcos);
	matrix[1][0] =  w * rsin + v*u*(1-rcos);
	matrix[2][0] = -v * rsin + w*u*(1-rcos);
	matrix[0][1] = -w * rsin + u*v*(1-rcos);
	matrix[1][1] =      rcos + v*v*(1-rcos);
	matrix[2][1] =  u * rsin + w*v*(1-rcos);
	matrix[0][2] =  v * rsin + u*w*(1-rcos);
	matrix[1][2] = -u * rsin + v*w*(1-rcos);
	matrix[2][2] =      rcos + w*w*(1-rcos);
*/

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::AlignTo(VectorT<Obj> const & from, VectorT<Obj> const & to)
{
	// [FIXME] reference
	// rotates 'from' to 'to', the short way around
	// [NOTE] assumes 'from' and 'to' are normal vectors
	Obj e = from * to;
	if (e == (Obj)1.0) return Matrix4X4T<Obj>::Identity();
	Obj denom = ((Obj)1.0 + e);
	if (!denom)	// exactly opposite, so "short way" is undefined; 
	{
		// choose a vector for the rotation axis
		VectorT<Obj> v = from % VectorT<Obj>::YHat();
		// if we chose a bad one, choose another one
		if (!!v) v = from % VectorT<Obj>::ZHat();
		v = ~v;
		// rotate 180 degrees around v
		Matrix4X4T<Obj> m;
		m._mtx[0][0] = v[0]*v[0]*2 - 1.0;
		m._mtx[1][0] = v[1]*v[0]*2;
		m._mtx[2][0] = v[2]*v[0]*2;
		m._mtx[0][1] = v[0]*v[1]*2;
		m._mtx[1][1] = v[1]*v[1]*2 - 1.0;
		m._mtx[2][1] = v[2]*v[1]*2;
		m._mtx[0][2] = v[0]*v[2]*2;
		m._mtx[1][2] = v[1]*v[2]*2;
		m._mtx[2][2] = v[2]*v[2]*2 - 1.0;
		m._mat[W4X] = (Obj)0.0; m._mat[W4Y] = (Obj)0.0; m._mat[W4Z] = (Obj)0.0;
		m._mat[X4W] = (Obj)0.0; m._mat[Y4W] = (Obj)0.0; m._mat[Z4W] = (Obj)0.0; m._mat[W4W] = (Obj)1.0;
		return m;
	}
	VectorT<Obj> v = from % to;
	VectorT<Obj> hv = v / denom;
	Obj hvxy = hv[0] * v[1];
	Obj hvxz = hv[0] * v[2];
	Obj hvyz = hv[2] * v[1];
	Matrix4X4T<Obj> m;
	m._mtx[0][0] = e + hv[0] * v[0];
	m._mtx[1][0] = hvxy + v[2];
	m._mtx[2][0] = hvxz - v[1];
	m._mtx[0][1] = hvxy - v[2];
	m._mtx[1][1] = e + hv[1] * v[1];
	m._mtx[2][1] = hvyz + v[0];
	m._mtx[0][2] = hvxz + v[1];
	m._mtx[1][2] = hvyz - v[0];
	m._mtx[2][2] = e + hv[2] * v[2];
	m._mat[W4X] = (Obj)0.0; m._mat[W4Y] = (Obj)0.0; m._mat[W4Z] = (Obj)0.0;
	m._mat[X4W] = (Obj)0.0; m._mat[Y4W] = (Obj)0.0; m._mat[Z4W] = (Obj)0.0; m._mat[W4W] = (Obj)1.0;
	return m;
	// [OPTIMIZEME] just return the whole kitten kaboodle
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::PointAt(VectorT<Obj> const & to)
{
	VectorT<Obj> z = ~to;
	VectorT<Obj> x = ~(Vector::YHat() % z);
	VectorT<Obj> y = ~(z % x);
	return Matrix4X4T<Obj>(x, y, z);
}

template <class Obj> Matrix4X4T<Obj> Matrix4X4T<Obj>::Translate(VectorT<Obj> const & offset)
{
	return Matrix4X4T<Obj>(1,0,0,offset[0], 0,1,0,offset[1], 0,0,1,offset[2], 0,0,0,1);
}

#endif
