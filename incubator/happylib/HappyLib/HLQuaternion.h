
#ifndef _HLQUATERNION_H
#define _HLQUATERNION_H

//
//	this file contains the 4-tuple classes: QuatT.
//
//	Some code is based on Ken Shoemake's code from Graphics Gems
//
//  HLQuaternion use the same coordinate system conventions as HLPoint.h:
//
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
//  Quaternion rotation handedness is the same as the cross product handedness
//


#ifndef _HLPOINT_H
#include "HLPoint.h"
#endif
#ifndef _HLMATRIX3X3_H
#include "HLMatrix3x3.h"
#endif
#ifndef _HLMATRIX4X4_H
#include "HLMatrix4x4.h"
#endif
#ifndef _HLTRIG_H
#include "HLTrig.h"
#endif

#define EPSILON 0.000001


//// QuatT classes ////

template <class Obj> class QuatT
{
public:
	enum { X = 0, Y = 1, Z = 2, W = 3 };
	Obj _val[4];
	// constructors
	QuatT() {}
	explicit QuatT(Obj const * f)
	{
		_val[X] = f[X]; _val[Y] = f[Y]; _val[Z] = f[Z]; _val[W] = f[W];
	}
	QuatT(Obj x, Obj y, Obj z, Obj w) { _val[X] = x; _val[Y] = y; _val[Z] = z; _val[W] = w; }
	QuatT(Matrix3X3T<Obj> const &);
	QuatT(Obj heading, Obj pitch, Obj yaw)
	{
		*this = QuatT(Vector::YHat(), heading);
		*this *= QuatT(Vector::XHat(), pitch);
		*this *= QuatT(Vector::ZHat(), yaw);
	}
	template <class Obj2> QuatT(VectorT<Obj2> const & axis)
	{
		// make Quat pointing zHat towards axis with +Y as up
		double pitch = -HLTrig::aSin(axis[1]);
		double heading = atan2(axis[0], axis[2]);
		*this = QuatT(Vector::YHat(), heading);
		*this *= QuatT(Vector::XHat(), pitch);
	}
	template <class Obj2> QuatT(VectorT<Obj2> const & axis, Obj2 theta)
	{
		// make Quat from <axis, angle> rotation.  Handedness of rotation is same as cross product
		_val[W] = (Obj)cos(theta*0.5);
		Obj2 sth = sin(theta*0.5);
		_val[X] = Obj(axis[X]*sth); _val[Y] = Obj(axis[Y]*sth); _val[Z] = Obj(axis[Z]*sth);
	}
	// accessors
	void get(Obj * f) const
	{
		f[X] = _val[X]; f[Y] = _val[Y]; f[Z] = _val[Z]; f[W] = _val[W];
	}
	template <class Obj2> void get(VectorT<Obj2> & axis, Obj2 & theta) const
	{
		double len2 = _val[X]*_val[X] + _val[Y]*_val[Y] + _val[Z]*_val[Z];
		if (len2 > 0.0)
		{
			theta = 2.0 * HLTrig::aCos(_val[W]);
			axis = v() / (sqrt(len2));
			return;
		}
		theta = 0;
		axis = VectorT<Obj2>::YHat();
	}
	VectorT<Obj> const & v() const { return *(VectorT<Obj>*)&_val[X]; }	// get vector part
	VectorT<Obj> & v() { return *(VectorT<Obj>*)&_val[X]; }	// get vector part
	// mutators
	void set(Obj x, Obj y, Obj z, Obj w)
	{
		_val[X] = x; _val[Y] = y; _val[Z] = z; _val[W] = w;
	}
	void set(Obj const * f)
	{
		_val[X] = f[X]; _val[Y] = f[Y]; _val[Z] = f[Z]; _val[W] = f[W];
	}
	// functions
	QuatT operator ~ ()	const		// conjugate
	{
		return QuatT<Obj>(-_val[X], -_val[Y], -_val[Z], _val[W]);
	}
	double norm() const
	{
		return Sq(_val[X]) + Sq(_val[Y]) + Sq(_val[Z]) + Sq(_val[W]);
	}
	QuatT inverse() const
	{
		double n = norm();
		if (n)
		{
			n = (Obj)1.0 / n;
			return QuatT<Obj>(-_val[X] * n, -_val[Y] * n, -_val[Z] * n, _val[W] * n);
		}
		return Quat((Obj)1, (Obj)0, (Obj)0, (Obj)0);
	}
	QuatT & normalize ()
	{
		double l = sqrt(Sq(_val[X]) + Sq(_val[Y]) + Sq(_val[Z]) + Sq(_val[W]));
		_val[X] /= l;
		_val[Y] /= l;
		_val[Z] /= l;
		_val[W] /= l;
		return *this;
	}
	//operator Matrix3X3T<Obj> () const;		// conversion to matrix
	operator Matrix4X4T<Obj> () const;		// conversion to matrix
	void get(Matrix4X4T<Obj> & m) const;
	void glMatrix(float * pMatrix) const;	// conversion to glMatrix
	// operators
	QuatT<Obj> operator - () const
	{
		return QuatT<Obj>(-_val[X], -_val[Y], -_val[Z], -_val[W]);
	}
	/*
	QuatT<Obj> & operator *= (double r)
	{
		_val[X] *= (Obj)r;
		_val[Y] *= (Obj)r;
		_val[Z] *= (Obj)r;
		_val[W] *= (Obj)r;
		return *this;
	}
	QuatT & operator /= (double b) { *this *= 1.0 / b; return *this; }
	*/
	template <class Obj2> QuatT & operator += (QuatT<Obj2> const & c)
	{
		_val[X] += (Obj)c[X]; _val[Y] += (Obj)c[Y]; _val[Z] += (Obj)c[Z]; _val[W] += (Obj)c[W];
		return *this;
	}
	template <class Obj2> QuatT & operator -= (QuatT<Obj2> const & c)
	{
		_val[X] -= (Obj)c[X]; _val[Y] -= (Obj)c[Y]; _val[Z] -= (Obj)c[Z]; _val[W] -= (Obj)c[W];
		return *this;
	}
	template <class Obj2> QuatT & operator *= (QuatT<Obj2> const & q)	// quat product
	{
		/*
		// [FIXME] there are two representations for multiplication... which is faster??
		QuatT r;
		r._val[W] = _val[W] * q._val[W] - v() * q.v();
		r.v() = q.v() * _val[W] + v() * q._val[W] + v() % q.v();
		return *this = r;
		*/
		QuatT r;
		r._val[W] = _val[W]*q._val[W] - _val[X]*q._val[X] - _val[Y]*q._val[Y] - _val[Z]*q._val[Z];
		r._val[X] = _val[W]*q._val[X] + _val[X]*q._val[W] + _val[Y]*q._val[Z] - _val[Z]*q._val[Y];
		r._val[Y] = _val[W]*q._val[Y] - _val[X]*q._val[Z] + _val[Y]*q._val[W] + _val[Z]*q._val[X];
		r._val[Z] = _val[W]*q._val[Z] + _val[X]*q._val[Y] - _val[Y]*q._val[X] + _val[Z]*q._val[W];
		return *this = r;
	}
	template <class Obj2> QuatT & operator /= (QuatT<Obj2> const & p)	// quat quotient
	{
		return operator *= (p.inverse());
	}
	/* [FIXME]
	VectorT<Obj> vectorRepresentation() const
	{
	}
	*/
	VectorT<Obj> xHat() const;
	VectorT<Obj> yHat() const;
	VectorT<Obj> zHat() const;
	QuatT shortWay(VectorT<Obj> const & dir) const;
	operator Obj*() { return _val; }
	operator const Obj*() const { return _val; }
	static QuatT Identity() { return QuatT(0,0,0,1); }
};

