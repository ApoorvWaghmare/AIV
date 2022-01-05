// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_SEGMENT2D_H_
#define _COMMONUTIL_SEGMENT2D_H_

// math include files
#include "CUPoint2D.h"

namespace CommonUtil
{

//forward declarations
class Transform2D;
class BoundingBox2D;

//This class represents segment in XY plane
class Segment2D
{
	//========================== private data members =========================

	Point2D m_point1;
	Point2D m_point2;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//Default constructor
	Segment2D()
	{}

	Segment2D(const Point2D &point1, const Point2D &point2) :m_point1(point1), m_point2(point2)
	{}
	
	//****************************** destructor *******************************

	// Destructor
	~Segment2D()
	{}
	
	//*************************** get/set methods *****************************

	Point2D GetP1()const
	{
		return m_point1;
	}

	Point2D GetP2()const
	{
		return m_point2;
	}

	void Set(const Point2D &point1, const Point2D &point2)
	{
		m_point1 = point1;
		m_point2 = point2;
	}

	Point2D GetPoint(double uVal)const;

	// returns the length of the segment
	double GetLength()const
	{
		return m_point1.Distance(m_point2);
	}

	//returns slope of the segment
	// if the segment is vertical then the flag parallelToYAxis is made true
	double GetSlope(bool &parallelToYAxis)const;

	// returns the Y-intercept of the line passing through the segment
	// if the segment is vertical then the flag parallelToYAxis is made true
	double GetYIntercept(bool &parallelToYAxis)const;

	//Gives the offset for the segment
	Segment2D GetOffset(const Point2D &point, double offsetVal);

	//returns start point of segment
	Point2D GetStartPoint()const;

	//returns end point of segment
	Point2D GetEndPoint()const;

	//returns mid point of segment
	Point2D GetMidPoint()const;

	//gets the bounding box for segment2D
	BoundingBox2D GetBoundingBox(double expansionTol)const;

	// returns a point on the segment at the given uVal
	Point2D GetPointOnCurve(double uVal)const;

	//**************************general methods********************************

	//tessellates the segment
	void Tessellate(const std::vector<Point2D> &points, double tessellationTol)const;

	//transforms the segment
	void Transform(const Transform2D &transform);

	// checks if the given point has a projection on the segment
	// if the projection is nearer to one of the ends of segment, then
	// it is snapped as per the snapping tolerance
	// uVal is a parametric value of projected point on the segment
	// uVal is returned even if it is found < 0 or > 1.
	bool Project(const Point2D &projPoint, double snappingTol, double &uVal)const;

	// puts the given point in the equation of the line and returns the value of the 
	// expression
	double CheckSign(const Point2D &point)const;

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	bool IsIntersecting(const Segment2D &segment, const double tol = CommonConstants::ZERO)const;

	bool IsIntersectingNew(const Segment2D &testSegment, bool &isExactIntersection,
		bool &isBoundaryIntersection, bool &isParallel, const double tol = CommonConstants::PNT_TOL)const;

	bool IsCollinearAndOverlapping(const Segment2D &segment, Point2D intersectionPoints[2])const;

	bool IsCollinear(const Point2D &point)const;

	bool IsCollinear(const Segment2D &segment)const;

	//return false if parallel lines otherwise true
	bool GetIntersection(const Segment2D& otherSegment, Point2D &intersectionPoint)const;
};

}

#endif