// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// math include files
#include "CUSegment2D.h"
#include "CUBoundingBox2D.h"
#include "CUVector.h"
#include <algorithm>

//util include files
//#include "CUReport.h"
#include "CUMathUtil.h"

namespace CommonUtil
{

Point2D Segment2D::GetPoint(double uVal)const
{
	double x0 = (double)m_point1.GetX();
	double y0 = (double)m_point1.GetY();

	double x1 = (double)m_point2.GetX();
	double y1 = (double)m_point2.GetY();

	float x = (float)(x0 + uVal*(x1 - x0));
	float y = (float)(y0 + uVal*(y1 - y0));

	return Point2D(x, y);
}

//-----------------------------------------------------------------------------

double Segment2D::GetSlope(bool &parallelToYAxis)const
{
	parallelToYAxis = false;

	double x1,y1,x2,y2, slope = -1;
	x1 = m_point1.GetX();
	y1 = m_point1.GetY();
	x2 = m_point2.GetX();
	y2 = m_point2.GetY();

	if(fabs(x2-x1) > 1e-2)
		slope = (y2-y1)/(x2-x1);
	else
		parallelToYAxis = true;

	return slope;
}

//-----------------------------------------------------------------------------

double Segment2D::GetYIntercept(bool &parallelToYAxis)const
{
	parallelToYAxis = false;

	double x1,y1,x2,y2, yIntercept = 0;
	x1 = m_point1.GetX();
	y1 = m_point1.GetY();
	x2 = m_point2.GetX();
	y2 = m_point2.GetY();
	if(fabs(x2-x1) > 1e-2)
		yIntercept = (x2*y1 - x1*y2)/(x2-x1);
	else
		parallelToYAxis = true;

	return yIntercept;
}

//-----------------------------------------------------------------------------

double Segment2D::CheckSign(const Point2D &point)const
{
	double value;
	value = ( (point.GetY() - m_point1.GetY()) *
				(m_point2.GetX() - m_point1.GetX()) ) -
					( (m_point2.GetY() - m_point1.GetY()) *
						(point.GetX() - m_point1.GetX()) );
	return value;
}

//-----------------------------------------------------------------------------

bool Segment2D::Project(const Point2D &projPoint, double snappingTol,
						double &uVal)const
{
	bool stat = false;

	snappingTol = fabs(snappingTol);

	double x0 = projPoint.GetX();
	double y0 = projPoint.GetY();

	double x1 = m_point1.GetX();
	double y1 = m_point1.GetY();

	double x2 = m_point2.GetX();
	double y2 = m_point2.GetY();

	double numerator = (x0-x1)*(x2-x1) + (y0-y1)*(y2-y1);

	double denominator = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);

	if(denominator > CommonConstants::ZERO)
	{
		uVal = numerator / denominator;

		if (uVal > -snappingTol && uVal < (1+snappingTol))
		{
			stat = true;
			if( uVal < 0.0 && uVal > -snappingTol)
			{
				uVal = 0;
			}
			else if(uVal > 1 &&  uVal < (1 + snappingTol))
			{
				uVal = 1;
			}
		}
	}
	return stat;
}

//-----------------------------------------------------------------------------

Point2D Segment2D::GetPointOnCurve(double uVal)const
{
	double x0 = m_point1.GetX();
	double y0 = m_point1.GetY();

	double x1 = m_point2.GetX();
	double y1 = m_point2.GetY();

	double x = x0 + uVal*(x1-x0);
	double y = y0 + uVal*(y1-y0);

	return Point2D(x, y);
}

//-----------------------------------------------------------------------------

Segment2D Segment2D::GetOffset(const Point2D &point, double offsetVal)
{
	Point2D p1 = GetStartPoint();
	Point2D p2 = GetEndPoint();

	Point3D p3 = (Point3D)p1;
	Point3D p4 = (Point3D)p2;

	Vector tempVec1(p3, p4);
	Vector tempVec2(0, 0, 1);
	Vector vec;
	tempVec1.CrossProduct(tempVec2, vec);

	Point3D point1, point2;
	vec.GetPointAlongVector(p3, point1, offsetVal);
	vec.GetPointAlongVector(p4, point2, offsetVal);

	Point2D tempPoint(point1.GetX(), point1.GetY());

	double val = CheckSign(point);
	double val1 = CheckSign(tempPoint);
	double val2 = val * val1;

	if(val2 < 0)
	{
		vec.Reverse();
		vec.GetPointAlongVector(p3, point1, offsetVal);
		vec.GetPointAlongVector(p4, point2, offsetVal);
	}

	Point2D point11(point1.GetX(), point1.GetY());
	Point2D point22(point2.GetX(), point2.GetY());

	Segment2D offSetSegment(point11, point22);
	return offSetSegment;
}

