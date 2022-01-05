// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_BOUNDINGBOX2D_H_
#define _COMMONUTIL_BOUNDINGBOX2D_H_

//math include files
#include "CUPoint2D.h"
#include "CUCommonConstants.h"

namespace CommonUtil
{

//forward declarations
class Report;

// This class represents a bounding box in 2D
class BoundingBox2D
{
	//========================== private data members =========================
	
	double m_minX, m_minY;		//minimum coordinate values along X, Y and Z
	double m_maxX, m_maxY;		//maximum coordinate values along X, Y and Z

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//default constructor
	BoundingBox2D();
	 
	//construct from array of points
	BoundingBox2D(const std::vector<const Point2D *> &points, double expansionTol = 0);

	//constructor with minimum and maximum values
	BoundingBox2D(double minX, double maxX, double minY, double maxY,
				  double expansionTol = 0);
	
	BoundingBox2D(const std::vector<Point2D> &points, double expansionTol = 0);

	//****************************** destructor *******************************

	~BoundingBox2D()
	{}
	
	//*************************** get/set methods *****************************

	//sets all ranges along X, Y and Z axis
	void SetAllRanges(double minX, double maxX, double minY, double maxY,
					  double expansionTol = 0);
	
	//returns all ranges along X, Y and Z axis
	void GetAllRanges(double& minX, double& maxX, 
					  double& minY, double& maxY)const;

	void GetPoints(const std::vector<Point2D> &points, Point2D &minXPoint, 
				   Point2D &minYPoint, Point2D &maxXPoint, 
				   Point2D &maxYPoint)const;

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

		//returns minimum X
	double GetMinX()const
	{
		return m_minX;
	}

	//returns maximum X
	double GetMaxX()const
	{
		return m_maxX;
	}

	//returns minimum Y
	double GetMinY()const
	{
		return m_minY;
	}

	//returns maximumY
	double GetMaxY()const
	{
		return m_maxY;
	}

	//returns bounding box center
	Point2D GetCenter()const;

	//**************************general methods********************************

	//merge given Bounding box
	void Unite(const BoundingBox2D &box);

	//expands bounding box by given value
	void ExpandBoundingBox(double value);

	//checks if the two bounding boxes have overlap
	bool IsHavingOverlap(const BoundingBox2D &otherBox, double expansionTol = 0,
						 bool *coincidentBoxes = 0)const;

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	//ckecks if the point is inside bounding box
	bool IsInside(const Point2D &point)const;

	//check if the boundingBox is totaly inside of the otherBox.
	bool IsInside(const BoundingBox2D &otherBox)const;

	// Checks if given bounding box intersects, 
	// touches, overlaps, inside to self or viceversa
	// If yes, returns true, otherwise returns false
	bool IsIntersecting(const BoundingBox2D &otherBox, double tol = CommonConstants::ZERO)const;

};

}
#endif