
#ifndef _HLMATRIX_H
#define _HLMATRIX_H

//
//	This file contains the MatrixT class, which is a general-purpose matrix type.
//  It also includes the headers for Matrix3X3T and Matrix4X4T, which are hard-coded-size matrix types
//

//	MATRIX CONVENTIONS:

//	VectorT and PointT are conidered column vectors, i.e. a 1x4 Matrix (1x3 + implicit w).
//	The w component of PointT is implicitly 1.
//	The w component of VectorT is implicitly 0.

//	MatrixT is row-major, i.e. [[xx xy xz xw] [yx yy yz yw] [zx zy zz zw] [wx wy wz ww]] for 4x4.
//  Note that a 3x3 Matrix is implicitly [[xx xy xz 0] [yx yy yz 0] [zx zy zz 0] [wx wy wz 1]].

//	Thus to transform a point by a matrix we use newPoint = Mat * oldPoint.

#ifndef _HLPOINT_H
#include "HLPoint.h"
#endif
#ifndef _HLCOLOR_H
#include "HLColor.h"
#endif
#ifndef _HLMATRIX3X3_H
#include "HLMatrix3X3.h"
#endif
#ifndef _HLMATRIX4X4_H
#include "HLMatrix4X4.h"
#endif

template <class Obj> class MatrixT
{
public:
	int _rows;
	int _cols;
	Obj * _mat;

	MatrixT() : _rows(3), _cols(3) { _mat = new Obj[9]; }
	MatrixT(int r, int c) : _rows(r), _cols(c) { _mat = new Obj[_rows*_cols]; }
	MatrixT(Obj const * m, int r, int c) : _rows(r), _cols(c)
	{ _mat = new Obj[_rows*_cols]; memcpy(_mat, m, sizeof(Obj)*_rows*_cols); }
	MatrixT(double, double, double, double, double, double, double, double, double);
	MatrixT(VectorT<Obj> const & vx, VectorT<Obj> const & vy, VectorT<Obj> const & vz)
	: _rows(3), _cols(3)
	{
		// create a transformation matrix from Right, Up, Forward
		 _mat = new Matrix3X3T<Obj>(vx, vy, vz);
	}
	template<class Obj2> MatrixT(MatrixT<Obj2> const & m) : _rows(m._rows), _cols(m._cols)
	{
		_mat = new Obj[_rows*_cols];
		for (int i = 0; i < _rows*_cols; i ++) _mat[i] = (Obj)m._mat[i];
	}
	template<class Obj2> MatrixT & operator = (MatrixT<Obj2> const & m)
	{
		delete[] _mat;
		_rows = m._rows;
		_cols = m._cols;
		for (int i = 0; i < _rows*_cols; i ++) _mat[i] = (Obj)m._mat[i];
		return *this;
	}
	~MatrixT() { delete _mat; }
	void init(int r, int c) { if (r*c != _rows*_cols) { delete _mat; _mat = new Obj[r*c]; } _rows = r; _cols = c; }
	void set(Obj const * m, int r, int c) { init(r,c); memcpy(_mat, m, sizeof(Obj)*_rows*_cols); }
	void get(Obj * m) const { memcpy(m, _mat, sizeof(Obj)*_rows*_cols); }
	MatrixT & operator = (MatrixT const & m) { set(m._mat, m._rows, m._cols); return *this; }
	template <class Obj2> Obj2 wTrans(PointT<Obj2> const & src, PointT<Obj2> & dest) const
	{
		// wTrans performs Point = Mat * Point, but returns w instead of dividing through by w.
		if (_cols == 3 && _rows == 3) return ((Matrix3X3T<Obj>*)_mat)->wTrans(src, dest);
		if (_cols == 4 && _rows == 4) return ((Matrix4X4T<Obj>*)_mat)->wTrans(src, dest);
		HLASSERT(0, "MatrixT::wTrans(PointT,PointT): Matrix is a bad size:\n" << *this);
		return 0.0;
	}
	template <class Obj2> VectorT<Obj2> normTrans(VectorT<Obj2> const & norm) const
	{
		// normTrans transforms a surface normal
		if (_cols == 3 && _rows == 3) return ((Matrix3X3T<Obj>*)_mat)->normTrans(norm);
		if (_cols == 4 && _rows == 4) return ((Matrix4X4T<Obj>*)_mat)->normTrans(norm);
		HLASSERT(0, "MatrixT::normTrans(VectorT): Matrix is a bad size:\n" << *this);
		return VectorT<Obj2>();
	}
	double determinant() const;	// determinant of the matrix
	MatrixT transposed() const;	// transposed matrix
	MatrixT adjoint() const;	// upper-left part of the adjoint matrix (use determinant to get the rest)
	MatrixT inverse() const;	// inverse of the matrix (adjoint() / determinant())
	void transpose() { *this = transposed(); }	// transpose the matrix
	void invert() { *this = inverse(); }		// invert the matrix
	MatrixT & operator += (MatrixT const &);
	MatrixT & operator -= (MatrixT const &);
	MatrixT & operator *= (double);
	MatrixT & operator *= (MatrixT const &);
	MatrixT & operator &= (MatrixT const &);
	MatrixT & operator /= (MatrixT const &);
	int operator == (MatrixT const &) const;
	int operator != (MatrixT const & m) const { return !operator==(m); }
	operator Obj*() { return _mat; }
	operator const Obj*() const { return _mat; }
	Obj & operator () (int r, int c) { return _mat[r * _cols + c]; }
	const Obj & operator () (int r, int c) const { return _mat[r * _cols + c]; }
	void display(HLSTD::ostream &) const;
};

