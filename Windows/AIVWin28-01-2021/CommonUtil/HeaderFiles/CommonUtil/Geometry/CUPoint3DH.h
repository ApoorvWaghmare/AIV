// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_POINT3DH_H_
#define _COMMONUTIL_POINT3DH_H_

// util include files
#include "CUPoint3D.h"
#include "CUSPoint3DH.h"

namespace CommonUtil
{

// This class represents the Point3D in homogeneous system
class Point3DH
{
	//========================== private data members =========================

	double m_x;			// realX
	double m_y;			// realY
	double m_z;			// realZ
    double m_weight;	// weight

public:

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Point3DH()
		: m_x(0.0), m_y(0.0), m_z(0.0), m_weight(1.0)
	{}

	Point3DH(double x, double y, double z, double weight)
        : m_x(x), m_y(y), m_z(z), m_weight(weight)
    {}

	Point3DH(const Point3D &point, double w = 1.0)
								: m_x(point.GetX()), 
								  m_y(point.GetY()), 
								  m_z(point.GetZ()), 
								  m_weight(w)
	{}

	Point3DH(const SPoint3DH &point )
								: m_x(point.x), 
								  m_y(point.y), 
								  m_z(point.z), 
								  m_weight(point.weight)
	{}


	// Copy constructor
	Point3DH(const Point3DH &pt) 
							: m_x(pt.m_x),
							  m_y(pt.m_y),
							  m_z(pt.m_z),
							  m_weight(pt.m_weight)
	{}

	//*************************** get/set methods *****************************

	void SetX(double x)
	{
		m_x = x;
	}
	void SetY(double y)
	{
		m_y = y;
	}
	void SetZ(double z)
	{
		m_z = z;
	}
	void SetWeight(double w)
	{
		m_weight = w;
	}

	// This method sets co-ordinates of the point3dH
	void Set(double x, double y, double z, double w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_weight = w;
	}

	// This method sets co-ordinates of the point3dH
	void Set(const Point3D &point, double w)
	{
		m_x = point.GetX();
		m_y = point.GetY();
		m_z = point.GetZ();
		m_weight = w;
	}

	void SetH(double xw, double yw, double zw, double w)
	{
		m_x = xw/w;
		m_y = yw/w;
		m_z = zw/w;
		m_weight = w;
	}

	double GetX()const
	{
		return m_x;
	}

	double GetY()const
	{
		return m_y;
	}

	double GetZ()const
		{
		return m_z;
		}
	double GetXW()const
	{
		return m_x*m_weight;
	}

	double GetYW()const
		{
		return m_y*m_weight;
	}

	double GetZW()const
	{
		return m_z*m_weight;
	}

	// Returns the weight
	double GetWeight()const
		{
		return m_weight;
	}

	// Returns the real point
	Point3D GetRealPoint()const
	{
		return Point3D(m_x, m_y, m_z);
	}

	// Returns the real point
	void GetRealPoint(Point3D& point)const
	{
		point.Set(m_x, m_y, m_z);
	}

	Point3D GetHPoint()const
	{
		return Point3D(m_x*m_weight, m_y*m_weight, m_z*m_weight);
	}

	//**************************general methods********************************

	Point3DH& operator=( const Point3DH& pt )
    {
        if( this != &pt )
        {
  		    m_x = pt.m_x;
		    m_y = pt.m_y;
		    m_z = pt.m_z;
		    m_weight = pt.m_weight;
        }

        return *this;
    }

    operator SPoint3DH() const
    {
        SPoint3DH newPoint;
        newPoint.x = m_x;
        newPoint.y = m_y;
        newPoint.z = m_z;
        newPoint.weight = m_weight;

        return newPoint;
    }

};

}

#endif