// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _TRIANGLE3D_H_
#define _TRIANGLE3D_H_

#include <float.h>

// Include Files
#include "CUPoint3D.h"
#include "CUPoint2D.h"
#include "CUSegment3D.h"
#include "CUPoint3DF.h"

namespace CommonUtil
{
class Plane;

class Triangle3D
{
	Point3D m_point[3];

public:

	Triangle3D()
	{}

	Triangle3D(Point3D point1,Point3D point2,Point3D point3)
	{
		m_point[0] = point1;
		m_point[1] = point2;
		m_point[2] = point3;
	}

	Triangle3D(Point3DF& point1, Point3DF& point2, Point3DF& point3)
	{
		m_point[0].Set(point1.GetX(), point1.GetY(), point1.GetZ());
		m_point[1].Set(point2.GetX(), point2.GetY(), point2.GetZ());
		m_point[2].Set(point3.GetX(), point3.GetY(), point3.GetZ());
	}

	~Triangle3D()
	{}

	enum IntersectionType
	{
		INTERSECTING,
		POINT_IN_TRIANGLE,
		SIDE_IN_TRIANGLE,
		COMMON_POINT,
		COMMON_SIDE,
		POINT_TOUCHING_SIDE,
		SIDE_TOUCHING_SIDE,
		COPLANAR,
		OVERLAPPING,
		EXACT,
		NONE
	};

	void Set(Point3D point1,Point3D point2,Point3D point3)
	{
		m_point[0] = point1;
		m_point[1] = point2;
		m_point[2] = point3;
	}

	void GetOtherPoints(const Point3D& Point, Point3D otherPoints[2])const;

	double GetAngleAtPoint(const Point3D& Point)const;

	void GetMaxAnglePoint(Point3D &tmpPoint)const;

	double GetMinAngle()const;

	double GetAspectRatio()const;

	double ComputeMinHeight()const;

	bool IsValid()const;

	bool ProjectPointAndCheckSign(const Point3D* point, const Segment3D* segmentForCheckSign,
								  bool *onBoundary = nullptr,
								  double projTol = DBL_MAX)const;

	bool ProjectPointAndCheckSign(const Point3DF* point, const Segment3D* segmentForCheckSign,
								bool *onBoundary = nullptr,
								double projTol = DBL_MAX)const;
	Point2D Get2DPoint(const Plane* plane, const Point3D* point)const;

	Point3D GetCentroid()const;

	double GetAngleOfPointAtSegment(const Point3D* point, const Segment3D* segment)const;

	double GetAngleOfPointAtSegment(const Point3DF* point, const Segment3D* segment)const;

	Point3D GetThirdPoint(const Point3D *p1, const Point3D *p2)const;

	bool IsInside(const Point3D& p1);

	Plane CreatePlaneFromPoints()const;

	void GetPoints(Point3D points[3])const;

	bool IsCoplanar(const Triangle3D& triangle)const;

	bool IsCoplanarAndOverlapping(const Triangle3D& triangle, bool &coplanar)const;

	bool IsIntersecting(const Triangle3D& triangle, Point3D intersectionPoints[2],
						IntersectionType& intersectionType)const;
};

}
#endif
