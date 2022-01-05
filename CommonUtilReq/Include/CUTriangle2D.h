// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_TRIANGLE_H_
#define _COMMONUTIL_TRIANGLE_H_

//geom include files
#include "CUPoint2D.h"

namespace CommonUtil
{

//forward declaration
class Segment2D;
class CommonConstants;

//This class represents a triangle in XY plane
class Triangle2D
{
	//========================== private data members =========================

	Point2D m_point[3];
	
public:

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Triangle2D()
	{}

	//construct the triangle using three points
	Triangle2D(Point2D &point1, Point2D &point2, Point2D &point3)
	{
		m_point[0] = point1;
		m_point[1] = point2;
		m_point[2] = point3;
	}

	//sets the vertices of triangle
	void Set(const Point2D &point1,const Point2D &point2,const Point2D &point3)
	{
		m_point[0] = point1;
		m_point[1] = point2;
		m_point[2] = point3;
	}

	//sets the vertices of triangle from another triangle
	void Set(const Triangle2D *triangle)
	{
		m_point[0] = triangle->GetPoint(0);
		m_point[1] = triangle->GetPoint(1);
		m_point[2] = triangle->GetPoint(2);
	}

	//sets the point
	void SetPoint(int index, const Point2D &point)
	{
		m_point[index] = point;
	}

	//returns vertex of a triangle given by its index
	Point2D GetPoint(int index)const
	{
		return m_point[index];
	}	

	//returns the angle made by two edges at any vertex
	double GetAngle(int index)const;

	//returns the array of segments
	std::vector<Segment2D> GetSegments()const;

	//returns previous index
	int GetPreviousIndex(int currentIndex)const;

	//returns next index
	int GetNextIndex(int currentIndex)const;

	//returns remaining index
	int GetRemainingIndex(int minimumIndexNo, int maxIndexNum)const;

	//returns the index corresponding to min angle
	int GetMinimumAngleIndex()const;

	//returns the area of triangle
	double GetArea();

	//returns centroid of the triangle
	Point2D GetCentroid();

	//**************************general methods********************************

	//checks the triangle
	bool IsValidTriangle()const;

	//checks if a point is inside triangle
	bool CheckInsideTriangle(const Point2D &point, bool *onBoundary = 0,
		bool *onVertex = 0, size_t *parallelEdgeIndices = 0,
		double inSideTolerance = 0.0)const;

	//orients the triangle in anticlockwise sense
	void OrientTriangle();

	// The overlap is detected only if two triangles share some common area.
	// Triangles which share only point(s) but not area, won't be treated as overlapping
	bool IsOverlapping(const Triangle2D& triangle)const;

	void GetPoints(Point2D points[3])const
	{
		points[0] = m_point[0];
		points[1] = m_point[1];
		points[2] = m_point[2];
	}

	bool GetThirdPoint(const Point2D &p1, const Point2D &p2, Point2D &thirdPoint)const;

	//dumps the data
	void Dump(Report &report, int tabIndent)const;
	
};
}

#endif