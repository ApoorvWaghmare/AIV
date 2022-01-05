// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// math include files
#include "CUSegment3D.h"
#include "CUVector.h"
#include <algorithm>
#include <float.h>

//util include files
//#include "CUReport.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

bool Segment3D::ParallelInterSectionInfo::IsOverlap()const
{
	bool overlap = false, outsideProjection = false;
	for (int i = 0; i < 4; i++)
	{
		double val = -1;
		switch(i)
		{
		case 0: val = m_Edge1p1ProjectionOnEdge2;
			break;
		case 1: val = m_Edge1p2ProjectionOnEdge2;
			break;
		case 2: val = m_Edge2p1ProjectionOnEdge1;
			break;
		case 3: val = m_Edge2p2ProjectionOnEdge1;
			break;
		}
		if (val > CommonConstants::ZERO && val < (1 - CommonConstants::ZERO))
		{
			overlap = true;
			break;
		}
		else if (val < -CommonConstants::ZERO) //val == -1 when no overlap
			outsideProjection = true;
	}
	if (!overlap && !outsideProjection)
	{
		//this is a complete overlap case
		overlap = true;
	}
	return overlap;
}

//-----------------------------------------------------------------------------

bool Segment3D::GetSkewIntersection(const Segment3D &segment, double &u,
									double &v, double skewTol,
									double uSnappingFactor,
									double vSnappingFactor, double angTol,
									ParallelInterSectionInfo *parallelInfo)const