//-----------------------------------------------------------------------------

void Segment2D::Tessellate(const std::vector<Point2D> &points,
						   double tessellationTol)const
{
	//to be coded
}

//-----------------------------------------------------------------------------

BoundingBox2D Segment2D::GetBoundingBox(double expansionTol)const
{
	std::vector<const Point2D *> points;
	points.push_back(&m_point1);
	points.push_back(&m_point2);

	BoundingBox2D box(points, expansionTol);
	return box;
}

//-----------------------------------------------------------------------------

void Segment2D::Transform(const Transform2D &transform)
{
	m_point1.Transform(transform);
	m_point2.Transform(transform);
}

//-----------------------------------------------------------------------------

Point2D Segment2D::GetStartPoint()const
{
	return m_point1;
}

//-----------------------------------------------------------------------------

Point2D Segment2D::GetEndPoint()const
{
	return m_point2;
}

//-----------------------------------------------------------------------------

Point2D Segment2D::GetMidPoint()const
{
	Point2D startPoint = GetStartPoint();
	Point2D endPoint = GetEndPoint();

	double midPointXVal = (startPoint.GetX() + endPoint.GetX())/2.0;
	double midPointYVal = (startPoint.GetY() + endPoint.GetY())/2.0;
	Point2D midPoint(midPointXVal, midPointYVal);
	return midPoint;
}

//-----------------------------------------------------------------------------

void Segment2D::Dump(Report &report, int tabIndent)const
{
	/*report.AddMarker(tabIndent);

	report.Add(tabIndent, "Segment2D\n");

	report.Add(tabIndent, "m_point1\n");
	m_point1.Dump(report, tabIndent + 1);

	report.Add(tabIndent, "m_point2\n");
	m_point2.Dump(report, tabIndent + 1);

	report.AddMarker(tabIndent);*/
}

//-----------------------------------------------------------------------------


bool Segment2D::IsIntersecting(const Segment2D &segment, const double tol)const

{
	bool status = false;

	double p0x0 = m_point1.GetX();
	double p0y0 = m_point1.GetY();

	double p1x1 = m_point2.GetX();
	double p1y1 = m_point2.GetY();

	double q0x0 = segment.m_point1.GetX();
	double q0y0 = segment.m_point1.GetY();

	double q1x1 = segment.m_point2.GetX();
	double q1y1 = segment.m_point2.GetY();

	double a1 = p1x1 - p0x0;
	double b1 = -(q1x1 - q0x0);
	double c1 = q0x0 - p0x0;

	double a2 = p1y1 - p0y0;
	double b2 = -(q1y1 - q0y0);
	double c2 = q0y0 - p0y0;

	double x, y;

	//define***

	status = MathUtil::SolveTwoLinearEquationsByCramersRule(a1, b1, c1,
															a2, b2, c2,
												            x, y);

	//define***

	if(status)
	{
		double min = 0 - tol;
		double max = 1 + tol;

		if(min < x && x < max && min < y && y < max)
			status = true;
		else
			status = false;

	}

	return status;
}

//-----------------------------------------------------------------------------

