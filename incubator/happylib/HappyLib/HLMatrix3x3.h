
#ifndef _HLMATRIX3X3_H
#define _HLMATRIX3X3_H

//
//	This file contains the Matrix3X3T class, which is a 3x3 matrix type.
//

//	MATRIX CONVENTIONS:

//	VectorT and PointT are conidered column vectors, i.e. a 1x4 Matrix (1x3 + implicit w).
//	The w component of PointT is implicitly 1.
//	The w component of VectorT is implicitly 0.

//	Matrix3X3T is row-major, i.e. [[xx xy xz] [yx yy yz] [zx zy zz]].
//  Note that a 3x3 Matrix is implicitly [[xx xy xz 0] [yx yy yz 0] [zx zy zz 0] [0 0 0 1]].

//	Thus to transform a point by a matrix we use newPoint = Mat * oldPoint.

#ifndef _HLPOINT_H
#include "HLPoint.h"
#endif
#ifndef _HLCOLOR_H
#include "HLColor.h"
#endif

enum MatCell3{X3X,X3Y,X3Z, Y3X,Y3Y,Y3Z, Z3X,Z3Y,Z3Z};

template <class Obj> class Matrix3X3T
{
public:
	Obj _mat[9];

	enum Axis{X,Y,Z,W};

	Matrix3X3T() {}
	Matrix3X3T(Obj const * m) { for (int i = 0; i < 9; i ++) _mat[i] = m[i]; }
	Matrix3X3T(double, double, double, double, double, double, double, double, double);
	Matrix3X3T(VectorT<Obj> const & vx, VectorT<Obj> const & vy, VectorT<Obj> const & vz)
	{
		// create a transformation matrix from Right, Up, Forward
		_mat[X3X] = vx[X]; _mat[Y3X] = vx[Y]; _mat[Z3X] = vx[Z];
		_mat[X3Y] = vy[X]; _mat[Y3Y] = vy[Y]; _mat[Z3Y] = vy[Z];
		_mat[X3Z] = vz[X]; _mat[Y3Z] = vz[Y]; _mat[Z3Z] = vz[Z];
	}
	template<class Obj2> Matrix3X3T(Matrix3X3T<Obj2> const & m)
	{
		for (int i = 0; i < 9; i ++) _mat[i] = (Obj)m._mat[i];
	}
	template<class Obj2> Matrix3X3T & operator = (Matrix3X3T<Obj2> const & m)
	{
		for (int i = 0; i < 9; i ++) _mat[i] = (Obj)m._mat[i];
		return *this;
	}
	void set(Obj const * m) { for (int i = 0; i < 9; i ++) _mat[i] = m[i]; }
	void get(Obj * m) const { for (int i = 0; i < 9; i ++) m[i] = _mat[i]; }
	template <class Obj2> Obj2 wTrans(PointT<Obj2> const & src, PointT<Obj2> & dest) const
	{
		// wTrans performs Point = Mat * Point, but returns w instead of dividing through by w.
		dest[X] = src[X]*_mat[X3X] + src[Y]*_mat[X3Y] + src[Z]*_mat[X3Z];
		dest[Y] = src[X]*_mat[Y3X] + src[Y]*_mat[Y3Y] + src[Z]*_mat[Y3Z];
		dest[Z] = src[X]*_mat[Z3X] + src[Y]*_mat[Z3Y] + src[Z]*_mat[Z3Z];
		// [NOTE] w is 1, because matrix w is implicitly [0 0 0 1] and pt[W] is implicitly 1
		return Obj2(1.0);
	}
	template <class Obj2> VectorT<Obj2> transposeTrans(VectorT<Obj2> const & v) const
	{
		// transforms v by the transpose
		// [NOTE] for surface normals use ~inverseMat.transposeTrans(norm)
		return VectorT<Obj2>(
			Obj2(v[X]*_mat[X3X] + v[Y]*_mat[Y3X] + v[Z]*_mat[Z3X]),
			Obj2(v[X]*_mat[X3Y] + v[Y]*_mat[Y3Y] + v[Z]*_mat[Z3Y]),
			Obj2(v[X]*_mat[X3Z] + v[Y]*_mat[Y3Z] + v[Z]*_mat[Z3Z]));
	}
	double determinant() const;	// determinant of the matrix
	Matrix3X3T transposed() const;	// transposed matrix
	Matrix3X3T adjoint() const;	// upper-left part of the adjoint matrix (use determinant to get the rest)
	Matrix3X3T inverse() const;	// inverse of the matrix (adjoint() / determinant())
	Matrix3X3T safeInverse() const;	// inverse of the matrix (adjoint() / determinant())
	void transpose() { *this = transposed(); }	// transpose the matrix
	void invert() { *this = inverse(); }		// invert the matrix
	Matrix3X3T & operator += (Matrix3X3T const &);
	Matrix3X3T & operator -= (Matrix3X3T const &);
	Matrix3X3T & operator *= (double);
	Matrix3X3T & operator *= (Matrix3X3T const &);
	Matrix3X3T & operator &= (Matrix3X3T const &);
	Matrix3X3T & operator /= (Matrix3X3T const &);
	int operator == (Matrix3X3T const &) const;
	int operator != (Matrix3X3T const & m) const { return !operator==(m); }
	VectorT<Obj> solve(VectorT<Obj> const & v) const;
	operator Obj*() { return _mat; }
	operator const Obj*() const { return _mat; }
	Obj & operator () (int r, int c) { return _mat[r * 3 + c]; }
	const Obj & operator () (int r, int c) const { return _mat[r * 3 + c]; }
	void display(HLSTD::ostream &) const;
};