{
	bool status = false;

	if (parallelInfo)
	{
		//check if the two segments are parallel to each other
		parallelInfo->Initialize();
		if (CheckParallelism(segment, angTol, skewTol))
		{
			//set the parallel information structure
			parallelInfo->m_parallel = true;

			double parameterVal;
			int numProjectSuccess = 0;
			if (segment.Project(m_p1, vSnappingFactor, parameterVal))
			{
				parallelInfo->m_Edge1p1ProjectionOnEdge2 = parameterVal;
				numProjectSuccess++;
			}

			if (segment.Project(m_p2, vSnappingFactor, parameterVal))
			{
				parallelInfo->m_Edge1p2ProjectionOnEdge2 = parameterVal;
				numProjectSuccess++;
			}

			if (Project(segment.GetStartPoint(), uSnappingFactor, parameterVal))
			{
				parallelInfo->m_Edge2p1ProjectionOnEdge1 = parameterVal;
				numProjectSuccess++;
			}

			if (Project(segment.GetEndPoint(), uSnappingFactor, parameterVal))
			{
				parallelInfo->m_Edge2p2ProjectionOnEdge1 = parameterVal;
				numProjectSuccess++;
			}

			//resolve the snapping related abnormalities
			if (fabs(parallelInfo->m_Edge1p1ProjectionOnEdge2) < CommonConstants::ZERO)
			{
				parallelInfo->m_Edge2p1ProjectionOnEdge1 = 0;
			}
			else if (parallelInfo->m_Edge1p1ProjectionOnEdge2 > (1 - CommonConstants::ZERO))
			{
				parallelInfo->m_Edge2p2ProjectionOnEdge1 = 0;
			}

			if (fabs(parallelInfo->m_Edge1p2ProjectionOnEdge2) < CommonConstants::ZERO)
			{
				parallelInfo->m_Edge2p1ProjectionOnEdge1 = 1;
			}
			else if (parallelInfo->m_Edge1p2ProjectionOnEdge2 > (1 - CommonConstants::ZERO))
			{
				parallelInfo->m_Edge2p2ProjectionOnEdge1 = 1;
			}

			//check other segment
			if (fabs(parallelInfo->m_Edge2p1ProjectionOnEdge1) < CommonConstants::ZERO)
			{
				parallelInfo->m_Edge1p1ProjectionOnEdge2 = 0;
			}
			else if (parallelInfo->m_Edge2p1ProjectionOnEdge1 > (1 - CommonConstants::ZERO))
			{
				parallelInfo->m_Edge1p2ProjectionOnEdge2 = 0;
			}

			if (fabs(parallelInfo->m_Edge2p2ProjectionOnEdge1) < CommonConstants::ZERO)
			{
				parallelInfo->m_Edge1p1ProjectionOnEdge2 = 1;
			}
			else if (parallelInfo->m_Edge2p2ProjectionOnEdge1 > (1 - CommonConstants::ZERO))
			{
				parallelInfo->m_Edge1p2ProjectionOnEdge2 = 1;
			}

			if (numProjectSuccess > 1)	//there should be at least
			{							//2 succecssful projections
				status = true;
				parallelInfo->m_collinear = true;
			}
			return status;
		}
	}

	const Point3D& point1 = segment.GetStartPoint();
	const Point3D& point2 = segment.GetEndPoint();

	double x0 = m_p1.GetX();
	double y0 = m_p1.GetY();
	double z0 = m_p1.GetZ();

	double x1 = m_p2.GetX();
	double y1 = m_p2.GetY();
	double z1 = m_p2.GetZ();

	double x00 = point1.GetX();
	double y00 = point1.GetY();
	double z00 = point1.GetZ();

	double x11 = point2.GetX();
	double y11 = point2.GetY();
	double z11 = point2.GetZ();

	double k1 = (x11-x00)*(x11-x00) + (y11-y00)*(y11-y00) + (z11-z00)*(z11-z00);

	double k2 = (x1-x0)*(x11-x00) + (y1-y0)*(y11-y00) + (z1-z0)*(z11-z00);

	double k3 = (x00-x0)*(x11-x00) + (y00-y0)*(y11-y00) + (z00-z0)*(z11-z00);

	double k11 = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) + (z1-z0)*(z1-z0);

	double k22 = (x1-x0)*(x11-x00) + (y1-y0)*(y11-y00) + (z1-z0)*(z11-z00);

	double k33 = (x1-x0)*(x00-x0) + (y1-y0)*(y00-y0) + (z1-z0)*(z00-z0);

	double p1,p2;

	bool skewNotPossible = false;

	if(k1 != 0)
	{
		 p1 = k2 / k1;
		 p2 = k3 / k1;

		 double numerator = k33 - (k22 * p2);
		 double denominator = k11 - (k22 * p1);

		 if(denominator != 0)
		 {
			 u = numerator / denominator;

			 v = u*p1 - p2;
		 }
		 else
		 {
			  skewNotPossible = true;
		 }
	}
	else
	{
		if(k2 != 0)
		{
			if(k22 != 0)
			{
				u = k3 / k2;
				v = (u*k11 - k33) / k22;
			}
			else
			{
			  skewNotPossible = true;
			}

		}

		else
		{
			 skewNotPossible = true;
		}
	}


	if(!skewNotPossible)
	{

		if (u > -CommonConstants::ZERO && u < (1 + CommonConstants::ZERO))
		{
			if( u < uSnappingFactor)
			{
				u = 0;
			}
			else if(u > (1-uSnappingFactor))
			{
				u = 1;
			}

			if (v > -CommonConstants::ZERO && v < (1 + CommonConstants::ZERO))
			{
				if( v < vSnappingFactor)
				{
					v = 0;
				}
				else if(v > (1-vSnappingFactor))
				{
					v = 1;
				}
				status = true;
			}
		}

	}
	return status;
}

//-----------------------------------------------------------------------------

Point3D Segment3D::GetPoint(double val)const
{
	double x0 = m_p1.GetX();
	double y0 = m_p1.GetY();
	double z0 = m_p1.GetZ();

	double x1 = m_p2.GetX();
	double y1 = m_p2.GetY();
	double z1 = m_p2.GetZ();

	double x = x0 + val*(x1-x0);
	double y = y0 + val*(y1-y0);
	double z = z0 + val*(z1-z0);

	return Point3D(x, y, z);
}

//-----------------------------------------------------------------------------

void Segment3D::GetPoint(double val, Point3D& point)const
{
	double x0 = m_p1.GetX();
	double y0 = m_p1.GetY();
	double z0 = m_p1.GetZ();

	double x1 = m_p2.GetX();
	double y1 = m_p2.GetY();
	double z1 = m_p2.GetZ();

	double x = x0 + val*(x1-x0);
	double y = y0 + val*(y1-y0);
	double z = z0 + val*(z1-z0);

	point.Set(x, y, z);
}

