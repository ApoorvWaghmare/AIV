// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_BOUNDINGBOX3D_H_
#define _COMMONUTIL_BOUNDINGBOX3D_H_

//math include files
#include "CUPoint3D.h"

namespace CommonUtil
{

// This class represents a bounding box in 3D
class BoundingBox3D
{
	//========================== private data members =========================

	double m_minX, m_minY, m_minZ;		//minimum coordinate values along X, Y and Z
	double m_maxX, m_maxY, m_maxZ;		//maximum coordinate values along X, Y and Z

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	BoundingBox3D();

	//construct from array of points
	BoundingBox3D(const std::vector<const Point3D *> &points, double expansionTol = 0);

	BoundingBox3D(const std::vector<Point3D > &points, double expansionTol = 0);

	//constructor with minimum and maximum values
	BoundingBox3D(double minX, double maxX, double minY, double maxY,
				  double minZ, double maxZ, double expansionTol = 0);

	BoundingBox3D(const std::vector<double> &points);

	BoundingBox3D(const std::vector<float> &points);

	//****************************** destructor *******************************

	// Destructor
	~BoundingBox3D()
	{}

	//*************************** get/set methods *****************************

	//sets all ranges along X, Y and Z axis
	void SetAllRanges(double minX, double maxX, double minY,
					  double maxY, double minZ, double maxZ, double expansionTol = 0);

	//returns all ranges along X, Y and Z axis
	void GetAllRanges(double& minX, double& maxX, double& minY, double& maxY,
					  double& minZ, double& maxZ)const;

	//return range along X axis
	double GetXrange()const
	{
		return m_maxX - m_minX;
	}

	//return range along Y axis
	double GetYrange()const
	{
		return m_maxY - m_minY;
	}

	//return range along Z axis
	double GetZrange()const
	{
		return m_maxZ - m_minZ;
	}

	//returns minimum X
	double GetMinX()const
	{
		return m_minX;
	}

	//return maximum X
	double GetMaxX()const
	{
		return m_maxX;
	}

	//returns minimum Y
	double GetMinY()const
	{
		return m_minY;
	}

	//return maximum Y
	double GetMaxY()const
	{
		return m_maxY;
	}

	//returns minimum Z
	double GetMinZ()const
	{
		return m_minZ;
	}

	//return maximum Z
	double GetMaxZ()const
	{
		return m_maxZ;
	}

	//Name: setExtents
	//
	//Parameter: m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ
	//			 These are the minimum and maximum co-ordinates along X, Y and Z axis
	//
	//Description: This function sets ranges along X, Y and Z axis
	void SetExtents(double minX, double minY, double minZ,
		double maxX, double maxY, double maxZ)
	{
		m_minX = minX;
		m_minY = minY;
		m_minZ = minZ;

		m_maxX = maxX;
		m_maxY = maxY;
		m_maxZ = maxZ;
	}

	//provides corner points of bounding box
	// eight points
	//						(5)*----------------------*(7)
	//						  /|  z					 /|
	//					y	 / | /					/ |
	//					^	/  |/				   /  |
	//					|  /   *------------------/-- *(6)
	//					| /	  /	(4)				 /   /
	//					|/	 /				(3)	/   /
	//				 (1)* ---------------------*   /
	//					|  /				   |  /
	//					| /					   | /
	//				 (0)*----------------------*(2)-----> x

	//points[0].Set(m_minX, m_minY, m_minZ);
	//points[1].Set(m_minX, m_maxY, m_minZ);
	//points[2].Set(m_maxX, m_minY, m_minZ);
	//points[3].Set(m_maxX, m_maxY, m_minZ);
	//points[4].Set(m_minX, m_minY, m_maxZ);
	//points[5].Set(m_minX, m_maxY, m_maxZ);
	//points[6].Set(m_maxX, m_minY, m_maxZ);
	//points[7].Set(m_maxX, m_maxY, m_maxZ);
	void GetCornerPoints(std::vector<CommonUtil::Point3D> &points)const;

	//Description: This function gives ranges along X, Y and Z axis
	void GetExtents(double &minX, double &minY, double &minZ,
		double &maxX, double &maxY, double &maxZ)const
	{
		minX = m_minX;
		minY = m_minY;
		minZ = m_minZ;

		maxX = m_maxX;
		maxY = m_maxY;
		maxZ = m_maxZ;
	}

	void GetExtents(float &minX, float &minY, float &minZ,
		float &maxX, float &maxY, float &maxZ)const
	{
		minX = (float)m_minX;
		minY = (float)m_minY;
		minZ = (float)m_minZ;
		maxX = (float)m_maxX;
		maxY = (float)m_maxY;
		maxZ = (float)m_maxZ;
	}

	//returns bounding box center
	Point3D GetCenter()const;

	//**************************general methods********************************

	//merge given Bounding box
	void Unite(const BoundingBox3D &box);

	//expands bouding box by given value
	void ExpandBoundingBox(double value);

	//checks if the three bounding boxes have overlap
	bool IsHavingOverlap(const BoundingBox3D &otherBox, double expansionTol = 0,
					     bool *coincidentBoxes = 0)const;

	//expands the bounding box by given tolerance
	//if the tolerance is negative, it contracts the bounding box
	void Expand(double expansionTolerance);

	bool IsPointInBox(const Point3D &point)const;

	bool IsIntersecting(const BoundingBox3D &box)const;

	double GetDiagonalLength()const;

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	bool IsInside(const Point3D* point)const;

	bool IsInside(const Point3DF* point)const;

	void GetCornerPoints(double cornerPoints[24])const;

	bool IsPointOnWall(const Point3D* point, double tolerance)const;

	bool IsPointOnWall(const Point3DF* point, double tolerance)const;

	//check if other box falll inside the existing box
	bool IsInside(const BoundingBox3D &otherBox)const;

	Point3DF GetCenterPoint3DF();

	BoundingBox3D* MergeBoxes(BoundingBox3D &box);
};

}
#endif
