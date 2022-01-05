// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#include "CUSecureUtil.h"

//standard include files
#include <float.h>

// math include files
#include "CUSegment2DIntersector.h"
#include "CUSegment2D.h"
								
namespace CommonUtil
{

//-----------------------------------------------------------------------------

Segment2DIntersector::Segment2DIntersector(CommonUtil::Segment2D* segment,
										   double snappingTol)
{
	m_testSegment = 0;
	m_segment = segment;
	m_isIntersecting = false;
	m_isParallel = false;
	m_connectedEndtoEnd = false;
	m_isOverlap = false;
	m_colinear = false;
	m_snappingTol = snappingTol;
	m_uValOrgSeg = DBL_MAX;
	m_uValTestSeg = DBL_MAX;
	m_status = UNKNOWN;

}
	
//-----------------------------------------------------------------------------

bool Segment2DIntersector::Execute(CommonUtil::Segment2D* testSegment, double angleTol)
{
	m_testSegment = testSegment;

	CommonUtil::Vector::Direction direction;
	m_isParallel = checkParallel(direction, angleTol);

	if(!m_isParallel)
	{
		setIntersection();
	}	

	return m_isIntersecting;
}

//-----------------------------------------------------------------------------

void Segment2DIntersector::setIntersection()
{
	CommonUtil::Point2D tempPoint1 = m_segment->GetStartPoint();
	CommonUtil::Point2D tempPoint2 = m_segment->GetEndPoint();

	CommonUtil::Point2D tempPoint3 = m_testSegment->GetStartPoint();
	CommonUtil::Point2D tempPoint4 = m_testSegment->GetEndPoint();

	double x1 = tempPoint1.GetX();
	double y1 = tempPoint1.GetY();

	double x2 = tempPoint2.GetX();
	double y2 = tempPoint2.GetY();

	double x3 = tempPoint3.GetX();
	double y3 = tempPoint3.GetY();

	double x4 = tempPoint4.GetX();
	double y4 = tempPoint4.GetY();

	double numerator1 = (y4 - y3)*(x3 - x1) - (x4 - x3)*(y3-y1);
	double numerator2 = (y2 - y1)*(x3 - x1) - (x2 - x1)*(y3-y1);

	double denominator = (x3-x4)*(y2-y1) - (y3-y4)*(x2-x1);
	if(fabs(denominator) > CommonUtil::CommonConstants::ZERO)
	{
		m_uValOrgSeg = numerator1 / denominator;
		m_uValTestSeg = numerator2 / denominator;
	
		if(fabs(m_uValOrgSeg) < m_snappingTol)
			m_uValOrgSeg = 0.0;

		else if(fabs(m_uValOrgSeg-1.0) < m_snappingTol)
			m_uValOrgSeg = 1.0;
		
		if(fabs(m_uValTestSeg) < m_snappingTol)
			m_uValTestSeg = 0.0;

		else if(fabs(m_uValTestSeg-1.0) < m_snappingTol)
			m_uValTestSeg = 1.0;


		if(m_uValOrgSeg < 0.0 || m_uValOrgSeg > 1.0 ||
		   m_uValTestSeg < 0.0 || m_uValTestSeg > 1.0)
		{
			m_isIntersecting = false;
			m_status = NONINTERSECTING;
		}
		else
		{
			m_isIntersecting = true;
			m_status = INTERSECTING;
		}
	}
}

//-----------------------------------------------------------------------------

bool Segment2DIntersector::checkParallel(CommonUtil::Vector::Direction &direction,
										 double angleTol)
{
	bool parallel = false;
	CommonUtil::Point2D tempPoint1 = m_segment->GetStartPoint();
	CommonUtil::Point2D tempPoint2 = m_segment->GetEndPoint();

	CommonUtil::Point2D tempPoint3 = m_testSegment->GetStartPoint();
	CommonUtil::Point2D tempPoint4 = m_testSegment->GetEndPoint();

	CommonUtil::Vector orgSegVector(tempPoint1, tempPoint2);                 
	CommonUtil::Vector testSegVector(tempPoint3, tempPoint4);

	if(orgSegVector.IsParallel(testSegVector, direction, angleTol))
	{
		parallel = true;
		m_status = PARALLEL;
	}
	
	return parallel;
}

//-----------------------------------------------------------------------------

void Segment2DIntersector::SetEndtoEndConnected()
{
	if(m_uValOrgSeg == 0 && m_uValTestSeg == 0 || m_uValOrgSeg == 0 && m_uValTestSeg == 1
		 || m_uValOrgSeg == 1 && m_uValTestSeg == 0 || m_uValOrgSeg == 1 && m_uValTestSeg == 1)
		 m_connectedEndtoEnd = true;
}

//-----------------------------------------------------------------------------

void Segment2DIntersector::CheckOverlap(double overlappingTol, double &uVal1,
										double &vVal1, double &uVal2, 
										double &vVal2)
{
	CommonUtil::Segment2D* orgSegment = m_segment;
	CommonUtil::Segment2D* projSegment = m_testSegment;
	
	double tmpuVal1 = DBL_MAX, tmpuVal2 = DBL_MAX;
	for(int i = 0; i < 2; i++)
	{
		CommonUtil::Point2D testSegPoint1 = projSegment->GetStartPoint();
		orgSegment->Project(testSegPoint1, m_snappingTol, tmpuVal1);


		CommonUtil::Point2D orgSegPoint1 = orgSegment->GetPointOnCurve(tmpuVal1);

		double tmpDist1 = orgSegPoint1.Distance(testSegPoint1);
		if(tmpDist1 < overlappingTol)
		{
			m_isOverlap = true;

			if(tmpuVal1 < 0.0 || tmpuVal1 > 1.0)
				m_isOverlap = false;
		}

		CommonUtil::Point2D testSegPoint2 = projSegment->GetEndPoint();
		orgSegment->Project(testSegPoint2, m_snappingTol, tmpuVal2);
		
		CommonUtil::Point2D orgSegPoint2 = orgSegment->GetPointOnCurve(tmpuVal2);

		double tmpDist2 = orgSegPoint2.Distance(testSegPoint2);
		if(tmpDist2 < overlappingTol)
		{
			m_isOverlap = true;

			if(tmpuVal2 < 0.0 || tmpuVal2 > 1.0)
				m_isOverlap = false;
		}

		if(i == 0)
		{	
			projSegment = m_segment;
			orgSegment = m_testSegment;
			uVal1 = tmpuVal1;
			uVal2 = tmpuVal2;
		}
		else
		{
			vVal1 = tmpuVal1;
			vVal2 = tmpuVal2;
		}
	}
}

//-----------------------------------------------------------------------------

bool Segment2DIntersector::CheckColinear()
{
	CommonUtil::Point2D startPoint2 = m_testSegment->GetStartPoint();
	CommonUtil::Point2D endPoint2 = m_testSegment->GetEndPoint();

	double value1 = m_segment->CheckSign(startPoint2);
	double value2 = m_segment->CheckSign(endPoint2);

	if(value1 < CommonUtil::CommonConstants::ZERO && value2 < CommonUtil::CommonConstants::ZERO)
		m_colinear = true;
	
	CommonUtil::Point2D startPoint1 = m_segment->GetStartPoint();
	CommonUtil::Point2D endPoint1 = m_segment->GetEndPoint();

	double tmpDist1 = startPoint1.Distance(startPoint2);
	double tmpDist2 = startPoint1.Distance(endPoint2);
	double tmpDist3 = startPoint2.Distance(startPoint1);
	double tmpDist4 = startPoint2.Distance(endPoint1);

	if(tmpDist1 < CommonUtil::CommonConstants::PNT_TOL || tmpDist2 < CommonUtil::CommonConstants::PNT_TOL ||
	   tmpDist3 < CommonUtil::CommonConstants::PNT_TOL || tmpDist4 < CommonUtil::CommonConstants::PNT_TOL)
	{
		m_connectedEndtoEnd = true;
	}

	return m_colinear;
}

//-----------------------------------------------------------------------------

bool Segment2DIntersector::IsParallel()const
{
	return m_isParallel;
}

//-----------------------------------------------------------------------------

bool Segment2DIntersector::IsConnectedEndtoEnd()const
{
	return m_connectedEndtoEnd;
}

//-----------------------------------------------------------------------------

bool Segment2DIntersector::IsOverlap()const
{
	return m_isOverlap;
}

//-----------------------------------------------------------------------------

bool Segment2DIntersector::IsColinear()const
{
	return m_colinear;
}

//-----------------------------------------------------------------------------
void Segment2DIntersector::GetuValForTwoSegments(double &uValOrgSeg,
												 double &vValTestSeg)const
{
	if(m_status != UNKNOWN)
	{
		uValOrgSeg = m_uValOrgSeg;
		vValTestSeg = m_uValTestSeg;
	}
}

//-----------------------------------------------------------------------------

}