//-----------------------------------------------------------------------------

bool Segment3D::Project(const Point3D &projPoint, double snappingTol,
						double &uVal)const
{
    bool stat = false;

    snappingTol = fabs(snappingTol); // added on 11aug06

    const double x0 = projPoint.GetX();
    const double y0 = projPoint.GetY();
    const double z0 = projPoint.GetZ();

    const double x1 = m_p1.GetX();
    const double y1 = m_p1.GetY();
    const double z1 = m_p1.GetZ();

    const double x2 = m_p2.GetX();
    const double y2 = m_p2.GetY();
    const double z2 = m_p2.GetZ();

    const double x2_x1 = x2-x1;
    const double y2_y1 = y2-y1;
    const double z2_z1 = z2-z1;
    const double denominator = (x2_x1)*(x2_x1) + (y2_y1)*(y2_y1) + (z2_z1)*(z2_z1);

	if (denominator > CommonConstants::ZERO)
    {
        const double numerator = (x0-x1)*(x2_x1) + (y0-y1)*(y2_y1) + (z0-z1)*(z2_z1);
		double denomTimesSnapTol;
		if((denominator > 1.0) && (snappingTol == DBL_MAX))
		{
			denomTimesSnapTol = DBL_MAX;
		}
		else
		{
			denomTimesSnapTol = denominator * snappingTol;
		}

        // Since denominator is always positive, we can optimize the code.
        // Old code (less optimized)
        // uVal = numerator / denominator;
        // if (uVal > -snappingTol && uVal < (1+snappingTol))
        //  ...    if( uVal < 0.0 && uVal > -snappingTol )
        //  ...    else if(uVal > 1 &&  uVal < (1 + snappingTol))
        if (numerator > -denomTimesSnapTol && numerator < (denominator+denomTimesSnapTol))
        {
            stat = true;
            if( numerator < 0.0 && numerator > -denomTimesSnapTol ) //changed on May 26\08
            {
                uVal = 0.0;
            }
            else if(numerator > denominator && numerator < (denominator+denomTimesSnapTol)) //changed on May 26\08
            {
                uVal = 1.0;
            }
            else
            {
                uVal = numerator / denominator;
            }
        }
        else
        {
            uVal = numerator / denominator;
        }
    }
    else
    {
        // otherwise this is undetermined if the caller did not initiated the variable.
        uVal = 0.0;
    }
    return stat;
}

//-----------------------------------------------------------------------------

bool Segment3D::Project(const Point3DF &projPoint, double snappingTol,
	double &uVal)const
{
	CommonUtil::Point3D tempPt(projPoint.GetX(), projPoint.GetY(), projPoint.GetZ());
	return Project(tempPt, snappingTol, uVal);
}

/*
//-----------------------------------------------------------------------------

bool Segment3D::checkParallelism(const Segment3D &segment, double angTol)const
{
	Vector v1(getStartPoint(), getEndPoint());
	Vector v2(segment.getStartPoint(), segment.getEndPoint());

	Vector::Direction dir;

	return v1.isParallel(v2, dir, angTol);
}

//-----------------------------------------------------------------------------
*/

//-----------------------------------------------------------------------------

bool Segment3D::CheckParallelism(const Segment3D &segment, double angTol,
								 double skewTol)const
{
	bool stat = false;

	Vector v1(GetStartPoint(), GetEndPoint());
	Vector v2(segment.GetStartPoint(), segment.GetEndPoint());
	v1.Normalize();
	v2.Normalize();
	Vector::Direction dir;

	stat = v1.IsParallel(v2, dir, angTol);
	if (stat)
	{
		double distance = GetDistance(segment.GetStartPoint());
		if (distance < skewTol)
			stat = true;
		else
			stat = false;
	}
	return stat;
}

//-----------------------------------------------------------------------------