typedef QuatT<double> Quat;
typedef QuatT<float> Quatf;


//// non-member functions ////

// non-member operators

template <class Obj, class Obj2> QuatT<Obj> operator + (QuatT<Obj> a, QuatT<Obj2> const & b) { return a += b; }// sum
template <class Obj, class Obj2> QuatT<Obj> operator - (QuatT<Obj> a, QuatT<Obj2> const & b) { return a -= b; }// difference
template <class Obj, class Obj2> QuatT<Obj> operator * (QuatT<Obj> a, QuatT<Obj2> const & b) { return a *= b; }// quat product
template <class Obj, class Obj2> QuatT<Obj> operator / (QuatT<Obj> a, QuatT<Obj2> const & b) { return a /= b; }// quat quotient
//template <class Obj> QuatT<Obj> operator * (QuatT<Obj> a, double b) { return a *= b; }

template <class Obj, class Obj2> PointT<Obj2> operator * (QuatT<Obj> const & q, PointT<Obj2> const & p)
{ return (q * QuatT<Obj>(p[0], p[1], p[2], 0) * ~q).v(); }	// point rotation; assumes unit quaternion

template <class Obj, class Obj2> VectorT<Obj> operator * (QuatT<Obj> const & q, VectorT<Obj2> const & p)
{ return (q * QuatT<Obj>(p[0], p[1], p[2], 0) * ~q).v(); }	// vector rotation; assumes unit quaternion

