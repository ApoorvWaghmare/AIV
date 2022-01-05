// ============================================================================
//			Copyright (c) 2007 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_POINT3DF_H_
#define _COMMONUTIL_POINT3DF_H_

//standard include files
#include <cmath>
#include <vector>

//math include files
#include "CUCommonConstants.h"

namespace CommonUtil
{
class Point3D;
class SqMatrix;
class Transform3D;
class Report;

//forward declarations
class Point2D;
//It represents a point in 3 dimensions
class Point3DF
{
//protected :
	//========================== private data members =========================

	// Coordinates of point
	float m_x, m_y, m_z;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Point3DF()
       : m_x(0.0), m_y(0.0), m_z(0.0)
	{}

    Point3DF(float x, float y, float z)
        : m_x(x), m_y(y), m_z(z)
    {}

	// Copy constructor
	Point3DF(const Point3DF&pt) : m_x(pt.m_x), m_y(pt.m_y), m_z(pt.m_z)
	{}

	//constructor using Point2D... changes for Sketcher
	Point3DF(const Point2D* point);

	//*************************** get/set methods *****************************

	// Returns x coordinate
	float GetX()const
	{
		return m_x;
	}

	// Returns y coordinate
	float GetY()const
	{
		return m_y;
	}

	// Returns z coordinate
	float GetZ()const
	{
		return m_z;
	}

	// Sets the coordinates
	void Set(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	// Sets x coordinate
	void SetX(float x)
	{
		m_x = x;
	}

	// Sets y coordinate
	void SetY(float y)
	{
		m_y = y;
	}

	// Sets z coordiante
	void SetZ(float z)
	{
		m_z = z;
	}

	//**************************general methods********************************

	//equal to operator
	Point3DF& operator=( const Point3DF& pt )
    {
        if( this != &pt )
        {
            m_x      = pt.m_x;
            m_y      = pt.m_y;
            m_z      = pt.m_z;
        }

        return *this;
    }

	//checks if the points are equal in given tolerance
	bool IsEqual(const Point3DF &point, 
				 double tolerance = CommonConstants::PNT_TOL)const;

	//computes distance from another point
	double Distance(const Point3DF &point)const
	{
		double sqrDistance = ((double)m_x-(double)point.m_x) * ((double)m_x-(double)point.m_x) +
							 ((double)m_y-(double)point.m_y) * ((double)m_y-(double)point.m_y) +
							 ((double)m_z-(double)point.m_z) * ((double)m_z-(double)point.m_z);

		return sqrt(sqrDistance);
	}

	//computes square of the distance from another point
	double SqrDistance(const Point3DF &point)const
	{
		double sqrDist = ((double)m_x-(double)point.m_x) * ((double)m_x-(double)point.m_x) +
						 ((double)m_y-(double)point.m_y) * ((double)m_y-(double)point.m_y) +
						 ((double)m_z-(double)point.m_z) * ((double)m_z-(double)point.m_z);

		return sqrDist;
	}

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	//checks whether three points are collinear
	bool IsCollinear(const Point3DF& point2, const Point3DF& point3,
					 double collinearityTol = CommonConstants::COLLINEARITY_TOL)const;

	void ApplyLengthUnit(double conversionFactor);

	//transforms point with the input transformation
	void Transform(SqMatrix &Rotation, std::vector<double> &translation );

	void Transform(const Transform3D &transform);

	void Scale(float scalingFactor)
	{
		m_x *= scalingFactor;
		m_y *= scalingFactor;
		m_z *= scalingFactor;
	}

	operator Point3D();

	void GetAvgPoint(const Point3DF* point, double &x, double &y, double &z)const;
};

}
#endif