bool Segment3D::CheckOnSegment(const Point3D &projPoint)const
{
	bool stat = false;

	Vector v1(m_p1, projPoint);
	Vector v2(projPoint, m_p2);
	Vector v3(m_p1, m_p2);

	if(v1.DotProduct(v2) > 0)
	{
		if(fabs(v1.GetMagnitude() + v2.GetMagnitude() -
			v3.GetMagnitude()) < CommonConstants::ZERO)
			stat = true;
	}

	return stat;
}

//-----------------------------------------------------------------------------

double Segment3D::GetDistance(const Point3D &point)const
{
	double diff = GetSquareDistance(point);
	if(diff > 0.)
		return sqrt(diff);//sqrDis - disAlongSeg*disAlongSeg);
	else
		return 0.;
}

//-----------------------------------------------------------------------------

double Segment3D::GetSquareDistance(const Point3D &point)const
{
	Vector v1(m_p1, m_p2);
	v1.Normalize();

	Vector v2(m_p2, point);

	double disAlongSeg = v1.DotProduct(v2);
	double sqrDis = point.SqrDistance(m_p2);
	double diff = sqrDis - disAlongSeg*disAlongSeg;
	return diff;

}

//-----------------------------------------------------------------------------

BoundingBox3D Segment3D::GetBoundingBox(double expansionTol)const
{
	std::vector<const Point3D *> points;
	points.push_back(&m_p1);
	points.push_back(&m_p2);

	BoundingBox3D box(points, expansionTol);
	return box;
}

//-----------------------------------------------------------------------------

void Segment3D::Dump(Report &report, int tabIndent)const
{
	/*report.AddMarker(tabIndent);

	report.Add(tabIndent, "Segment3D\n");
	report.Add(tabIndent, "m_p1\n");
	m_p1.Dump(report, tabIndent + 1);

	report.Add(tabIndent, "m_p2\n");
	m_p2.Dump(report, tabIndent + 1);

	report.AddMarker(tabIndent);*/
}

//-----------------------------------------------------------------------------

void Segment3D::Transform(const Transform3D &transform)
{
	m_p1.Transform(transform);
	m_p2.Transform(transform);
}

//-----------------------------------------------------------------------------

void Segment3D::Tessellate(const std::vector<CommonUtil::Point3D> &points,
							double tessellationTol)const
{
	//to be coded
}

//-----------------------------------------------------------------------------

void Segment3D::Transform(const SqMatrix &Rotation,
						  const std::vector<double> &translation)
{
	//to be coded
}

//-----------------------------------------------------------------------------

//returns the first point of the segment
Point3D Segment3D::GetStartPoint()const
{
	//to be coded
	Point3D p;
	p = m_p1;
	return p;
}

//-----------------------------------------------------------------------------

//returns the second point of the segment
Point3D Segment3D::GetEndPoint()const
{
	//to be code
	Point3D p;
	p = m_p2;
	return p;
}

//-----------------------------------------------------------------------------

bool Segment3D::IsPointWithinParamVals(const Point3D& point,
									   double snappingTol)const
{
	double uVal;
	Project(point, snappingTol, uVal);

	bool stat = false;
	if (uVal > -CommonConstants::ZERO && uVal < (1 + CommonConstants::ZERO))
		stat = true;

	return stat;
}

//-----------------------------------------------------------------------------

double Segment3D::GetMaxDeviation(const std::vector<CommonUtil::Point3D> &dataPoints)const
{
	size_t numDataPoints = dataPoints.size();
	double maxDeviation = -1;
	for(int i = 0; i < numDataPoints; i++)
	{
		const Point3D &point = dataPoints[i];
		double uVal;
		Project(point, 0 , uVal);

		//code modified
		//************************************************
		if(uVal > 1)
			uVal = 1;

		else if(uVal < 0)
			uVal = 0;

		//************************************************
		Point3D pointOnSegment = GetPoint(uVal);

		double dist = point.Distance(pointOnSegment);
		if(dist > maxDeviation)
		{
			maxDeviation = dist;
		}
	}
	return maxDeviation;
}

//-----------------------------------------------------------------------------