bool Segment2D::IsIntersectingNew(const Segment2D &testSegment, bool &isExactIntersection,
	bool &isBoundaryIntersection, bool &isParallel, const double tol)const
{
	bool isIntersecting = false;

	double val1 = CheckSign(testSegment.GetP1());
	double val2 = CheckSign(testSegment.GetP2());

	if (fabs(val1) < tol)  // magnitude is Not less than PNT_TOL (if yes,set it as = 0.0f)
		val1 = 0.0;
	if (fabs(val2) < tol)  // magnitude is Not less than PNT_TOL (if yes,set it as = 0.0f)
		val2 = 0.0;

	if (val1 * val2 > 0.0f)	 // Segment is in one half-plane (both points have same sign)
		return false;		 // EARLY EXIT

	double val3 = testSegment.CheckSign(GetP1());
	double val4 = testSegment.CheckSign(GetP2());

	if (fabs(val3) < tol)  // magnitude is Not less than PNT_TOL (if yes,set it as = 0.0f)
		val3 = 0.0;
	if (fabs(val4) < tol)  // magnitude is Not less than PNT_TOL (if yes,set it as = 0.0f)
		val4 = 0.0;

	if (val3 * val4 > 0.0f)	 // Segment is in one half-plane (both points have same sign)
		return false;		 // EARLY EXIT

	if (val1 * val2 < 0.0f && val3 * val4 < 0.0f)
	{
		isIntersecting = true;
		isExactIntersection = true;	// Exact Intersecting
	}
	else if (val1 * val2 < 0.0f)	// means (val3 * val4) == 0.0f
	{
		// segment touching other segment - boundary Intersection
		isIntersecting = true;
		isBoundaryIntersection = true;
	}
	else if (val3 * val4 < 0.0f)	// means (val1 * val2) == 0.0f
	{
		// segment touching other segment - boundary Intersection
		isIntersecting = true;
		isBoundaryIntersection = true;
	}
	else if (val1 * val2 == 0.0f && val3 * val4 == 0.0f)
	{
		if ((val1 == 0 && val2 == 0) || (val3 == 0 && val4 == 0))	// check for collinear segments
		{
			isParallel = true;

			// May be Intersecting or not	// check for segment overlap
			Point2D intersectionPoints[2];
			isIntersecting = IsCollinearAndOverlapping(testSegment, intersectionPoints);
			if (isIntersecting && intersectionPoints[0].SqrDistance(intersectionPoints[1]) < tol * tol)
				isParallel = false;
		}
		else
		{
			// common Point //isCommonPoint = true;
			isIntersecting = true;
		}
	}

	return isIntersecting;
}

//---------------------------------------------------------------------

bool Segment2D::IsCollinear(const Point2D &point)const
{
	Vector vector1(m_point1.GetX() - point.GetX(), m_point1.GetY() - point.GetY(), 0);
	Vector vector2(m_point2.GetX() - point.GetX(), m_point2.GetY() - point.GetY(), 0);

	if (vector1.GetMagnitude() < CommonConstants::ZERO || vector2.GetMagnitude() < CommonConstants::ZERO)
		return true;

	double angle;
	if (vector1.GetAngle(vector2, angle))
	{
		double collinearityTol = 1e-4;
		if (angle < collinearityTol || CommonConstants::PI - angle < collinearityTol)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

bool Segment2D::IsCollinear(const Segment2D &segment)const
{
	if (IsCollinear(segment.GetP1()) && IsCollinear(segment.GetP2()))
		return true;

	return false;
}

//-----------------------------------------------------------------------------

bool Segment2D::GetIntersection(const Segment2D& otherSegment, Point2D &intersectionPoint)const
{
	//bool stat = IsIntersecting(otherSegment);
	double x1 = m_point1.GetX();
	double x2 = m_point2.GetX();
	double y1 = m_point1.GetY();
	double y2 = m_point2.GetY();

	Point2D point3 = otherSegment.GetStartPoint();
	Point2D point4 = otherSegment.GetEndPoint();
	double x3 = point3.GetX();
	double x4 = point4.GetX();
	double y3 = point3.GetY();
	double y4 = point4.GetY();

	if (1)
	{
		double x = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4) )
			/ ((x1 - x2)*(y3-y4) - (y1- y2)*(x3-x4));

		double y = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))
			/ ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

		intersectionPoint.Set(x, y);
	}
	return true;
}
//-----------------------------------------------------------------------------

bool Segment2D::IsCollinearAndOverlapping(const Segment2D &segment, Point2D intersectionPoints[2])const
{
	if (!IsCollinear(segment))
		return false;

	double u1 = 0.0;
	double u2 = 1.0;
	double u3 = -1;
	double u4 = -1;

	Project(segment.GetP1(), CommonConstants::PNT_TOL, u3);
	Project(segment.GetP2(), CommonConstants::PNT_TOL, u4);

	// uVlas for this segment are taken as u1=0, & u2=1.
	// If the intervals (u1, u2) and (u3, u4) overlaps,

	double l = std::min(u3, u4);
	double r = std::max(u3, u4);
	double A = std::max(u1, l);
	double B = std::min(u2, r);

	if (A > B)
	{
		// no intersection
		return false;
	}
	else if (A == B)
	{
		// only A is intersecting
		intersectionPoints[0] = GetPoint(A);
		intersectionPoints[1] = GetPoint(A);
	}
	else
	{
		// if (A < B) interval A to B is intersecting
		intersectionPoints[0] = GetPoint(A);
		intersectionPoints[1] = GetPoint(B);
	}

	return true;
}

//-----------------------------------------------------------------------------
}
