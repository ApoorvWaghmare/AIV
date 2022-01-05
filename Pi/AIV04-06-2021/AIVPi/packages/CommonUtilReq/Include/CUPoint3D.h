// ============================================================================
//			Copyright (c) 2007 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_POINT3D_H_
#define _COMMONUTIL_POINT3D_H_

//standard include files
#include <cmath>
#include <vector>
#include <math.h>

//math include files
#include "CUCommonConstants.h"

namespace CommonUtil
{

//forward declarations
class Matrix;
class Point3DF;
class Transform3D;
class SqMatrix;
class Report;


class Point2D;
//It represents a point in 3 dimensions
class Point3D
{
//protected :
	//========================== private data members =========================

	// Coordinates of point
	double m_x, m_y, m_z;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Point3D()
       : m_x(0.0), m_y(0.0), m_z(0.0)
	{}

    Point3D(double x, double y, double z)
        : m_x(x), m_y(y), m_z(z)
    {}

	// Copy constructor
	Point3D(const Point3D&pt) : m_x(pt.m_x), m_y(pt.m_y), m_z(pt.m_z)
	{}

	//constructor using Point2D... changes for Sketcher
	Point3D(const Point2D* point);

	// constructor to save polar coordinates as cartesian
	Point3D(double radius, double theta);

	//*************************** get/set methods *****************************

	// Returns x coordinate
	double GetX()const
	{
		return m_x;
	}

	// Returns y coordinate
	double GetY()const
	{
		return m_y;
	}

	// Returns z coordinate
	double GetZ()const
	{
		return m_z;
	}

	// Sets the coordinates
	void Set(double x, double y, double z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	// Sets x coordinate
	void SetX(double x)
	{
		m_x = x;
	}

	// Sets y coordinate
	void SetY(double y)
	{
		m_y = y;
	}

	// Sets z coordiante
	void SetZ(double z)
	{
		m_z = z;
	}

	//**************************general methods********************************

	//equal to operator
	Point3D& operator=( const Point3D& pt )
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
	bool IsEqual(const Point3D &point,
				 double tolerance = CommonConstants::PNT_TOL)const;

	//computes distance from another point
	double Distance(const Point3D &point)const
	{
		return sqrt((m_x-point.m_x) * (m_x-point.m_x) +
					(m_y-point.m_y) * (m_y-point.m_y) +
					(m_z-point.m_z) * (m_z-point.m_z));
	}

	//computes square of the distance from another point
	double SqrDistance(const Point3D &point)const
	{
		return ((m_x-point.m_x) * (m_x-point.m_x) +
				(m_y-point.m_y) * (m_y-point.m_y) +
				(m_z-point.m_z) * (m_z-point.m_z));
	}

	//transforms point with the input transformation
	void Transform(const Transform3D &transform);

	//transforms point with the input transformation matrix
	void Transformation(const Matrix &transMatrix);

	void ApplyTransformation(const Matrix &matrix);

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	//transforms point with the input transformation and rotation
	void Transform(const SqMatrix &Rotation, const std::vector<double> &translation);

	//checks whether three points are collinear
	bool IsCollinear(const Point3D& point2, const Point3D& point3,
		double collinearityTol = CommonConstants::COLLINEARITY_TOL)const;

	void ApplyLengthUnit(double conversionFactor);

	operator Point3DF()const;
};

}
#endif