bool Segment3D::ProjectPoint(const Point3D& pointToBeProjected,
							 double snappingTol, Point3D &projectionPoint, bool pointOnsegement)const
{
	double uVal;
	bool stat = true;

	if(pointOnsegement)
		stat = Project(pointToBeProjected, snappingTol, uVal);
	else
	{
		stat = Project1(pointToBeProjected, snappingTol, uVal);

		if(!stat)
			projectionPoint = m_p1;
	}

	if(stat)
		projectionPoint = GetPoint(uVal);

	return stat;
}


//-----------------------------------------------------------------------------

bool Segment3D::Project1(const Point3D &projPoint, double snappingTol,
							double &uVal)const
{
	 bool stat = false;

    snappingTol = fabs(snappingTol); // added on 11aug06

    const double x0 = projPoint.GetX();
    const double y0 = projPoint.GetY();
    const double z0 = projPoint.GetZ();

    const double x1 = m_p1.GetX();
    const double y1 = m_p1.GetY();
    const double z1 = m_p1.GetZ();

    const double x2 = m_p2.GetX();
    const double y2 = m_p2.GetY();
    const double z2 = m_p2.GetZ();

    const double x2_x1 = x2-x1;
    const double y2_y1 = y2-y1;
    const double z2_z1 = z2-z1;
    const double denominator = (x2_x1)*(x2_x1) + (y2_y1)*(y2_y1) + (z2_z1)*(z2_z1);

	if (denominator > CommonConstants::ZERO)
    {
        const double numerator = (x0-x1)*(x2_x1) + (y0-y1)*(y2_y1) + (z0-z1)*(z2_z1);
        const double denomTimesSnapTol = denominator * snappingTol;

        // Since denominator is always positive, we can optimize the code.
        // Old code (less optimized)
        // uVal = numerator / denominator;
        // if (uVal > -snappingTol && uVal < (1+snappingTol))
        //  ...    if( uVal < 0.0 && uVal > -snappingTol )
        //  ...    else if(uVal > 1 &&  uVal < (1 + snappingTol))

         uVal = numerator / denominator;

		 stat = true;
    }
    return stat;
}

//-----------------------------------------------------------------------------

double Segment3D::ComputeAngleMadeByTwoPoints(const Point3D &point1, const Point3D &point2)const
{
	double angle = -1;
	double uVal = 0;

	Project(point1, 0, uVal);
	Point3D edgePoint1 = GetPoint(uVal);
	Vector v1(edgePoint1, point1);

	Project(point2, 0, uVal);
	Point3D edgePoint2 = GetPoint(uVal);
	Vector v2(edgePoint2, point2);

	if (v1.GetSqrMagnitude() > CommonConstants::SQR_PNT_TOL && v2.GetSqrMagnitude() > CommonConstants::SQR_PNT_TOL)
	{
		v1.GetAngle(v2, angle);
	}

	return angle;
}

//-----------------------------------------------------------------------------

bool Segment3D::IsCollinear(const Point3D &point)const
{
	Vector vector1(point, m_p1);
	Vector vector2(point, m_p2);

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

bool Segment3D::IsCollinear(const Segment3D &segment)const
{
	if (IsCollinear(*segment.GetPoint1()) && IsCollinear(*segment.GetPoint2()))
		return true;

	return false;
}

//---------------------------------------------------------------------

bool Segment3D::IsCollinearAndOverlapping(const Segment3D &segment, Point3D intersectionPoints[2])const
{
	if (!IsCollinear(segment))
		return false;

	double u1 = 0.0;
	double u2 = 1.0;
	double u3 = -1;
	double u4 = -1;

	Project(*(segment.GetPoint1()), CommonConstants::PNT_TOL, u3);
	Project(*(segment.GetPoint2()), CommonConstants::PNT_TOL, u4);

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

//---------------------------------------------------------------------

bool Segment3D::CheckOnSeg(const Point3D *projPoint)const
{
	bool stat = false;

	Vector v1(m_p1, *projPoint);
	Vector v2(*projPoint, m_p2);
	Vector v3(m_p1, m_p2);

	if (v1.DotProduct(v2) > 0)
	{
		if (fabs(v1.GetMagnitude() + v2.GetMagnitude() - v3.GetMagnitude()) < CommonConstants::ZERO)
			stat = true;
	}

	return stat;
}

//---------------------------------------------------------------------
}