template <class Obj, class Obj2> int operator == (QuatT<Obj> const & a, QuatT<Obj2> const & b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}
template <class Obj, class Obj2> int operator != (QuatT<Obj> const & a, QuatT<Obj2> const & b) { return !(a == b); }


//// QuatT implementation ////

template <class Obj> VectorT<Obj> QuatT<Obj>::xHat() const
{
	return (*this * QuatT<Obj>(1, 0, 0, 0) * ~*this).v();
}

template <class Obj> VectorT<Obj> QuatT<Obj>::yHat() const
{
	return (*this * QuatT<Obj>(0, 1, 0, 0) * ~*this).v();
}

template <class Obj> VectorT<Obj> QuatT<Obj>::zHat() const
{
	return (*this * QuatT<Obj>(0, 0, 1, 0) * ~*this).v();
}

template <class Obj> void QuatT<Obj>::glMatrix(float * pMatrix) const	// [REF] from NEHE
{
	// Make sure the matrix has allocated memory to store the rotation data
	if(!pMatrix) return;
	// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( _val[Y] * _val[Y] + _val[Z] * _val[Z] );
	pMatrix[ 1] = 2.0f * (_val[X] * _val[Y] + _val[Z] * _val[W]);
	pMatrix[ 2] = 2.0f * (_val[X] * _val[Z] - _val[Y] * _val[W]);
	pMatrix[ 3] = 0.0f;
	// Second row
	pMatrix[ 4] = 2.0f * ( _val[X] * _val[Y] - _val[Z] * _val[W] );
	pMatrix[ 5] = 1.0f - 2.0f * ( _val[X] * _val[X] + _val[Z] * _val[Z] );
	pMatrix[ 6] = 2.0f * (_val[Z] * _val[Y] + _val[X] * _val[W] );
	pMatrix[ 7] = 0.0f;
	// Third row
	pMatrix[ 8] = 2.0f * ( _val[X] * _val[Z] + _val[Y] * _val[W] );
	pMatrix[ 9] = 2.0f * ( _val[Y] * _val[Z] - _val[X] * _val[W] );
	pMatrix[10] = 1.0f - 2.0f * ( _val[X] * _val[X] + _val[Y] * _val[Y] );
	pMatrix[11] = 0.0f;
	// Fourth row
	pMatrix[12] = 0.0f;
	pMatrix[13] = 0.0f;
	pMatrix[14] = 0.0f;
	pMatrix[15] = 1.0f;
	// Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}

template <class Obj> QuatT<Obj>::operator Matrix4X4T<Obj> () const
//template <class Obj> void QuatT<Obj>::get(Matrix4X4T<Obj> & m) const
{
	Matrix4X4T<Obj> m;
	Obj * pMatrix = m._mat;	// [FIXME] this is utterly borken :(
	// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( _val[Y] * _val[Y] + _val[Z] * _val[Z] );
	pMatrix[ 4] = 2.0f * (_val[X] * _val[Y] + _val[Z] * _val[W]);
	pMatrix[ 8] = 2.0f * (_val[X] * _val[Z] - _val[Y] * _val[W]);
	pMatrix[12] = 0.0f;
	// Second row
	pMatrix[ 1] = 2.0f * ( _val[X] * _val[Y] - _val[Z] * _val[W] );
	pMatrix[ 5] = 1.0f - 2.0f * ( _val[X] * _val[X] + _val[Z] * _val[Z] );
	pMatrix[ 9] = 2.0f * (_val[Z] * _val[Y] + _val[X] * _val[W] );
	pMatrix[13] = 0.0f;
	// Third row
	pMatrix[ 2] = 2.0f * ( _val[X] * _val[Z] + _val[Y] * _val[W] );
	pMatrix[ 6] = 2.0f * ( _val[Y] * _val[Z] - _val[X] * _val[W] );
	pMatrix[10] = 1.0f - 2.0f * ( _val[X] * _val[X] + _val[Y] * _val[Y] );
	pMatrix[14] = 0.0f;
	// Fourth row
	pMatrix[ 3] = 0.0f;
	pMatrix[ 7] = 0.0f;
	pMatrix[11] = 0.0f;
	pMatrix[15] = 1.0f;
	// Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
	return m;
}

// Construct a unit quaternion from 3x3 rotation matrix.
// Ken Shoemake, September 1993
// [NOTE] Modified to use RHR in LHS. // [FIXME] FIX IT BACK!!
template <class Obj> QuatT<Obj>::QuatT(Matrix3X3T<Obj> const & mat)
{
	// This algorithm avoids near-zero divides by looking for a large component
	// - first w, then x, y, or z.	When the trace is greater than zero,
	// |w| is greater than 1/2, which is as small as a largest component can be.
	// Otherwise, the largest diagonal entry corresponds to the largest of |x|,
	// |y|, or |z|, one of which must be larger than |w|, and at least 1/2.
	register double tr, s;

	tr = mat[X3X] + mat[Y3Y] + mat[Z3Z];
	if (tr >= 0.0)
	{
		s = sqrt(tr + 1.0);
		_val[W] = s * 0.5;
		s = 0.5 / s;
		_val[X] = (mat[Y3Z] - mat[Z3Y]) * s;
		_val[Y] = (mat[Z3X] - mat[X3Z]) * s;
		_val[Z] = (mat[X3Y] - mat[Y3X]) * s;
	}
	else
	{
		int h = X, h3h = X3X;
		if (mat[Y3Y] > mat[X3X]) { h = Y; h3h = Y3Y; }
		if (mat[Z3Z] > mat[h3h]) h = Z;
		switch (h)
		{
#define caseMacro(I,J,K) \
			case I:\
			s = sqrt( mat[I##3##I] - mat[J##3##J] - mat[K##3##K] + 1.0 );\
			_val[I] = s*0.5;\
			s = 0.5 / s;\
			_val[J] = (mat[I##3##J] + mat[J##3##I]) * s;\
			_val[K] = (mat[K##3##I] + mat[I##3##K]) * s;\
			_val[W] = (mat[J##3##K] - mat[K##3##J]) * s;\
			break
			caseMacro(X,Y,Z);
			caseMacro(Y,Z,X);
			caseMacro(Z,X,Y);
#undef caseMacro
		}
	}
}

template <class Obj> QuatT<Obj> QuatT<Obj>::shortWay(VectorT<Obj> const & dir) const
{
	Vector axis = zHat() % dir;
	return Quat(~axis, HLTrig::aSin(!axis)) * *this;
}


// Construct rotation matrix from quaternion (unit or not)
// Ken Shoemake, September 1991 
/*
template <class Obj> QuatT<Obj>::operator Matrix3X3T<Obj> () const
{
	// [FIXME] rip from NEHe
	// [FIXME] is this RHR or what?
	Matrix3X3T<Obj> m;
	double s = !(*this);
	s = (s > 0.0) ? 2.0 / s : 0.0;
	double xs = _val[X]*s;	double ys = _val[Y]*s;	double zs = _val[Z]*s;
	double xx = _val[X]*xs;	double yy = _val[Y]*ys;	double zz = _val[Z]*zs;
	double xy = _val[X]*ys;	double xz = _val[X]*zs;	double yz = _val[Y]*zs;
	double wx = _val[W]*xs;	double wy = _val[W]*ys;	double wz = _val[W]*zs;
	m[X3X] = 1.0 - yy - zz;	m[X3Y] = xy + wz;		m[X3Z] = xz - wy;
	m[Y3X] = xy - wz;		m[Y3Y] = 1.0 - xx - zz;	m[Y3Z] = yz + wx;
	m[Z3X] = xz + wy;		m[Z3Y] = yz - wx;		m[Z3Z] = 1.0 - xx - yy;
	return m;
}
*/

// Based on code from MagicSoftware.  Modified a little bit to work with the templates.
template <class Obj> QuatT<Obj> slerp(double alpha, QuatT<Obj> const & p, QuatT<Obj> const & q, int spin = 0)
{
	// alpha = interpolation parameter (0 to 1) 
	// p, q = start and end unit quaternions 
	// spin = number of extra spin rotations 

    double dAngle = HLTrig::aCos(p.v()*q.v() + p[W]*q[W]);
    double dSin = sin(dAngle);  // fSin >= 0 since fCos >= 0

    if (dSin < EPSILON) return p;
    else if (spin)
    {
        double dPhase = Pi*spin*alpha;
        double dInvSin = 1.0/dSin;
        double dCoeff0 = sin((1.0-alpha)*dAngle - dPhase)*dInvSin;
        double dCoeff1 = sin(alpha*dAngle + dPhase)*dInvSin;
        return p*dCoeff0 + q*dCoeff1;
    }
	else
	{
        double dInvSin = 1.0/dSin;
        double dCoeff0 = sin((1.0-alpha)*dAngle)*dInvSin;
        double dCoeff1 = sin(alpha*dAngle)*dInvSin;
        return p*dCoeff0 + q*dCoeff1;
	}
}


// [FIXME] here's a lookat function using Quats:

/*
sfquat LookAt(sfvec3f target,sfvec3f current, sfvec3f eye,sfvec3f up) { 
	float angle; 
	// turn vectors into unit vectors 
	n1 = (current - eye).norm();
	n2 = (target - eye).norm();  
	d = sfvec3f.dot(n1,n2); 
	// if no noticable rotation is available return zero rotation
	// this way we avoid Cross product artifacts 
	if( d > 0.9998 ) return new sfquat( 0, 0, 1, 0 ); 
	// in this case there are 2 lines on the same axis 
	if(Math.abs(angle)-Math.pi) < 0.001){ 
		n1 = n1.Rotx( 0.5f ); 
		// there are an infinite number of normals 
		// in this case. Anyone of these normals will be 
		// a valid rotation (180 degrees). so I rotate the curr axis by 0.5 radians this way we get one of these normals 
	} 
	sfvec3f axis = n1;
	axis.cross(n2);
	sfquat pointToTarget= new sfquat(1.0 + d,axis.x,axis.y,axis.z); 
    pointToTarget.norm();
	// now twist around the target vector, so that the 'up' vector points along the z axis
    sfmatrix projectionMatrix=new sfmatrix();
    double a = pointToTarget.x;
    double b = pointToTarget.y;
    double c = pointToTarget.z;
    projectionMatrix.m00 = b*b+c*c;
    projectionMatrix.m01 = -a*b;
    projectionMatrix.m02 = -a*c;
    projectionMatrix.m10 = -b*a;
    projectionMatrix.m11 = a*a+c*c;
    projectionMatrix.m12 = -b*c;
    projectionMatrix.m20 = -c*a;
    projectionMatrix.m21 = -c*b;
    projectionMatrix.m22 = a*a+b*b;
    sfvec3f upProjected = projectionMatrix.transform(up);
    sfvec3f yaxisProjected = projectionMatrix.transform(new sfvec(0,1,0);
    d = sfvec3f.dot(upProjected,yaxisProjected);
    // so the axis of twist is n2 and the angle is arcos(d)
    //convert this to quat as follows   
	double s=Math.sqrt(1.0 - d*d);
	sfquat twist=new sfquat(d,n2*s,n2*s,n2*s);
	return sfquat.mul(pointToTarget,twist);
} 
 

*/

#undef EPSILON

#endif
