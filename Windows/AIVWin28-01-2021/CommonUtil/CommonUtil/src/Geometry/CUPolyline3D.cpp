// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Std Include Files
#include <assert.h>

// Include Files
//#include "constants.h"
#include "CUpolyline3d.h"
#include "CUsegment3d.h"
#include "CUCommonConstants.h"

namespace CommonUtil
{
struct SegmentUvals
{
	double m_u1;
	double m_u2;
};

//-----------------------------------------------------------------------------

bool Polyline3D::checkSegment(double u1, double u2, double smoothenTol)const
{
	bool stat = false;

	Point3D firstPoint = GetPoint(u1);
	Point3D lastPoint = GetPoint(u2);

	double dist = firstPoint.SqrDistance(lastPoint);
	
	if (dist > CommonConstants::ZERO * CommonConstants::ZERO)
	{
		Segment3D tmpSeg(firstPoint, lastPoint);
		
		int firstP = (int)u1;
		int endP = (int)u2;

		int numPoints = endP - firstP;
		if(numPoints)
		{
			double sum = 0;
			for(int i = (firstP+1); i <= endP; i++)
			{
				Point3D tmpPoint = m_points[i];
				sum += tmpSeg.GetSquareDistance(tmpPoint);
			}
			double rmsVal = sum / numPoints;
			if(rmsVal <= smoothenTol)
				stat = true;
		}
		else
			stat = true;
	}
	return stat;
}

//-----------------------------------------------------------------------------

Point3D Polyline3D::GetPoint(double uVal)const
{
	int u = int(uVal);
	
	Point3D reqPoint;
	if (u == m_points.size() - 1)
	{
		reqPoint = m_points[m_points.size() - 1];
	}
	else
	{
		double val = uVal - u;

		Point3D p1 = m_points[u];
		Point3D p2 = m_points[u+1];

		Segment3D tmpSeg(p1, p2);
		reqPoint = tmpSeg.GetPoint(val);
	}
	return reqPoint;
}

//-----------------------------------------------------------------------------

void Polyline3D::Smoothen(double smoothenTol, std::vector<size_t> &indices)const
{
	std::vector<SegmentUvals> tmpSegments, finalSegments;

	SegmentUvals seg;
	seg.m_u1 = 0;
	seg.m_u2 = (int) (m_points.size()) - 1;

	tmpSegments.push_back(seg);

	while (tmpSegments.size())
	{
		SegmentUvals topSeg = tmpSegments[tmpSegments.size()-1];
		tmpSegments.pop_back();

		if(checkSegment(topSeg.m_u1, topSeg.m_u2, smoothenTol))
			finalSegments.push_back(topSeg);		
		else
		{
			double val = (topSeg.m_u1 + topSeg.m_u2)/2.;

			//adjust size_t val on one of size_t point of size_t polyline
			val -= CommonConstants::ZERO;
			int midUval = (int)ceil(val);

			if (midUval == m_points.size() - 1)
				midUval--;

			SegmentUvals s1, s2;

			s1.m_u1 = topSeg.m_u1;
			s1.m_u2 = midUval;

			s2.m_u1 = midUval;
			s2.m_u2 = topSeg.m_u2;

			tmpSegments.push_back(s2);
			tmpSegments.push_back(s1);
		}
	}
	for(int i = 0; i < finalSegments.size(); i++)
	{
		indices.push_back((size_t)(finalSegments[i].m_u1));

		if(i == finalSegments.size()-1)
			indices.push_back((size_t)(finalSegments[i].m_u2));
	}
}

//-----------------------------------------------------------------------------

Segment3D Polyline3D::GetSegment(size_t index)const
{
	assert(index < m_points.size());

	return Segment3D(m_points[index], m_points[index+1]);
}

//-----------------------------------------------------------------------------

}