typedef Matrix3X3T<double> Matrix3X3;
typedef Matrix3X3T<float> Matrix3X3f;
typedef Matrix3X3T<WeeFloat> Matrix3X3i;


//// non-member functions ////

template <class Obj, class Obj2> VectorT<Obj> operator * (Matrix3X3T<Obj2> const &, VectorT<Obj> const &);				// transform vector
template <class Obj, class Obj2> PointT<Obj> operator * (Matrix3X3T<Obj2> const &, PointT<Obj> const &);				// transform point
template <class Obj, class Obj2> ColorT<Obj> operator * (Matrix3X3T<Obj2> const & m, ColorT<Obj> const & a) { return ColorT<Obj>(m * VectorT<Obj>(a)); }

template <class Obj> HLSTD::ostream & operator << (HLSTD::ostream & out, Matrix3X3T<Obj> const & m)
{
	m.display(out);
	return out;
}

template <class Obj, class Obj2> VectorT<Obj> operator * (Matrix3X3T<Obj2> const & m, VectorT<Obj> const & v)
{
	//[NOTE] This is NOT the function you want for surface normals, which are not really directions!
	enum Axis{X,Y,Z,W};
	return VectorT<Obj>(
		Obj(v[X]*m[X3X] + v[Y]*m[X3Y] + v[Z]*m[X3Z]),
		Obj(v[X]*m[Y3X] + v[Y]*m[Y3Y] + v[Z]*m[Y3Z]),
		Obj(v[X]*m[Z3X] + v[Y]*m[Z3Y] + v[Z]*m[Z3Z]));
}

template <class Obj, class Obj2> PointT<Obj> operator * (Matrix3X3T<Obj2> const & m, PointT<Obj> const & pt)
{
	enum Axis{X,Y,Z,W};
	return PointT<Obj>(
		Obj(pt[X]*m[X3X] + pt[Y]*m[X3Y] + pt[Z]*m[X3Z]),
		Obj(pt[X]*m[Y3X] + pt[Y]*m[Y3Y] + pt[Z]*m[Y3Z]),
		Obj(pt[X]*m[Z3X] + pt[Y]*m[Z3Y] + pt[Z]*m[Z3Z]));
	// [NOTE] w is 1, because matrix w is implicitly [0 0 0 1] and pt[W] is implicitly 1
}

