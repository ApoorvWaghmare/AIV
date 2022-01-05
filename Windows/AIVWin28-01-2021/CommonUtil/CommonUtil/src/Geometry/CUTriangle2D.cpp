// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//math include files
#include "CUTriangle2D.h"
#include "CUSegment2D.h"
#include "CUVector.h"

//util include files
//#include "CUReport.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

bool Triangle2D::CheckInsideTriangle(const Point2D &point, bool *onBoundary,
								bool *onVertex, size_t *parallelEdgeIndices,
								double inSideTolerance)const
{
	bool status = false;

	//Forming  3 lines containing vertetices of the triangle
	Segment2D line1(m_point[0], m_point[1]);
	Segment2D line2(m_point[0], m_point[2]);
	Segment2D line3(m_point[1], m_point[2]);

	//Checking that the third verex of triangle and given point lie on the same side
	bool status1 = false;
	double value11 = line1.CheckSign(point);
	double value21 = line2.CheckSign(point);
	double value31 = line3.CheckSign(point);

	if (fabs(value11) < CommonConstants::PNT_TOL)
		value11 = 0.0;
	if (fabs(value21) < CommonConstants::PNT_TOL)
		value21 = 0.0;
	if (fabs(value31) < CommonConstants::PNT_TOL)
		value31 = 0.0;

	if (fabs(value11) < inSideTolerance ||
		fabs(value21) < inSideTolerance ||
		fabs(value31) < inSideTolerance)
	{
		//point is on the edge which is not to be accepted
		if (onBoundary)
			*onBoundary = true;

		if (fabs(value11) < inSideTolerance)
		{
			parallelEdgeIndices[0] = 0;
			parallelEdgeIndices[1] = 1;
		}
		if (fabs(value21) < inSideTolerance)
		{
			parallelEdgeIndices[0] = 0;
			parallelEdgeIndices[0] = 2;
		}

		if (fabs(value31) < inSideTolerance)
		{
			parallelEdgeIndices[0] = 2;
			parallelEdgeIndices[0] = 1;
		}
	}

	if (fabs(value11) < inSideTolerance && fabs(value21) < inSideTolerance ||
		fabs(value21) < inSideTolerance && fabs(value31) < inSideTolerance ||
		fabs(value11) < inSideTolerance && fabs(value31) < inSideTolerance)
	{
		if (onVertex)
			*onVertex = true;
	}

	double value12 = line1.CheckSign(m_point[2]);

	/*if (fabs(value12) < CommonConstants::PNT_TOL)
		value12 = 0.0;*/

	if (value11 * value12 >= 0)
		status1 = true;

	bool status2 = false;

	double value22 = line2.CheckSign(m_point[1]);

	/*if (fabs(value22) < CommonConstants::PNT_TOL)
		value22 = 0.0;*/

	if (value21 * value22 >= 0)
		status2 = true;

	bool status3 = false;

	double value32 = line3.CheckSign(m_point[0]);

	/*if (fabs(value32) < CommonConstants::PNT_TOL)
		value32 = 0.0;*/

	if (value31 * value32 >= 0)
		status3 = true;

	if (status1 && status2 && status3)
		status = true;

	return status;
}

//-----------------------------------------------------------------------------

void Triangle2D::OrientTriangle()
{
	Vector v1(m_point[0], m_point[1]);
	Vector v2(m_point[0], m_point[2]);

	Vector product;
	v1.CrossProduct(v2, product);

	if(product.GetK() < 0)
	{
		Point2D tempPoint =  m_point[1];
		SetPoint(1, m_point[2]);
		SetPoint(2, tempPoint);
	}
}

//-----------------------------------------------------------------------------

double Triangle2D::GetAngle(int index)const
{
	int currentIndex = index;
	int previousIndex, nextIndex;
	if(index == 0)
		previousIndex = 2;
	else
		previousIndex = index - 1;

	if(index == 2)
		nextIndex = 0;
	else
		nextIndex = index + 1;

	Vector tempVector1(m_point[currentIndex], m_point[previousIndex]);
	Vector tempVector2(m_point[currentIndex], m_point[nextIndex]);
	double angle;
	tempVector1.GetAngle(tempVector2, angle);

	return angle;
}

//-----------------------------------------------------------------------------

std::vector<Segment2D> Triangle2D::GetSegments()const
{
	std::vector<Segment2D>segments;
	Segment2D segment1(GetPoint(0), GetPoint(1));
	Segment2D segment2(GetPoint(1), GetPoint(2));
	Segment2D segment3(GetPoint(2), GetPoint(0));

	segments.push_back(segment1);
	segments.push_back(segment2);
	segments.push_back(segment3);
	
	return segments;
}

//-----------------------------------------------------------------------------