typedef MatrixT<double> Matrix;
typedef MatrixT<float> Matrixf;
typedef MatrixT<WeeFloat> Matrixi;


//// non-member functions ////

template <class Obj, class Obj2> VectorT<Obj> operator * (MatrixT<Obj2> const &, VectorT<Obj> const &);				// transform vector
template <class Obj, class Obj2> PointT<Obj> operator * (MatrixT<Obj2> const &, PointT<Obj> const &);				// transform point
template <class Obj, class Obj2> ColorT<Obj> operator * (MatrixT<Obj2> const & m, ColorT<Obj> const & a) { return ColorT<Obj>(m * VectorT<Obj>(a)); }

template <class Obj> HLSTD::ostream & operator << (HLSTD::ostream & out, MatrixT<Obj> const & m)
{
	m.display(out);
	return out;
}

template <class Obj, class Obj2> VectorT<Obj> operator * (MatrixT<Obj2> const & m, VectorT<Obj> const & v)
{
	if (m._cols == 3 && m._rows == 3) return *(Matrix3X3T<Obj2>*)m._mat * v;
	if (m._cols == 4 && m._rows == 4) return *(Matrix4X4T<Obj2>*)m._mat * v;
	HLASSERT(0, "MatrixT * VectorT: Matrix is a bad size:\n" << m);
	return VectorT<Obj>();
}

template <class Obj, class Obj2> PointT<Obj> operator * (MatrixT<Obj2> const & m, PointT<Obj> const & pt)
{
	if (m._cols == 3 && m._rows == 3) return *(Matrix3X3T<Obj2>*)m._mat * pt;
	if (m._cols == 4 && m._rows == 4) return *(Matrix4X4T<Obj2>*)m._mat * pt;
	HLASSERT(0, "MatrixT * PointT: Matrix is a bad size:\n" << m);
	return PointT<Obj>();
}

template <class Obj> MatrixT<Obj> operator + (MatrixT<Obj> a, MatrixT<Obj> const & b) { return a += b; }
template <class Obj> MatrixT<Obj> operator - (MatrixT<Obj> a, MatrixT<Obj> const & b) { return a -= b; }
template <class Obj> MatrixT<Obj> operator * (MatrixT<Obj> a, MatrixT<Obj> const & b) { return a *= b; }
template <class Obj> MatrixT<Obj> operator & (MatrixT<Obj> a, MatrixT<Obj> const & b) { return a &= b; }
template <class Obj> MatrixT<Obj> operator / (MatrixT<Obj> a, MatrixT<Obj> const & b) { return a /= b; }
template <class Obj> MatrixT<Obj> operator * (MatrixT<Obj> a, double b) { return a *= b; }
template <class Obj> MatrixT<Obj> operator / (MatrixT<Obj> a, double b) { return a *= 1.0 / b; }


//// MatrixT implementation ////

template <class Obj>
MatrixT<Obj>::MatrixT(double m0, double m1, double m2, double m3, double m4, double m5, double m6, double m7, double m8)
{
	_rows =_cols = 0;
	_mat = 0;
	init(3,3);
	_mat[X3X] = (Obj)m0; _mat[X3Y] = (Obj)m1; _mat[X3Z] = (Obj)m2;
	_mat[Y3X] = (Obj)m3; _mat[Y3Y] = (Obj)m4; _mat[Y3Z] = (Obj)m5;
	_mat[Z3X] = (Obj)m6; _mat[Z3Y] = (Obj)m7; _mat[Z3Z] = (Obj)m8;
}

template <class Obj> MatrixT<Obj> & MatrixT<Obj>::operator += (MatrixT const & m)
{
	for (int i = 0; i < _rows*_cols; i ++) _mat[i] += m[i];
	return *this;
}

template <class Obj> MatrixT<Obj> & MatrixT<Obj>::operator -= (MatrixT const & m)
{
	for (int i = 0; i < _rows*_cols; i ++) _mat[i] -= m[i];
	return *this;
}

template <class Obj> MatrixT<Obj> & MatrixT<Obj>::operator &= (MatrixT const & m)
{
	for (int i = 0; i < _rows*_cols; i ++) _mat[i] *= m[i];
	return *this;
}

template <class Obj> MatrixT<Obj> & MatrixT<Obj>::operator /= (MatrixT const & m)
{
	for (int i = 0; i < _rows*_cols; i ++) _mat[i] /= m[i];
	return *this;
}

template <class Obj> MatrixT<Obj> & MatrixT<Obj>::operator *= (double d)
{
	for (int i = 0; i < _rows*_cols; i ++) _mat[i] *= d;
	return *this;
}