template <class Obj> Matrix3X3T<Obj> operator + (Matrix3X3T<Obj> a, Matrix3X3T<Obj> const & b) { return a += b; }
template <class Obj> Matrix3X3T<Obj> operator - (Matrix3X3T<Obj> a, Matrix3X3T<Obj> const & b) { return a -= b; }
template <class Obj> Matrix3X3T<Obj> operator * (Matrix3X3T<Obj> a, Matrix3X3T<Obj> const & b) { return a *= b; }
template <class Obj> Matrix3X3T<Obj> operator & (Matrix3X3T<Obj> a, Matrix3X3T<Obj> const & b) { return a &= b; }
template <class Obj> Matrix3X3T<Obj> operator / (Matrix3X3T<Obj> a, Matrix3X3T<Obj> const & b) { return a /= b; }
template <class Obj> Matrix3X3T<Obj> operator * (Matrix3X3T<Obj> a, double b) { return a *= b; }
template <class Obj> Matrix3X3T<Obj> operator / (Matrix3X3T<Obj> a, double b) { return a *= 1.0 / b; }
template <class Obj> VectorT<Obj> operator / (VectorT<Obj> const & a, Matrix3X3T<Obj> const & b) { return b.solve(a); }


//// Matrix3X3T implementation ////

template <class Obj>
Matrix3X3T<Obj>::Matrix3X3T(double m0, double m1, double m2, double m3, double m4, double m5, double m6, double m7, double m8)
{
	_mat[X3X] = (Obj)m0; _mat[X3Y] = (Obj)m1; _mat[X3Z] = (Obj)m2;
	_mat[Y3X] = (Obj)m3; _mat[Y3Y] = (Obj)m4; _mat[Y3Z] = (Obj)m5;
	_mat[Z3X] = (Obj)m6; _mat[Z3Y] = (Obj)m7; _mat[Z3Z] = (Obj)m8;
}

template <class Obj> Matrix3X3T<Obj> & Matrix3X3T<Obj>::operator += (Matrix3X3T const & m)
{
	for (int i = 0; i < 9; i ++) _mat[i] += m[i];
	return *this;
}

template <class Obj> Matrix3X3T<Obj> & Matrix3X3T<Obj>::operator -= (Matrix3X3T const & m)
{
	for (int i = 0; i < 9; i ++) _mat[i] -= m[i];
	return *this;
}

template <class Obj> Matrix3X3T<Obj> & Matrix3X3T<Obj>::operator &= (Matrix3X3T const & m)
{
	for (int i = 0; i < 9; i ++) _mat[i] *= m[i];
	return *this;
}

template <class Obj> Matrix3X3T<Obj> & Matrix3X3T<Obj>::operator /= (Matrix3X3T const & m)
{
	for (int i = 0; i < 9; i ++) _mat[i] /= m[i];
	return *this;
}

template <class Obj> Matrix3X3T<Obj> & Matrix3X3T<Obj>::operator *= (double d)
{
	for (int i = 0; i < 9; i ++) _mat[i] *= d;
	return *this;
}

template <class Obj> Matrix3X3T<Obj> & Matrix3X3T<Obj>::operator *= (Matrix3X3T const & b)
{
	Matrix3X3T c;
	int ci = 0;
	for (int i = 0; i < 3; i ++) for (int j = 0; j < 3; j ++)
	{
		int ai = i * 3;
		int bi = j;
		c[ci] = 0.0;
		for (int k = 0; k < 3; k++)
		{
			c[ci] += _mat[ai ++] * b[bi];
			bi += 3;
		}
		ci ++;
	}
	return *this = c;
}

template <class Obj> int Matrix3X3T<Obj>::operator == (Matrix3X3T<Obj> const & m) const
{
	for (int i = 0; i < 9; i ++) if (_mat[i] != m._mat[i]) return 0;
	return 1;
}

template <class Obj> Matrix3X3T<Obj> Matrix3X3T<Obj>::transposed() const
{
	// [OPTIMIZEME] unroll and return-value-optimize
	Matrix3X3T b;
	for (int i = 0; i < 3; i ++) for (int j = 0; j < 3; j ++) b(j, i) = (*this)(i, j);
	return b;
}

template <class Obj> double Matrix3X3T<Obj>::determinant() const
{
	// [OPTIMIZEME] A really good compiler should optimize this stuff pretty well,
	//              but last I checked I'm not using a really good compiler ;)
	return _mat[X3X]*_mat[Y3Y]*_mat[Z3Z]-_mat[X3X]*_mat[Z3Y]*_mat[Y3Z]
		-_mat[Y3X]*_mat[X3Y]*_mat[Z3Z]+_mat[Y3X]*_mat[Z3Y]*_mat[X3Z]
		+_mat[Z3X]*_mat[X3Y]*_mat[Y3Z]-_mat[Z3X]*_mat[Y3Y]*_mat[X3Z];
}

