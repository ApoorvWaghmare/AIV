// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_POINT2D_H_
#define _COMMONUTIL_POINT2D_H_

//standard include files
#include<cmath>
#include<vector>

//geom include files
#include "CUPoint3D.h"

namespace CommonUtil
{

// forward declaration
class Report;
class Transform2D;

// It repesents a point on XY plane
class Point2D
{
	//========================== private data members =========================

	// x and y coordinates
	double m_x, m_y;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Point2D()
			: m_x(0.0), m_y(0.0)
	{}

    Point2D(double x, double y)
        : m_x(x), m_y(y)
    {}

	//constructor using Point3D
	Point2D(Point3D point)
			: m_x(point.GetX()),
			  m_y(point.GetY())
	{}

	//*************************** get/set methods *****************************

	//set the coordinates
	void Set(double x, double y);

	//set x co ordinate
	void SetX(double x)
	{
		m_x = x;
	}

	//set y coordinate
	void SetY(double y)
	{
		m_y = y;
	}

	//returns x coordinate
	double GetX()const
	{
		return m_x;
	}

	//returns y coordinate
	double GetY()const 
	{
		return m_y; 
	}


	//**************************general methods********************************

	//get Point3D
	operator Point3D()const;

	//checks if the points are equal in given tolerance
	bool IsEqual(const Point2D &point, 
				 double tolerance = CommonConstants::PNT_TOL)const;

	//returns distance from another point
	double Distance(const Point2D &point)const
	{
		return sqrt((m_x-point.m_x) * (m_x-point.m_x) +
					(m_y-point.m_y) * (m_y-point.m_y));
	}

	//returns sqaure of distance from another point
	double SqrDistance(const Point2D &point)const
	{
		return ((m_x-point.m_x) * (m_x-point.m_x) +
				(m_y-point.m_y) * (m_y-point.m_y));
	}

	//transforms the point
	void Transform(const Transform2D &transform);

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	// method for Sketcher... SDP
	bool IsSame(const Point2D &point)const;

};

}
#endif