int Triangle2D::GetMinimumAngleIndex()const
{
	double angle = DBL_MAX;
	int pointIndex = -1;

	for(int i = 0; i < 3; i++)
	{
		double tempAngle = GetAngle(i);
		if(angle > tempAngle)
		{
			angle = tempAngle;
			pointIndex = i;
		}
	}
	return pointIndex;
}

//-----------------------------------------------------------------------------

bool Triangle2D::IsValidTriangle()const
{
	bool status = true;

	double distance1 = m_point[0].Distance(m_point[1]);
	double distance2 = m_point[0].Distance(m_point[2]);
	double distance3 = m_point[1].Distance(m_point[2]);
	if (distance1 < CommonConstants::PNT_TOL || distance2 < CommonConstants::PNT_TOL ||
		distance3 < CommonConstants::PNT_TOL)
		status = false;

	return status;
}

//-----------------------------------------------------------------------------

int Triangle2D::GetRemainingIndex(int minimumIndexNo, int maxIndexNum)const
{
	int remainingIndex = 0;
	if(minimumIndexNo==0 && maxIndexNum==1)
		remainingIndex = 2;
	if(minimumIndexNo==1 && maxIndexNum==2)
		remainingIndex = 0;
	if(minimumIndexNo==0 && maxIndexNum==2)
		remainingIndex = 1;

	return remainingIndex;

}

//-----------------------------------------------------------------------------

int Triangle2D::GetPreviousIndex(int currentIndex)const
{
	int previousIndex;

	if(currentIndex == 0)
		previousIndex = 2;
	else
		previousIndex = currentIndex - 1;

	return previousIndex;
}

//-----------------------------------------------------------------------------

int Triangle2D::GetNextIndex(int currentIndex)const
{
	int nextIndex;

	if(currentIndex == 2)
		nextIndex = 0;
	else
		nextIndex = currentIndex + 1;

	return nextIndex;
}
//-----------------------------------------------------------------------------

double Triangle2D::GetArea()
{
	std::vector<Segment2D> segments;
	segments = GetSegments();

	double totalLength = 0.0;
	std::vector<double> lengths;
	for(int i = 0; i < 3; ++i)
	{
		double length = segments[i].GetLength();
		totalLength += length;
		lengths.push_back(length);
	}

	std::vector<double> values;
	for(int i = 0; i < 3; ++i)
	{
		double value = (totalLength / 2) - lengths[i];
		values.push_back(value);
	}

	double area = sqrt((totalLength / 2) * values[0] * values[1] * values[2]);

	return area;
}

//-----------------------------------------------------------------------------

Point2D Triangle2D::GetCentroid()
{
	double x1 = m_point[0].GetX();
	double x2 = m_point[1].GetX();
	double x3 = m_point[2].GetX();

	double y1 = m_point[0].GetY();
	double y2 = m_point[1].GetY();
	double y3 = m_point[2].GetY();

	double x = (x1 + x2 + x3) / 3;
	double y = (y1 + y2 + y3) / 3;

	Point2D centroid(x, y);
	return centroid;

}

//-----------------------------------------------------------------------------

