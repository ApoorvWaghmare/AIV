// ============================================================================
//			Copyright (c) 2018 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <algorithm>

#include "CUSegment3D.h"
#include "CUHatchLineCalculator.h"
#include "CUPlane.h"
namespace CommonUtil
{
	bool HatchLineCalculator::sortPointuValStructByuVal(const PointuValStruct& p1, const PointuValStruct&p2)
	{
		return (p1.m_uVal < p2.m_uVal);
	}

	void HatchLineCalculator::getlimitingDistanceInBothDirection(double &distanceInGrowDir, double &distanceInOppositeDir)
	{
		Point3D endPoint;
		m_growDirection.GetPointAlongVector(m_startPoint, endPoint, 100);
		Segment3D growAlongSeg(m_startPoint, endPoint);
		double uVal = 0., maxuVal = -DBL_MAX, minuVal = DBL_MAX;
		Point3D maxPoint, minPoint;
		for (size_t iCount = 0; iCount < m_polygonPoints.size(); iCount++)
		{
			growAlongSeg.Project(m_polygonPoints[iCount], CommonConstants::PNT_TOL, uVal);
			if (uVal > maxuVal)
			{
				maxuVal = uVal;
				maxPoint = growAlongSeg.GetPoint(uVal);
			}
				
			if (uVal < minuVal)
			{
				minuVal = uVal;
				minPoint = growAlongSeg.GetPoint(uVal);
			}
		}
		distanceInGrowDir = m_startPoint.Distance(maxPoint);
		distanceInOppositeDir = m_startPoint.Distance(minPoint);
	}
	//-----------------------------------------------------------------------------

	void HatchLineCalculator::getHatchLinesInGivenDir(Vector& growDir, const std::vector<Segment3D>&polygonSegments, const double &distanceInGivenDir, std::vector<Segment3D>& hatchLines)
	{
		if (distanceInGivenDir == 0.0)
			return;
		Segment3D::ParallelInterSectionInfo* info = new Segment3D::ParallelInterSectionInfo;
		Point3D tmpStartPoint = m_startPoint, endPoint, tmpEndPoint;
		m_hatchlineDirection.GetPointAlongVector(tmpStartPoint, endPoint, 10);
		tmpEndPoint = endPoint;
		int counter = 0;
		while (1)
		{
			Segment3D hatchSeg(tmpStartPoint, tmpEndPoint);
			Point3D actualPoint1, actualPoint2;
			std::vector<PointuValStruct>intersectionPoints;
			for (size_t polySegCount = 0; polySegCount < polygonSegments.size(); polySegCount++)
			{
				double uVal = 0., vVal = 0.;

				hatchSeg.GetSkewIntersection(polygonSegments[polySegCount], uVal, vVal, DBL_MAX, 1e-5, 1e-5, 1e-5, info);

				if (info->m_collinear)
				{
					continue;
				}
				else
				{
					//if (fabs(vVal) >= 0 && fabs(vVal) <= 1 + 1e-5)
					if (vVal >= 0 - +1e-5 && vVal <= 1 + 1e-5)
					{
						PointuValStruct ptStruct;
						ptStruct.m_uVal = uVal;
						ptStruct.m_point = hatchSeg.GetPoint(uVal);

						intersectionPoints.push_back(ptStruct);
					}					
				}
			}
			if (intersectionPoints.size() && (intersectionPoints.size() % 2 == 0))
			{
				std::sort(intersectionPoints.begin(), intersectionPoints.end(), sortPointuValStructByuVal);
				for (size_t intPtCount = 0; intPtCount < intersectionPoints.size()/2; intPtCount++)
				{
					hatchLines.push_back(Segment3D(intersectionPoints[intPtCount * 2].m_point, intersectionPoints[intPtCount * 2 + 1].m_point));
				}
			}
			else if (intersectionPoints.size())
			{
				std::sort(intersectionPoints.begin(), intersectionPoints.end(), sortPointuValStructByuVal);
				hatchLines.push_back(Segment3D(intersectionPoints[0].m_point, intersectionPoints[intersectionPoints.size() - 1].m_point));
			}
			counter++;
			growDir.GetPointAlongVector(m_startPoint, tmpStartPoint, counter*m_lineSpacing);
			growDir.GetPointAlongVector(endPoint, tmpEndPoint, counter*m_lineSpacing);
			
			if (distanceInGivenDir < counter*m_lineSpacing)
				break;			
		}
		delete info;
	}
	//-----------------------------------------------------------------------------

	HatchLineCalculator::HatchLineCalculator(const std::vector<Point3D>& polygonPoints, Vector& hatchLineDir, 
		Point3D& startPoint, double lineSpacing)
	{
		m_polygonPoints = polygonPoints;
		m_startPoint = startPoint;
		m_hatchlineDirection = hatchLineDir;
		m_lineSpacing = lineSpacing;
	}
	//-----------------------------------------------------------------------------

	bool HatchLineCalculator::Execute(std::vector<Segment3D>& hatchLines)
	{
		bool stat = false;
		
		if (m_polygonPoints.size() < 3)
		{
			return false;
		}

		//calculate polygon normal
		Vector v1(m_polygonPoints[0], m_polygonPoints[1]);
		Vector v2(m_polygonPoints[1], m_polygonPoints[2]);
		Vector polygonNormal;
		v1.CrossProduct(v2, polygonNormal);
		polygonNormal.Normalize();
		CommonUtil::Plane plane;
		plane.SetBasePoint(m_polygonPoints[0]);
		plane.SetNormal(polygonNormal);
		m_hatchlineDirection = plane.ProjectVector(m_hatchlineDirection);
		m_hatchlineDirection.Normalize();
		m_hatchlineDirection.CrossProduct(polygonNormal, m_growDirection);
		m_growDirection.Normalize();
		//make polygon segments
		std::vector<Segment3D>polygonSegments;
		Point3D p1, p2;
		for (size_t ptCount = 0; ptCount < m_polygonPoints.size(); ptCount++)
		{
			p1 = m_polygonPoints[ptCount];
			if (ptCount == m_polygonPoints.size() - 1)
			{
				p2 = m_polygonPoints[0];
			}
			else
			{
				p2 = m_polygonPoints[ptCount + 1];
			}
			polygonSegments.push_back(Segment3D(p1, p2));
		}

		double distInGrowDir, distInOppositeDir;
		getlimitingDistanceInBothDirection(distInGrowDir, distInOppositeDir);
		//make hatch segments
		getHatchLinesInGivenDir(m_growDirection, polygonSegments, distInGrowDir, hatchLines);
		m_growDirection.Reverse();
		getHatchLinesInGivenDir(m_growDirection, polygonSegments, distInOppositeDir, hatchLines);
		return stat;
	}
	//-----------------------------------------------------------------------------
}