// ============================================================================
//			Copyright (c) 2007 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_VECTOR_H_
#define _COMMONUTIL_VECTOR_H_

//standard include files
#include <assert.h>

//math include files
#include "CUPoint3D.h"
#include "CUPoint3DF.h"

#include "CUPSAssertions.h"

namespace CommonUtil
{

//This class represents mathematical vector in 3D space
class Vector
{
	//========================== private data members =========================

	//vector components
	double m_i, m_j, m_k;

public:

	//======================== public member functions ========================

	//to indicate if the two vectors are in the same direction or opposite direction
	enum Direction
	{
		DIFFERENT_DIRECTION,
		SAME_DIRECTION,
		OPPOSITE_DIRECTION
	};

	//************************** list of constructors *************************

	//defualt constructor
	Vector()
		: m_i(0.0), m_j(0.0), m_k(0.0)
	{}

	Vector(double i, double j, double k)
	{
		m_i = i;
		m_j = j;
		m_k = k;
	}

	//constructor with two points
	Vector (const Point3D &p1, const Point3D &p2)
	{
		Set(p1, p2);
	}

	Vector(const Point3DF& p1, const Point3DF& p2)
	{
		CommonUtil::Point3D tempP1(p1.GetX(), p1.GetY(), p1.GetZ());
		CommonUtil::Point3D tempP2(p2.GetX(), p2.GetY(), p2.GetZ());
		Set(tempP1, tempP2);
	}

	//*************************** get/set methods *****************************

	//creation method using two points
	void Set(const Point3D &p1, const Point3D &p2)
	{
		m_i = p2.GetX() - p1.GetX();
		m_j = p2.GetY() - p1.GetY();
		m_k = p2.GetZ() - p1.GetZ();
	}

	//sets i component
	void SetI(double i)
	{
		m_i = i;
	}

	//sets j component
	void SetJ(double j)
	{
		m_j = j;
	}

	//sets k component
	void SetK(double k)
	{
		m_k = k;
	}

	void Set(double i, double j, double k)
	{
		m_i = i;
		m_j = j;
		m_k = k;
	}

	//returns i component
	double GetI()const
	{
		return m_i;
	}

	//returns j component
	double GetJ()const
	{
		return m_j;
	}

	//returns k component
	double GetK()const
	{
		return m_k;
	}

	//returns magnitude of the vector
	double GetMagnitude()const
	{
		return sqrt(m_i*m_i + m_j*m_j + m_k*m_k);
	}

	//returns sqaure of the magnitude of the vector
	double GetSqrMagnitude()const
	{
		return m_i*m_i + m_j*m_j + m_k*m_k;
	}

	//get unit vector in the direction of this vector
	Vector GetUnitVector()const
	{
#ifdef _DEBUG
		PS_VERIFY(IsValid());
#endif // _DEBUG

		Vector v = *this;
		v.Normalize();
		return v;
	}
	
	//**************************general methods********************************

	//normalizes the vector
	bool Normalize(double tol = CommonConstants::PNT_TOL);

	//add a vector to the current vector and return the result
	Vector Add(const Vector &vec)const
	{
		Vector result;
		result.m_i = m_i + vec.m_i;
		result.m_j = m_j + vec.m_j;
		result.m_k = m_k + vec.m_k;
		return result;
	}

	//subtract a vector from the current vector and return the result
	Vector Subtract(const Vector &vec)const
	{
		Vector result;
		result.m_i = m_i - vec.m_i;
		result.m_j = m_j - vec.m_j;
		result.m_k = m_k - vec.m_k;
		return result;
	}

	//tests the vector for parallelism within the given angular tolerance
	bool IsParallel(const Vector &v1, Direction &dir, double angTol)const;

	//tests the vector for perpendicularity within the given angular tolerance
	bool IsPependicular(const Vector &inputVector, double angTol)const;

	//computes the cross product
	void CrossProduct (const Vector &v, Vector &product)const 
	{
#ifdef _DEBUG
		PS_VERIFY(IsValid(0.0));
		PS_VERIFY(v.IsValid(0.0));
#endif // _DEBUG

		product.m_i = ( m_j * v.m_k - v.m_j * m_k );
		product.m_j = -( m_i * v.m_k - v.m_i * m_k );
		product.m_k = ( m_i * v.m_j - v.m_i * m_j );
	}

	//returns the angle between the two vectors
	bool GetAngle(const Vector &v1, double &angle)const;

	double GetAngle(const Vector &inputVector)const;

	// Adds two vectors
	const Vector & operator +=(const Vector &v)
	{
		m_i += v.m_i;
		m_j += v.m_j;
		m_k += v.m_k;

		return *this;
	}

	//adds two vectors
	void AddTo(const Vector &v)
	{
		m_i += v.m_i;
		m_j += v.m_j;
		m_k += v.m_k;
	}

	//SDP 18Aug2007
	const Vector & operator -=(const Vector &v)
	{
		m_i -= v.m_i;
		m_j -= v.m_j;
		m_k -= v.m_k;

		return *this;
	}

	//returns the dot product
	double DotProduct (const Vector &v)const
	{
		return m_i*v.m_i + m_j*v.m_j + m_k*v.m_k;
	}

	//reverse the diraction of the vector
	void Reverse()
	{
		m_i = -m_i;
		m_j = -m_j;
		m_k = -m_k;
	}

	// multiplies its elements with the given value
	void ScalarMultiply(double val)
	{
		m_i = val * m_i;
		m_j = val * m_j;
		m_k = val * m_k;
	}

	// computes a point in its direction starting from the given base Point
	// and at a given distance 
	void GetPointAlongVector(const Point3D &basePoint, Point3D &point,
							 double distance)const;

	void GetPointAlongVector(const Point3DF &basePoint, Point3DF &point,
		double distance)const;

	// this function returns the Vector with which the given vector
	// would give the given cross product
	Vector GetOtherVectorFromCrossProd(const Vector &crossProduct, 
				double angle, bool &vectorObtained)const;

	//dump the data
	void Dump(Report &report, int tabIndent)const;
	
	// Checks the validity of the vector
	bool IsValid(double tolSquare = CommonConstants::PNT_TOL*CommonConstants::PNT_TOL)const
	{
		bool stat = false;
		
		double sqrMag = GetSqrMagnitude();
		if( sqrMag > tolSquare )
			stat = true;

		return stat;
	}

	void GetPointAlongVector(const Point2D &basePoint, Point2D &point,
							 double distance)const;

	bool IsPointAlongVector(const Point2D &basePoint, const Point2D &point)const;

	double GetCosineAngle(const Vector &v1)const;

	void Transform(const Transform3D &transform);
};

}
#endif