template <class Obj> MatrixT<Obj> & MatrixT<Obj>::operator *= (MatrixT const & b)
{
	if (b._cols == 3 && b._rows == 3) { *(Matrix3X3T<Obj>*)_mat *= *(Matrix3X3T<Obj>*)&b; return *this; }
	if (b._cols == 4 && b._rows == 4) { *(Matrix4X4T<Obj>*)_mat *= *(Matrix4X4T<Obj>*)&b; return *this; }
	HLASSERT(_cols == b._rows,
		"MatrixT::operator *= MatrixT: Incompatable sizes:\n" << *this << endl << b);
	MatrixT c(_rows, b._cols);
	int i, j, k, ai, bi, ci = 0;
	for (i = 0; i < c._rows; i++) for (j = 0; j < c._cols; j++)
	{
		ai = i * _cols; bi = j;
		c[ci] = 0.0;
		for (k = 0; k < _cols; k++)
		{
			c[ci] += _mat[ai] * b[bi];
			ai ++;
			bi += b._cols;
		}
		ci ++;
	}
	return *this = c;
}

template <class Obj> int MatrixT<Obj>::operator == (MatrixT<Obj> const & m) const
{
	for (int i = 0; i < _rows * _cols; i ++) if (_mat[i] != m._mat[i]) return 0;
	return 1;
}

template <class Obj> MatrixT<Obj> MatrixT<Obj>::transposed() const
{
	if (_cols == 3 && _rows == 3) return MatrixT<Obj>(((Matrix3X3T<Obj>*)_mat)->transposed(), 3, 3);
	if (_cols == 4 && _rows == 4) return MatrixT<Obj>(((Matrix4X4T<Obj>*)_mat)->transposed(), 4, 4);
	MatrixT b(_cols, _rows);
	for (int i = 0; i < _rows; i ++) for (int j = 0; j < _cols; j ++) b(j, i) = (*this)(i, j);
	return b;
}

template <class Obj> double MatrixT<Obj>::determinant() const
{
	HLASSERT(_rows == _cols,
		"MatrixT::determinant(): Non-square matrix:\n" << *this);
	switch (_rows)
	{
	case 1:	return _mat[0];
	case 2:	return _mat[0]*_mat[3]-_mat[2]*_mat[1];
	case 3:	return ((Matrix3X3T<Obj>*)_mat)->determinant();
	case 4:	return ((Matrix4X4T<Obj>*)_mat)->determinant();
	}
	// [FIXME] add general case
	HLASSERT(0, "MatrixT::determinant(): Unsupported size:\n" << *this);
	return 0.0;
}

template <class Obj> MatrixT<Obj> MatrixT<Obj>::adjoint() const
{
	HLASSERT(_rows == _cols,
		"MatrixT::adjoint(): Non-square matrix:\n" << *this);
	MatrixT m2(_rows, _cols);
	switch(_rows)
	{
	case 1:
		m2[0] = (Obj)1.0;
		return m2;
	case 2:
		m2[0] = _mat[3];
		m2[1] = -_mat[1];
		m2[2] = -_mat[2];
		m2[3] = _mat[0];
		return m2;
	case 3:
		m2.set(((Matrix3X3T<Obj>*)_mat)->adjoint(), 3, 3);
		return m2;
	case 4:
		m2.set(((Matrix4X4T<Obj>*)_mat)->adjoint(), 4, 4);
		return m2;
	}
	// [FIXME] add general case
	HLASSERT(0, "MatrixT::adjoint(): Unsupported size:\n" << *this);
	return m2;
}

template <class Obj> MatrixT<Obj> MatrixT<Obj>::inverse() const
{
	HLASSERT(_rows == _cols,
		"MatrixT::inverse(): Non-square matrix:\n" << *this);
	if (_rows <= 4) 
	{
		double det = determinant();
		HLASSERT(det != 0.0, "MatrixT::inverse(): Singular matrix:\n" << *this);
		return adjoint() / det;
	}
	// General Matrix inversion as per Topics in Advanced Scientific Computation
	// [NOTE] Doesn't detect all singular matrices.
	int i;
	MatrixT b, x;
	Obj scale = 0.0;
	// compute scale
	for (i = 0; i < _rows * _cols; i++) scale += _mat[i];
	HLASSERT(scale != 0.0, "MatrixT::inverse(): Singular matrix:\n" << *this);
	scale = 0.1 / sqrt(scale);
	// compute b and x
	(x = b = *this * scale).transpose();
	// iterate (expect about 15 places accuracy)
	for (i = 0; i < 25; i++) x = x * 2.0 - (x * b) * x;
	// scale result
	return x * scale;
}

template <class Obj> void MatrixT<Obj>::display(HLSTD::ostream & out) const
{
	out << endl;
	for (int i = 0; i < _rows; i ++)
	{
		out << "[ ";
		for (int j = 0; j < _cols; j ++)
		{
			out << (*this)(i,j) << " ";
		}
		out << "]" << endl;
	}	
}

#endif