bool Triangle2D::IsOverlapping(const Triangle2D& triangle)const
{

	Point2D triPoints[3];
	triangle.GetPoints(triPoints);

	std::vector< Point2D > commonPoints;

	if (m_point[0].SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL || m_point[0].SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL || m_point[0].SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
		commonPoints.push_back(m_point[0]);
	if (m_point[1].SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL || m_point[1].SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL || m_point[1].SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
		commonPoints.push_back(m_point[1]);
	if (m_point[2].SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL || m_point[2].SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL || m_point[2].SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
		commonPoints.push_back(m_point[2]);

	if (commonPoints.size() == 3)
	{
		// EXACT OVERLAP found : The given two triangles are EXACT overlapping
		return true;
	}

	if (commonPoints.size() == 2)
	{
		// COMMON_SIDE found:

		Point2D otherPoint1, otherPoint2;
		if (GetThirdPoint(commonPoints[0], commonPoints[1], otherPoint1) &&
			triangle.GetThirdPoint(commonPoints[0], commonPoints[1], otherPoint2))
		{
			Segment2D commonSegment(commonPoints[0], commonPoints[1]);
			double val1 = commonSegment.CheckSign(otherPoint1);
			double val2 = commonSegment.CheckSign(otherPoint2);
			if (val1*val2 > 0.0f) 	// if other points are on same side - OVERLAP=true   
				return true;
			else
				return false;
		}
		else
			return false;
	}

	Segment2D tri1Segment[3];
	Segment2D tri2Segment[3];

	//Forming 3 lines containing vertices of the triangle1
	tri1Segment[0].Set(m_point[0], m_point[1]);
	tri1Segment[1].Set(m_point[0], m_point[2]);
	tri1Segment[2].Set(m_point[1], m_point[2]);

	//Forming 3 lines containing vertices of the triangle2
	tri2Segment[0].Set(triPoints[0], triPoints[1]);
	tri2Segment[1].Set(triPoints[0], triPoints[2]);
	tri2Segment[2].Set(triPoints[1], triPoints[2]);

	bool exactIntersection = false;
	bool boundaryIntersection = false;
	bool overlapping = false;
	size_t cntr = 0;

	for (int t1Seg = 0; t1Seg < 3; ++t1Seg)
	{
		for (int t2Seg = 0; t2Seg < 3; ++t2Seg)
		{
			if (!tri1Segment[t1Seg].IsIntersectingNew(tri2Segment[t2Seg], exactIntersection,
				boundaryIntersection, overlapping))
			{
				cntr++;
				continue;
			}

			if (exactIntersection)
				return true;
			else if (boundaryIntersection)
			{
				// Now check the segments for on/inside the triangles - OVERLAP = true 
				if (triangle.CheckInsideTriangle(tri1Segment[t1Seg].GetP1()) &&
					triangle.CheckInsideTriangle(tri1Segment[t1Seg].GetP2()))
				{
					return true;		// triangle1's segment is on/inside triangle2
				}
				else if (CheckInsideTriangle(tri2Segment[t2Seg].GetP1()) &&
					CheckInsideTriangle(tri2Segment[t2Seg].GetP2()))
				{
					return true;	// triangle2's segment is on/inside triangle1
				}
			}
			else if (overlapping)	// tese are actually collinear segments
			{
				// take 1 parallel segment

				Point2D otherPoint1, otherPoint2;
				if (GetThirdPoint(tri1Segment[t1Seg].GetP1(), tri1Segment[t1Seg].GetP2(), otherPoint1) &&
					triangle.GetThirdPoint(tri2Segment[t2Seg].GetP1(), tri2Segment[t2Seg].GetP2(), otherPoint2))
				{
					Segment2D parallelSegment = tri1Segment[t1Seg];
					double val1 = parallelSegment.CheckSign(otherPoint1);
					double val2 = parallelSegment.CheckSign(otherPoint2);
					if (val1*val2 > 0.0f) 	// if other points are on same side - OVERLAP=true
						return true;
					else
						return false;
				}
				else
					return false;
			}
			else
			{
				if (triangle.CheckInsideTriangle(m_point[0]) &&
					triangle.CheckInsideTriangle(m_point[1]) &&
					triangle.CheckInsideTriangle(m_point[2]))
				{
					return true;		// triangle1's three points are on/inside triangle2
				}
				else if (CheckInsideTriangle(triPoints[0]) &&
					CheckInsideTriangle(triPoints[1]) &&
					CheckInsideTriangle(triPoints[2]))
				{
					return true;	// triangle2's three points are on/inside triangle1
				}
			}

			exactIntersection = false;
			boundaryIntersection = false;
			overlapping = false;
		}
	}

	// At this stage it is assumed that, No segment Intersection occurred so,
	if (cntr == 9 && commonPoints.size() == 0)   // Triangle inside triangle check
	{
		if (CheckInsideTriangle(triPoints[0]) || triangle.CheckInsideTriangle(m_point[0]))
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

bool Triangle2D::GetThirdPoint(const Point2D &p1, const Point2D &p2, Point2D &thirdPoint)const
{
	bool stat = false;
	for (size_t i = 0; i < 3; i++)
	{
		if ((p1.GetX() != m_point[i].GetX() || p1.GetY() != m_point[i].GetY()) &&
			(p2.GetX() != m_point[i].GetX() || p2.GetY() != m_point[i].GetY()))
		{
			thirdPoint = m_point[i];
			stat = true;
			break;
		}
	}
	return stat;

	/*if(p1.SqrDistance(m_point[0]) < CommonConstants::SQR_PNT_TOL && p2.SqrDistance(m_point[1]) < CommonConstants::SQR_PNT_TOL)
	thirdPoint =  m_point[2];
	else if(p1.SqrDistance(m_point[0]) < CommonConstants::SQR_PNT_TOL && p2.SqrDistance(m_point[2]) < CommonConstants::SQR_PNT_TOL)
	thirdPoint =  m_point[1];
	else if(p1.SqrDistance(m_point[1]) < CommonConstants::SQR_PNT_TOL && p2.SqrDistance(m_point[2]) < CommonConstants::SQR_PNT_TOL)
	thirdPoint =  m_point[0];

	return thirdPoint;*/
}

//-----------------------------------------------------------------------------

void Triangle2D::Dump(Report &report, int tabIndent)const
{
	/*report.AddMarker(tabIndent);
	
	report.Add(tabIndent, "Triangle\n");
	for(int i = 0; i < 3; i++)
	{
		const Point2D &tmpPoint = m_point[i];
		tmpPoint.Dump(report, tabIndent + 1);
	}

	report.AddMarker(tabIndent);*/
}

//-----------------------------------------------------------------------------

}