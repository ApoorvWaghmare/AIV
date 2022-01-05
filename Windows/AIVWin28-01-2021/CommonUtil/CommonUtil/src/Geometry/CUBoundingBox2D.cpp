// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <float.h>

//util include files
#include "CUReport.h"

//math include files
#include "CUBoundingBox2D.h"
#include "CUCommonConstants.h"

namespace CommonUtil
{
 
//-----------------------------------------------------------------------------

BoundingBox2D::BoundingBox2D()
{
	m_minX = m_minY = DBL_MAX;
	m_maxX = m_maxY = -DBL_MAX;
}

//-----------------------------------------------------------------------------

BoundingBox2D::BoundingBox2D(const std::vector<const Point2D *> &points,
							 double expansionTol)
{
	//initialised for lint
	double minX = DBL_MAX, minY = DBL_MAX, maxX = -DBL_MAX, maxY = -DBL_MAX;
	size_t size = points.size();

	if(size)
	{
		minX = maxX = points[0]->GetX();
		minY = maxY = points[0]->GetY();

		for(int i = 1; i < size; i++)
		{
			double x = points[i]->GetX();
			double y = points[i]->GetY();

			if(x < minX)
			{
				minX = x;
			}
			if(y < minY)
			{
				minY = y;
			}
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
		}
	}

	BoundingBox2D::SetAllRanges(minX, maxX, minY, maxY, expansionTol);
}

//-----------------------------------------------------------------------------

BoundingBox2D::BoundingBox2D(const std::vector<Point2D> &points,
							 double expansionTol)
{
	//initialised for lint
	double minX = DBL_MAX, minY = DBL_MAX, maxX = -DBL_MAX, maxY = -DBL_MAX;
	
	size_t size = points.size();

	if(size)
	{
		minX = maxX = points[0].GetX();
		minY = maxY = points[0].GetY();

		for(int i = 1; i < size; i++)
		{
			double x = points[i].GetX();
			double y = points[i].GetY();

			if(x < minX)
			{
				minX = x;
			}
			if(y < minY)
			{
				minY = y;
			}
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
		}
	}

	BoundingBox2D::SetAllRanges(minX, maxX, minY, maxY, expansionTol);
}

//-----------------------------------------------------------------------------

BoundingBox2D::BoundingBox2D(double minX, double maxX, double minY,
							 double maxY, double expansionTol)
{
	SetAllRanges(minX, maxX, minY, maxY, expansionTol);
}


//-----------------------------------------------------------------------------

Point2D BoundingBox2D::GetCenter()const
{
	double xCenter = (m_minX + m_maxX) / 2;
	double yCenter = (m_minY + m_maxY) / 2;

	return Point2D(xCenter, yCenter);
}

//-----------------------------------------------------------------------------

void BoundingBox2D::Unite(const BoundingBox2D &box)
{
	double boxMinX = DBL_MAX, boxMaxX = -DBL_MAX;
	double boxMinY = DBL_MAX, boxMaxY = -DBL_MAX;

	box.GetAllRanges(boxMinX, boxMaxX, boxMinY, boxMaxY);
	
	//Sets the min limits
	if(boxMinX < m_minX)
		m_minX = boxMinX;
	if(boxMinY < m_minY)
		m_minY = boxMinY;

	//Sets the max limits
	if(m_maxX < boxMaxX)
		m_maxX = boxMaxX;
	if(m_maxY < boxMaxY)
		m_maxY = boxMaxY;
}

//-----------------------------------------------------------------------------

void BoundingBox2D::GetAllRanges(double& minX, double& maxX,
							   double& minY, double& maxY)const
{
	minX = m_minX;
	maxX = m_maxX;

	minY = m_minY;
	maxY = m_maxY;
}

//-----------------------------------------------------------------------------

void BoundingBox2D::SetAllRanges(double minX,double maxX,
							   double minY, double maxY, double expansionTol)
{
	m_minX = minX - expansionTol;
	m_maxX = maxX + expansionTol;

	m_minY = minY - expansionTol;
	m_maxY = maxY + expansionTol;
}

//-----------------------------------------------------------------------------

void BoundingBox2D::ExpandBoundingBox(double value)
{
	m_minX -= value;
	m_maxX += value;

	m_minY -= value;
	m_maxY += value;
}

//-----------------------------------------------------------------------------

bool BoundingBox2D::IsHavingOverlap(const BoundingBox2D &otherBox,
									double expansionTol,
									bool *coincidentBoxes)const
{
	bool status = false;
	
	if(coincidentBoxes)
		*coincidentBoxes = false;

	if( (m_minX - expansionTol <= otherBox.m_minX &&
		otherBox.m_minX <= m_maxX + expansionTol)
		||
		(m_minX - expansionTol <= otherBox.m_maxX &&
		otherBox.m_maxX <= m_maxX + expansionTol) )
	{
		if( (m_minY - expansionTol <= otherBox.m_minY &&
			otherBox.m_minY <= m_maxY + expansionTol)
			||
			(m_minY - expansionTol <= otherBox.m_maxY &&
			otherBox.m_maxY <= m_maxY + expansionTol))
		{
			status = true;
		}
	}

	if(!status)
	{
		if( (otherBox.m_minX - expansionTol <= m_minX && 
			m_minX <= otherBox.m_maxX + expansionTol)
			||
			(otherBox.m_minX - expansionTol <= m_maxX &&
			m_maxX <= otherBox.m_maxX + expansionTol) )
		{
			if( (otherBox.m_minY - expansionTol <= m_minY &&
				m_minY <= otherBox.m_maxY + expansionTol)
				||
				(otherBox.m_minY - expansionTol <= m_maxY &&
				m_maxY <= otherBox.m_maxY + expansionTol) )
			{
				status = true;					
			}
		}
	}  

	if( (m_minX - expansionTol >= otherBox.m_minX &&
		m_maxX - expansionTol <= otherBox.m_maxX)
		&&
		(m_minY - expansionTol <= otherBox.m_minY &&
		m_maxY + expansionTol >= otherBox.m_maxY ) )
		status = true;

/*	if( (m_minX - expansionTol <= otherBox.m_minX &&
		m_maxX - expansionTol >= otherBox.m_maxX)
		&&
		(m_minY - expansionTol >= otherBox.m_minY &&
		m_maxY + expansionTol <= m_maxY ) )
		status = true;*/

	if(!status)
	{
		if(fabs(m_minX - otherBox.m_minX) < CommonConstants::PNT_TOL &&
		   fabs(m_maxX - otherBox.m_maxX) < CommonConstants::PNT_TOL )
		{
			if(fabs(m_minY - otherBox.m_minY) < CommonConstants::PNT_TOL &&
			   fabs(m_maxY - otherBox.m_maxY) < CommonConstants::PNT_TOL )
			{
				status = true;
				if(coincidentBoxes)
					*coincidentBoxes = true;
			}
		}

	}
	return status;
}

//-----------------------------------------------------------------------------

void BoundingBox2D::Dump(Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);
	