template <class Obj> Matrix3X3T<Obj> Matrix3X3T<Obj>::adjoint() const
{
	// [OPTIMIZEME] A really good compiler should optimize this stuff pretty well,
	//              but last I checked I'm not using a really good compiler ;)
	return Matrix3X3T(
		_mat[Y3Y]*_mat[Z3Z]-_mat[Z3Y]*_mat[Y3Z],
		-_mat[X3Y]*_mat[Z3Z]+_mat[Z3Y]*_mat[X3Z],
		_mat[X3Y]*_mat[Y3Z]-_mat[Y3Y]*_mat[X3Z],
		-_mat[Y3X]*_mat[Z3Z]+_mat[Z3X]*_mat[Y3Z],
		_mat[X3X]*_mat[Z3Z]-_mat[Z3X]*_mat[X3Z],
		-_mat[X3X]*_mat[Y3Z]+_mat[Y3X]*_mat[X3Z],
		_mat[Y3X]*_mat[Z3Y]-_mat[Z3X]*_mat[Y3Y],
		-_mat[X3X]*_mat[Z3Y]+_mat[Z3X]*_mat[X3Y],
		_mat[X3X]*_mat[Y3Y]-_mat[Y3X]*_mat[X3Y]);
}

template <class Obj> Matrix3X3T<Obj> Matrix3X3T<Obj>::inverse() const
{
	double det = determinant();
	HLASSERT(det != 0.0, "Matrix3X3T::inverse(): Singular matrix:\n" << *this);
	return adjoint() / det;
}

template <class Obj> Matrix3X3T<Obj> Matrix3X3T<Obj>::safeInverse() const
{
	double det = determinant();
	if (!det) return adjoint() * 1e14;
	return adjoint() / det;
}

template <class Obj> VectorT<Obj> Matrix3X3T<Obj>::solve(VectorT<Obj> const & v) const
{
	// solve a system of 3 linear equations in 3 variables; return 0s if no solution
	enum Axis{X,Y,Z,W};
	double denom = _mat[X3X] * (_mat[Y3Y] * _mat[Z3Z] - _mat[Y3Z] * _mat[Z3Y])
					+ _mat[X3Y] * (_mat[Y3Z] * _mat[Z3X] - _mat[Y3X] * _mat[Z3Z])
					+ _mat[X3Z] * (_mat[Y3X] * _mat[Z3Y] - _mat[Y3Y] * _mat[Z3X]);
	if (denom == 0.0) return VectorT<Obj>(0.0, 0.0, 0.0);
	double iDenom = 1.0 / denom;
	return VectorT<Obj>(
		(v[X] * (_mat[Z3Z]*_mat[Y3Y] - _mat[Y3Z]*_mat[Z3Y])
		+ v[Y] * (_mat[X3Z]*_mat[Z3Y] - _mat[Z3Z]*_mat[X3Y])
		+ v[Z] * (_mat[X3Y]*_mat[Y3Z] - _mat[X3Z]*_mat[Y3Y])) * iDenom,
		(v[X] * (_mat[Z3X]*_mat[Y3Z] - _mat[Y3X]*_mat[Z3Z])
		+ v[Y] * (_mat[Z3Z]*_mat[X3X] - _mat[Z3X]*_mat[X3Z])
		+ v[Z] * (_mat[Y3X]*_mat[X3Z] - _mat[X3X]*_mat[Y3Z])) * iDenom,
		(v[X] * (_mat[Y3X]*_mat[Z3Y] - _mat[Z3X]*_mat[Y3Y])
		+ v[Y] * (_mat[X3Y]*_mat[Z3X] - _mat[X3X]*_mat[Z3Y])
		+ v[Z] * (_mat[X3X]*_mat[Y3Y] - _mat[X3Y]*_mat[Y3X])) * iDenom);
}

template <class Obj> void Matrix3X3T<Obj>::display(HLSTD::ostream & out) const
{
	out << endl;
	for (int i = 0; i < 3; i ++)
	{
		out << "[ ";
		for (int j = 0; j < 3; j ++)
		{
			out << (*this)(i,j) << " ";
		}
		out << "]" << endl;
	}	
}

#endif
