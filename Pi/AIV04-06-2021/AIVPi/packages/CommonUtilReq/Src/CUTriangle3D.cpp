// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Include Files
#include "CUTriangle3D.h"
#include "CUSegment3D.h"
#include "CUVector.h"
#include "CUPlane.h"
#include "CUTriangle2D.h"
#include "CUSegment2D.h"
#include "CUSqMatrix.h"
#include "CUCommonConstants.h"
#include "CUPoint3DF.h"

namespace CommonUtil
{

void Triangle3D::GetOtherPoints(const Point3D& Point, Point3D otherPoints[2])const
{
	size_t counter = 0;
	for(size_t i = 0; i < 3; i++)
	{
		if (Point.GetX() != m_point[i].GetX() || Point.GetY() != m_point[i].GetY() || 
			Point.GetZ() != m_point[i].GetZ())
		{
			otherPoints[counter] = m_point[i];
			counter++;
		}
	}
}

//-----------------------------------------------------------------------------

double Triangle3D::GetAngleAtPoint(const Point3D& point)const
{
	Point3D otherPoints[2];

	GetOtherPoints(point, otherPoints);

	Vector vec1 (point, otherPoints[0]);
	Vector vec2 (point, otherPoints[1]);

	double angle = -1;

	vec1.GetAngle(vec2,angle);

	return angle;
}

//-----------------------------------------------------------------------------

void Triangle3D::GetMaxAnglePoint(Point3D &tmpPoint)const
{
	double angle1 = GetAngleAtPoint(m_point[0]);
	double angle2 = GetAngleAtPoint(m_point[1]);
	double angle3 = GetAngleAtPoint(m_point[2]);

	double maxAngle = angle1;
	size_t index = 0;

	if(angle2 > maxAngle)
	{
		maxAngle = angle2;
		index = 1;
	}

	if(angle3 > maxAngle)
		index = 2;

	tmpPoint = m_point[index];
}

//-----------------------------------------------------------------------------

double Triangle3D::GetMinAngle()const
{
	double angle1 = GetAngleAtPoint(m_point[0]);
	double angle2 = GetAngleAtPoint(m_point[1]);
	double angle3 = GetAngleAtPoint(m_point[2]);

	double minAngle = angle1;

	if(angle2 < minAngle)
		minAngle = angle2;

	if(angle3 < minAngle)
		minAngle = angle3;

	return minAngle;
}

//-----------------------------------------------------------------------------

double Triangle3D::GetAspectRatio()const
{
	Point3D tmpPoint;
	GetMaxAnglePoint(tmpPoint);
	
	Point3D otherPoints[2];
	GetOtherPoints(tmpPoint, otherPoints);

	Segment3D segment(otherPoints[0], otherPoints[1]);
	double edgeLength = segment.GetLength();

	double snappingTol = 0.00;
	double uVal;
	segment.Project(tmpPoint, snappingTol, uVal);

	Point3D projectedPoint;
	projectedPoint = segment.GetPoint( uVal);

	double projectedLength = projectedPoint.Distance(tmpPoint);

	double aspectRatio = 0;

	if(fabs(projectedLength - CommonConstants::ZERO) < CommonConstants::PNT_TOL)
		return aspectRatio;
	else
		aspectRatio = edgeLength/projectedLength;

	return aspectRatio;
}

//-----------------------------------------------------------------------------

double Triangle3D::ComputeMinHeight()const
{
	double projectedLength;
	double snappingTol = 0.00;

	Point3D tmpPoint;
	GetMaxAnglePoint(tmpPoint);

	Point3D otherPoints[2];
	GetOtherPoints(tmpPoint, otherPoints);

	Segment3D segment(otherPoints[0], otherPoints[1]);

	Point3D projectedPoint;
	double uVal;

	segment.Project(tmpPoint, snappingTol, uVal);
	projectedPoint = segment.GetPoint( uVal);

	projectedLength = projectedPoint.Distance(tmpPoint);

	return projectedLength;
}

//-----------------------------------------------------------------------------

bool Triangle3D::IsValid()const
{
	if(m_point[0].SqrDistance(m_point[1]) < CommonConstants::SQR_PNT_TOL || 
	   m_point[0].SqrDistance(m_point[2]) < CommonConstants::SQR_PNT_TOL ||
	   m_point[1].SqrDistance(m_point[2]) < CommonConstants::SQR_PNT_TOL)
	   return false;

	if(GetAspectRatio() - CommonConstants::ZERO < CommonConstants::PNT_TOL)
		return false;

	return true;
}

//-----------------------------------------------------------------------------

bool Triangle3D::ProjectPointAndCheckSign(const Point3D* point, const Segment3D* segmentForCheckSign, 
										  bool *onBoundary, double projTol)const
{
	Plane p;
	p.CreateFromThreePoints(&m_point[0], &m_point[1], &m_point[2]);
	Vector P1(m_point[1], m_point[0]);
	Vector P2(m_point[2], m_point[0]);
	Point3D tmpProjPoint;
	bool stat = p.ProjectPoint(*point, tmpProjPoint, projTol);
	if(stat)
	{
		double alpha1 = 0.0, alpha2 = 0.0;
		Vector P(tmpProjPoint, m_point[0]);
		double D = P1.GetI() * P2.GetJ() - P2.GetI() * P1.GetJ();
		if(fabs(D) < CommonConstants::ZERO)
			stat = false;
		if(stat)
		{
			alpha1 = P.GetI() * P2.GetJ() - P2.GetI() * P.GetJ();
			alpha1 /= D;
			if(alpha1 < 0.0 || alpha1 > 1.0)
				stat = false;
		}
		if(stat)
		{
			alpha2 = P.GetI() * P2.GetJ() - P2.GetI() * P.GetJ();
			alpha2 /= D;
			if(alpha2 < 0.0 || alpha2 > 1.0)
				stat = false;
		}
		if(stat && (alpha1 + alpha2) > 1.0)
			stat = false;

		if(onBoundary && stat && ((alpha1 + alpha2) > 1.0 - CommonConstants::PNT_TOL))
			*onBoundary = true;
/*
		if(stat)
		{
			double Z = alpha1 * P1.GetK() + alpha2 * P2.GetK();
			if(fabs(Z - P.GetK()) > PNT_TOL)
				int debug = 0;
		}
*/
	}

/*
	Plane p;
	p.CreateFromThreePoints(&m_point[0], &m_point[1], &m_point[2]);
	
	Point2D p1 = Get2DPoint(&p, &m_point[0]);
	Point2D p2 = Get2DPoint(&p, &m_point[1]);
	Point2D p3 = Get2DPoint(&p, &m_point[2]);

	Point3D tmpProjPoint;
	
	bool stat = p.ProjectPoint(point, tmpProjPoint, projTol);
	
	if (stat)
	{
		Point2D p4 = Get2DPoint(&p, &tmpProjPoint);

		Triangle2D triangle(p1, p2, p3);
		stat = triangle.CheckInsideTriangle(p4, onBoundary);

		if(stat)
			return stat;
		/ *else 
		{
			Point2D segPoint1, segPoint2;

			segPoint1 = Get2DPoint(&p, segmentForCheckSign->GetPoint1());
			segPoint2 = Get2DPoint(&p, segmentForCheckSign->GetPoint2());

			Segment2D segment(segPoint1, segPoint2);

			Point3D centroid = GetCentroid();
			Point2D p5 = Get2DPoint(&p, &centroid);

			if(segment.CheckSign(p4) * segment.CheckSign(p5) > 0 )
				stat = true;
		}* /
	}
*/

	return stat;
}

//-----------------------------------------------------------------------------
bool Triangle3D::ProjectPointAndCheckSign(const Point3DF* point, const Segment3D* segmentForCheckSign, 
										  bool *onBoundary, double projTol)const
{
	Point3D tempPoint(*(const CommonUtil::Point3D*)point);
	return ProjectPointAndCheckSign(&tempPoint, segmentForCheckSign, onBoundary, projTol);
}
//-----------------------------------------------------------------------------

Point2D Triangle3D::Get2DPoint(const Plane* plane, const Point3D* point)const
{
	double costhetaX;
	double sinthetaX;
	
	double costhetaY;
	double sinthetaY;

	std::vector<double> translation(3);
	translation[0] = 0;
	translation[1] = 0;
	translation[2] = 0;
	
	const Vector& normal = plane->GetNormal();

	double x = normal.GetI();
	double y = normal.GetJ();
	double z = normal.GetK();
	
	double thetaY,thetaX;
	
	if(fabs(x) < CommonConstants::ZERO)
		thetaY = 0;
	else
		thetaY = atan(x/z);
	
	if(fabs(y) < CommonConstants::ZERO)
		thetaX = 0;
	else
	thetaX = atan(y/z);

	costhetaX = cos(thetaX);
	sinthetaX = sin(thetaX);
	
	costhetaY = cos(thetaY);
	sinthetaY = sin(thetaY);
	
	SqMatrix RotateY(3,SqMatrix::IDENTITY);
	RotateY(0,0) = costhetaY;
	RotateY(0,2) = -sinthetaY;
	RotateY(2,0) = sinthetaY;
	RotateY(2,2) = costhetaY;

	SqMatrix RotateX(3,SqMatrix::IDENTITY);
	RotateX(1,1) = costhetaX;
	RotateX(1,2) = sinthetaX;
	RotateX(2,1) = -sinthetaX;
	RotateX(2,2) = costhetaX;

	SqMatrix Transform(3) ;
	RotateX.Multiply(RotateY,Transform);
	Point3D tempPoint = *point;
	tempPoint.Transform(Transform, translation);
	double xTemp = tempPoint.GetX();
	double yTemp = tempPoint.GetY();
	
	return CommonUtil::Point2D(xTemp,yTemp);
}

//-----------------------------------------------------------------------------

Point3D Triangle3D::GetCentroid()const
{
	Point3D point;
	float x = 0, y = 0, z = 0;
	for (size_t i = 0; i < 3; i++)
	{
		x += m_point[i].GetX();
		y += m_point[i].GetY();
		z += m_point[i].GetZ();
	}
	x /= 3.;
	y /= 3.;
	z /= 3.;
	
	point.Set(x, y, z);
	return point;
}

//-----------------------------------------------------------------------------

double Triangle3D::GetAngleOfPointAtSegment(const Point3D* point, const Segment3D* segment)const
{
	double angle = -1;
	double uVal;
	
	segment->Project(*point,0,uVal);
	Point3D segmentPoint1 = segment->GetPoint(uVal);
	Vector v1(segmentPoint1, *point);
	
	Point3D thirdPoint = GetThirdPoint(segment->GetPoint1(), segment->GetPoint2());

	segment->Project(thirdPoint, 0, uVal);
	Point3D segmentPoint2 = segment->GetPoint(uVal);
	Vector v2(segmentPoint2, thirdPoint);

	if (v1.GetSqrMagnitude() > CommonConstants::SQR_PNT_TOL && v2.GetSqrMagnitude() > CommonConstants::SQR_PNT_TOL)
	{
		v1.GetAngle(v2,angle);
	}
	return angle;
}

//-----------------------------------------------------------------------------
double Triangle3D::GetAngleOfPointAtSegment(const Point3DF* point, const Segment3D* segment)const
{
	Point3D tempPoint(*(const CommonUtil::Point3D*)point);
	return GetAngleOfPointAtSegment(&tempPoint, segment);
}

//-----------------------------------------------------------------------------
Point3D Triangle3D::GetThirdPoint(const Point3D *p1, const Point3D *p2)const
{
	//assert (p1->Distance(*p2) > PNT_TOL);

	Point3D thirdPoint;

	double tol = CommonConstants::SQR_PNT_TOL;
	if((p1->SqrDistance(m_point[0]) < tol && p2->SqrDistance(m_point[1]) < tol) ||
		(p2->SqrDistance(m_point[0]) < tol && p1->SqrDistance(m_point[1]) < tol))
		thirdPoint = m_point[2];
	else if((p1->SqrDistance(m_point[1]) < tol && p2->SqrDistance(m_point[2]) < tol) ||
		(p2->SqrDistance(m_point[1]) < tol && p1->SqrDistance(m_point[2]) < tol))
		thirdPoint = m_point[0];
	else if((p1->SqrDistance(m_point[2]) < tol && p2->SqrDistance(m_point[0]) < tol) ||
		(p2->SqrDistance(m_point[2]) < tol && p1->SqrDistance(m_point[0]) < tol))
		thirdPoint = m_point[1];

	return thirdPoint;
}

//-----------------------------------------------------------------------------

bool Triangle3D::IsInside(const Point3D& p1)
{
	// this method uses barycentric technique
	Vector v0(m_point[0], m_point[1]);
	Vector v1(m_point[0], m_point[2]);
	Vector v2(m_point[0], p1);

	// Compute dot products
	double dot00, dot01, dot02, dot11, dot12;
	dot00 = v0.GetSqrMagnitude();	// For optimization
	dot01 = v0.DotProduct(v1);
	dot02 = v0.DotProduct(v2);
	dot11 = v1.GetSqrMagnitude();	// For optimization
	dot12 = v1.DotProduct(v2);

	// Compute barycentric coordinates
	double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return ((u > 0) && (v > 0) && (u + v < 1));
}

//-----------------------------------------------------------------------------

Plane Triangle3D::CreatePlaneFromPoints()const
{
	Plane plane;
	plane.CreateFromThreePoints(&m_point[0], &m_point[1], &m_point[2]);

	return plane;
}

//-----------------------------------------------------------------------------

void Triangle3D::GetPoints(Point3D points[3])const
{
	points[0] = m_point[0];
	points[1] = m_point[1];
	points[2] = m_point[2];
}

//-----------------------------------------------------------------------------

bool Triangle3D::IsCoplanar(const Triangle3D& triangle)const
{
	Plane plane = CreatePlaneFromPoints();
	return plane.IsTriangleCoplanar(triangle);
}

//-----------------------------------------------------------------------------	

bool Triangle3D::IsCoplanarAndOverlapping(const Triangle3D& triangle, bool &coplanar)const
{
	if(!IsCoplanar(triangle))
		return false;

	// Create plane from triangle points
	Plane plane = CreatePlaneFromPoints();	// Plane

	Point3D points[3];	
	triangle.GetPoints(points);				// triangle2's Points3D

	// Compute 2D points for points of both triangles with plane as XY plane
	Point2D triPoints1[3], triPoints2[3];

	triPoints1[0] = Get2DPoint(&plane, &m_point[0]);		// triangle1's Points2D
	triPoints1[1] = Get2DPoint(&plane, &m_point[1]);
	triPoints1[2] = Get2DPoint(&plane, &m_point[2]);

	triPoints2[0] = Get2DPoint(&plane, &points[0]);			// triangle2's Points2D
	triPoints2[1] = Get2DPoint(&plane, &points[1]);
	triPoints2[2] = Get2DPoint(&plane, &points[2]);

	// Create 2D triangles and check if they overlap each other
	Triangle2D triangle1(triPoints1[0],triPoints1[1],triPoints1[2]);   // triangle1 in 2D
	Triangle2D triangle2(triPoints2[0],triPoints2[1],triPoints2[2]);   // triangle2 in 2D

	return triangle1.IsOverlapping(triangle2);	   // 2D Triangle-Triangle Overlap
}

//-----------------------------------------------------------------------------	

bool Triangle3D::IsIntersecting(const Triangle3D& triangle, Point3D intersectionPoints[2],
								IntersectionType& intersectionType)const
{
	intersectionType = NONE;

	if(!IsValid() || !triangle.IsValid())		// if any triangle is invalid, return false
		return false;

	Point3D triPoints[3];
	triangle.GetPoints(triPoints);

	std::vector< Point3D > commonPoints;

	if(m_point[0].SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL || m_point[0].SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL || m_point[0].SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
		commonPoints.push_back(m_point[0]);
  	if(m_point[1].SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL || m_point[1].SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL || m_point[1].SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
		commonPoints.push_back(m_point[1]);
	if(m_point[2].SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL || m_point[2].SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL || m_point[2].SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
		commonPoints.push_back(m_point[2]);

	if(commonPoints.size() == 3)
	{
		// EXACT OVERLAP found : The given two triangles are EXACT overlapping
		intersectionType = EXACT;
		return true;
	}

	bool coplanar = false;
	if(IsCoplanarAndOverlapping(triangle, coplanar))
	{
		intersectionType = OVERLAPPING;
		return true;
	}
	else if(coplanar)
	{
		intersectionType = COPLANAR;
		return false;				   // check for feasibility	- true/ false
	}

	if(commonPoints.size() == 2)
	{
		intersectionPoints[0] = commonPoints[0];
		intersectionPoints[1] = commonPoints[1];
		intersectionType = COMMON_SIDE;
		return true;				// If common side found : intersectionFound = true;
	}

	// 1.--------------- COMPUTE INTERSECTION INTERVALS -------------------------

	// compute intervals for triangle 1
	Point3D intersectPointsA[2];
	Plane plane2 = triangle.CreatePlaneFromPoints();
	Plane::PlaneIntersectionType plane2IntersectionType;

	if(!plane2.IsTriangleIntersecting(*this, intersectPointsA, plane2IntersectionType, commonPoints))
		return false;			// EARLY EXIT

	// compute intervals for triangle 2
	Point3D intersectPointsB[2];
	Plane plane1 = CreatePlaneFromPoints();
	Plane::PlaneIntersectionType plane1IntersectionType;

	if(!plane1.IsTriangleIntersecting(triangle, intersectPointsB, plane1IntersectionType, commonPoints))
		return false;			// EARLY EXIT

	// 2.------------ FIND THE SEGMENT OVERLAP & DECIDE INTERSECTION ----------------
	bool segment1Valid = false, segment2Valid = false, intersectionFound = false;

	if(intersectPointsA[0].SqrDistance(intersectPointsA[1]) > CommonConstants::SQR_PNT_TOL)	// for plane2
		segment1Valid = true;

	if(intersectPointsB[0].SqrDistance(intersectPointsB[1]) > CommonConstants::SQR_PNT_TOL)	// for plane1
		segment2Valid = true;

	if(segment1Valid && segment2Valid)	// two Line Segments
	{
		Segment3D segment1(intersectPointsA[0], intersectPointsA[1]);
		Segment3D segment2(intersectPointsB[0], intersectPointsB[1]);

		if(!segment1.IsCollinearAndOverlapping(segment2,intersectionPoints))
			return false;

		intersectionFound = true;
		intersectionType = INTERSECTING;

		if(plane2IntersectionType == Plane::SIDE_ON_PLANE &&
		   plane1IntersectionType == Plane::SIDE_ON_PLANE)
		{
			if(intersectionPoints[0].SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL)
				intersectionType = COMMON_POINT;
			else if((segment1.GetPoint1()->SqrDistance(intersectionPoints[0]) < CommonConstants::SQR_PNT_TOL || 
					 segment1.GetPoint1()->SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL) &&
					(segment1.GetPoint2()->SqrDistance(intersectionPoints[0]) < CommonConstants::SQR_PNT_TOL || 
					 segment1.GetPoint2()->SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL))
				intersectionType = COMMON_SIDE;
			else
				intersectionType = SIDE_TOUCHING_SIDE;
		}
		else if(plane2IntersectionType == Plane::SIDE_ON_PLANE &&
				plane1IntersectionType == Plane::INTERSECTING)
		{
			if(intersectionPoints[0].SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL)
				intersectionType = POINT_TOUCHING_SIDE;
			else if((segment1.GetPoint1()->SqrDistance(intersectionPoints[0]) < CommonConstants::SQR_PNT_TOL || 
					 segment1.GetPoint1()->SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL) &&
					(segment1.GetPoint2()->SqrDistance(intersectionPoints[0]) < CommonConstants::SQR_PNT_TOL || 
					 segment1.GetPoint2()->SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL))
				intersectionType = SIDE_IN_TRIANGLE;
		}
		else if(plane2IntersectionType == Plane::INTERSECTING &&
				plane1IntersectionType == Plane::SIDE_ON_PLANE)
		{
			if(intersectionPoints[0].SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL)
				intersectionType = POINT_TOUCHING_SIDE;
			else if((segment2.GetPoint1()->SqrDistance(intersectionPoints[0]) < CommonConstants::SQR_PNT_TOL || 
					 segment2.GetPoint1()->SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL) &&
					(segment2.GetPoint2()->SqrDistance(intersectionPoints[0]) < CommonConstants::SQR_PNT_TOL || 
					 segment2.GetPoint2()->SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL))
				intersectionType = SIDE_IN_TRIANGLE;
		}
		else if(plane2IntersectionType == Plane::INTERSECTING &&
				plane1IntersectionType == Plane::INTERSECTING)
		{
			if(intersectionPoints[0].SqrDistance(intersectionPoints[1]) < CommonConstants::SQR_PNT_TOL)
				intersectionType = SIDE_TOUCHING_SIDE;
		}
	}
	else if(segment1Valid && !segment2Valid)	// One Line Segment & one point
	{
		Segment3D segment1(intersectPointsA[0], intersectPointsA[1]);
		Segment3D segment2(intersectPointsB[0], intersectPointsB[1]);

		if(!segment1.IsCollinearAndOverlapping(segment2,intersectionPoints))
			return false;

		intersectionFound = true;
		intersectionType = INTERSECTING;

		if(plane2IntersectionType == Plane::SIDE_ON_PLANE &&
		   plane1IntersectionType == Plane::POINT_ON_PLANE)
		{
			if(intersectionPoints[0].SqrDistance(*segment1.GetPoint1()) < CommonConstants::SQR_PNT_TOL ||
			   intersectionPoints[0].SqrDistance(*segment1.GetPoint2()) < CommonConstants::SQR_PNT_TOL)
				intersectionType = COMMON_POINT;
			else
				intersectionType = POINT_TOUCHING_SIDE;
		}
		else if(plane2IntersectionType == Plane::INTERSECTING &&
				plane1IntersectionType == Plane::POINT_ON_PLANE)
		{
			if(intersectionPoints[0].SqrDistance(*segment1.GetPoint1()) < CommonConstants::SQR_PNT_TOL ||
			   intersectionPoints[0].SqrDistance(*segment1.GetPoint2()) < CommonConstants::SQR_PNT_TOL)
				intersectionType = POINT_TOUCHING_SIDE;
			else
				intersectionType = POINT_IN_TRIANGLE;
		}
	}
	else if(!segment1Valid && segment2Valid)	// One Line Segment & one point
	{
		Segment3D segment1(intersectPointsA[0], intersectPointsA[1]);
		Segment3D segment2(intersectPointsB[0], intersectPointsB[1]);

		if(!segment2.IsCollinearAndOverlapping(segment1,intersectionPoints))
			return false;

		intersectionFound = true;
		intersectionType = INTERSECTING;

		if(plane2IntersectionType == Plane::POINT_ON_PLANE &&
		   plane1IntersectionType == Plane::SIDE_ON_PLANE)
		{
			if(intersectionPoints[0].SqrDistance(*segment2.GetPoint1()) < CommonConstants::SQR_PNT_TOL ||
			   intersectionPoints[0].SqrDistance(*segment2.GetPoint2()) < CommonConstants::SQR_PNT_TOL)
				intersectionType = COMMON_POINT;
			else
				intersectionType = POINT_TOUCHING_SIDE;
		}
		else if(plane2IntersectionType == Plane::POINT_ON_PLANE && 
				plane1IntersectionType == Plane::INTERSECTING)
		{
			if(intersectionPoints[0].SqrDistance(*segment2.GetPoint1()) < CommonConstants::SQR_PNT_TOL ||
			   intersectionPoints[0].SqrDistance(*segment2.GetPoint2()) < CommonConstants::SQR_PNT_TOL)
				intersectionType = POINT_TOUCHING_SIDE;
			else
				intersectionType = POINT_IN_TRIANGLE;
		}
	}
	else if(!segment1Valid && !segment2Valid) // two points or one point
	{
		if(plane2IntersectionType == Plane::POINT_ON_PLANE && plane1IntersectionType == Plane::POINT_ON_PLANE)
		{
			if(intersectPointsA[0].SqrDistance(intersectPointsB[0]) < CommonConstants::SQR_PNT_TOL)
			{
				intersectionPoints[0] = intersectPointsA[0];

				intersectionFound = true;
				intersectionType = COMMON_POINT;
			}
		}
	}
	return intersectionFound;
}

//-----------------------------------------------------------------------------	

}