	report.Add(tabIndent, "BoundingBox2D\n");
	report.Add(tabIndent, "MinX, MaxX : ");
	report.Add(tabIndent, "MinY, MaxY : ");
	
	report.Add(0, "%15.6f, %15.6f\n", m_minX, m_maxX);
	report.Add(0, "%15.6f, %15.6f\n", m_minY, m_maxY);

	report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

void BoundingBox2D::GetPoints(const std::vector<Point2D> &points,
							  Point2D &minXPoint, Point2D &minYPoint, 
							  Point2D &maxXPoint, Point2D &maxYPoint)const
{
	double minX, minY, maxX, maxY;
	size_t size = points.size();

	minX = maxX = points[0].GetX();
	minY = maxY = points[0].GetY();

	for(int i = 1; i < size; i++)
	{
		double x = points[i].GetX();
		double y = points[i].GetY();

		if(x < minX)
		{
			minX = x;
			minXPoint = points[i];
		}
		if(y < minY)
		{
			minY = y;
			minYPoint = points[i];
		}
		if (x > maxX)
		{
			maxX = x;
			maxXPoint = points[i];
		}
		if (y > maxY)
		{
			maxY = y;
			maxYPoint = points[i];
		}
	}

}

//-----------------------------------------------------------------------------

bool BoundingBox2D::IsInside(const Point2D &point)const
{
	bool stat = false;
	double x = point.GetX();
	double y = point.GetY();
	if((m_minX - CommonConstants::PNT_TOL < x  && x < m_maxX + CommonConstants::PNT_TOL) && 
		(m_minY - CommonConstants::PNT_TOL < y  && y  < m_maxY + CommonConstants::PNT_TOL))
		stat = true;
	return stat;
}

//-----------------------------------------------------------------------------
bool BoundingBox2D::IsInside(const BoundingBox2D &otherBox)const
{
	bool status = false;
	if( (otherBox.m_minX <= m_minX && m_minX <= otherBox.m_maxX) &&
		(otherBox.m_minX <= m_maxX && m_maxX <= otherBox.m_maxX) )
	{
		if( (otherBox.m_minY <= m_minY && m_minY <= otherBox.m_maxY ) &&
			(otherBox.m_minY <= m_maxY && m_maxY <= otherBox.m_maxY ) )
		{
			status = true;
		}
	}
	return status;
}
//-----------------------------------------------------------------------------

bool BoundingBox2D::IsIntersecting(const BoundingBox2D &otherBox, double tol/*CommonConstants::ZERO*/)const
{
	// Check if one condition of following four is true
	//		Check minX of other box in between x range of this box
	//		Check maxX of other box in between x range of this box
	//		Check minX of this box in between x range of other box
	//		Check maxX of this box in between x range of other box
	// Check if one condition of above four is true for y-direction
	if ((m_minX - tol <= otherBox.m_minX && otherBox.m_minX <= m_maxX + tol) ||
		(m_minX - tol <= otherBox.m_maxX && otherBox.m_maxX <= m_maxX + tol) ||
		(otherBox.m_minX - tol <= m_minX && m_minX <= otherBox.m_maxX + tol) ||
		(otherBox.m_minX - tol <= m_maxX && m_maxX <= otherBox.m_maxX + tol))
	{
		if ((m_minY - tol <= otherBox.m_minY && otherBox.m_minY <= m_maxY + tol) ||
			(m_minY - tol <= otherBox.m_maxY && otherBox.m_maxY <= m_maxY + tol) ||
			(otherBox.m_minY - tol <= m_minY && m_minY <= otherBox.m_maxY + tol) ||
			(otherBox.m_minY - tol <= m_maxY && m_maxY <= otherBox.m_maxY + tol))
		{
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------
}