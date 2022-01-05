// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <float.h>
#include <assert.h>

//math include files
#include "CUBoundingBox3d.h"

//util include files
#include "CUReport.h"
#include "CUPoint3DF.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

BoundingBox3D::BoundingBox3D()
{
	m_minX = m_minY = m_minZ = DBL_MAX;
	m_maxX = m_maxY = m_maxZ = -DBL_MAX;
}

//-----------------------------------------------------------------------------

BoundingBox3D::BoundingBox3D(const std::vector<const Point3D *> &points,
							 double expansionTol)
{
	double minX, minY, maxX, maxY, minZ, maxZ ;
	size_t size = points.size();

	minX = maxX = points[0]->GetX();
	minY = maxY = points[0]->GetY();
	minZ = maxZ = points[0]->GetZ();

	for(int i = 1; i < size; i++)
	{
		double x = points[i]->GetX();
		double y = points[i]->GetY();
		double z = points[i]->GetZ();

		if(x < minX)
		{
			minX = x;
		}
		if(y < minY)
		{
			minY = y;
		}
		if(z < minZ)
		{
			minZ = z;
		}
		if (x > maxX)
		{
			maxX = x;
		}
		if (y > maxY)
		{
			maxY = y;
		}
		if (z > maxZ)
		{
			maxZ = z;
		}

	}

	BoundingBox3D::SetAllRanges(minX, maxX, minY, maxY, minZ,
								maxZ, expansionTol);
}

//-----------------------------------------------------------------------------

BoundingBox3D::BoundingBox3D(const std::vector<CommonUtil::Point3D> &points,
							 double expansionTol)
{
	double minX, minY, maxX, maxY, minZ, maxZ ;
	size_t size = points.size();

	minX = maxX = points[0].GetX();
	minY = maxY = points[0].GetY();
	minZ = maxZ = points[0].GetZ();

	for(int i = 1; i < size; i++)
	{
		double x = points[i].GetX();
		double y = points[i].GetY();
		double z = points[i].GetZ();

		if(x < minX)
		{
			minX = x;
		}
		if(y < minY)
		{
			minY = y;
		}
		if(z < minZ)
		{
			minZ = z;
		}
		if (x > maxX)
		{
			maxX = x;
		}
		if (y > maxY)
		{
			maxY = y;
		}
		if (z > maxZ)
		{
			maxZ = z;
		}

	}

	BoundingBox3D::SetAllRanges(minX, maxX, minY, maxY, minZ,
								maxZ, expansionTol);

}

//-----------------------------------------------------------------------------

BoundingBox3D::BoundingBox3D(double minX, double maxX, double minY, double maxY,
							 double minZ, double maxZ, double expansionTol)
{
	m_minX = minX - expansionTol;
	m_maxX = maxX + expansionTol;

	m_minY = minY - expansionTol;
	m_maxY = maxY + expansionTol;

	m_minZ = minZ - expansionTol;
	m_maxZ = maxZ + expansionTol;
}

//-----------------------------------------------------------------------------

BoundingBox3D::BoundingBox3D(const std::vector<double> &points)
{
	m_minX = DBL_MAX;
	m_minY = DBL_MAX;
	m_minZ = DBL_MAX;
	m_maxX = -DBL_MAX;
	m_maxY = -DBL_MAX;
	m_maxZ = -DBL_MAX;
	double xVal = .0;
	double yVal = .0;
	double zVal = .0;
	size_t totalNoPoints = points.size();
	if (totalNoPoints % 3 != 0)
		assert("0");
	for (int ptCount = 0; ptCount < totalNoPoints; ptCount = ptCount + 3)
	{
		xVal = points[ptCount];
		yVal = points[ptCount + 1];
		zVal = points[ptCount + 2];

		if (xVal < m_minX)
			m_minX = xVal;

		if (xVal > m_maxX)
			m_maxX = xVal;

		if (yVal < m_minY)
			m_minY = yVal;

		if (yVal > m_maxY)
			m_maxY = yVal;

		if (zVal < m_minZ)
			m_minZ = zVal;

		if (zVal > m_maxZ)
			m_maxZ = zVal;
	}
}

//-----------------------------------------------------------------------------

BoundingBox3D::BoundingBox3D(const std::vector<float> &points)
{
	m_minX = DBL_MAX;
	m_minY = DBL_MAX;
	m_minZ = DBL_MAX;
	m_maxX = -DBL_MAX;
	m_maxY = -DBL_MAX;
	m_maxZ = -DBL_MAX;
	float xVal = .0;
	float yVal = .0;
	float zVal = .0;
	size_t totalNoPoints = points.size();
	if (totalNoPoints % 3 != 0)
		assert("0");
	for (int ptCount = 0; ptCount < totalNoPoints; ptCount = ptCount + 3)
	{
		xVal = points[ptCount];
		yVal = points[ptCount + 1];
		zVal = points[ptCount + 2];

		if (xVal < m_minX)
			m_minX = xVal;

		if (xVal > m_maxX)
			m_maxX = xVal;

		if (yVal < m_minY)
			m_minY = yVal;

		if (yVal > m_maxY)
			m_maxY = yVal;

		if (zVal < m_minZ)
			m_minZ = zVal;

		if (zVal > m_maxZ)
			m_maxZ = zVal;
	}
}

//-----------------------------------------------------------------------------

Point3D BoundingBox3D::GetCenter() const
{
	double xCenter = (m_minX + m_maxX) / 2;
	double yCenter = (m_minY + m_maxY) / 2;
	double zCenter = (m_minZ + m_maxZ) / 2;

	return Point3D(xCenter, yCenter, zCenter);
}

//-----------------------------------------------------------------------------

void BoundingBox3D::Unite(const BoundingBox3D &box)
{
	double boxMinX = DBL_MAX, boxMaxX = -DBL_MAX;
	double boxMinY = DBL_MAX, boxMaxY = -DBL_MAX;
	double boxMinZ = DBL_MAX, boxMaxZ = -DBL_MAX;

	box.GetAllRanges(boxMinX, boxMaxX, boxMinY, boxMaxY, boxMinZ, boxMaxZ);

	//Sets the min limits
	if(boxMinX < m_minX)
		m_minX = boxMinX;
	if(boxMinY < m_minY)
		m_minY = boxMinY;
	if(boxMinZ < m_minZ)
		m_minZ = boxMinZ;

	//Sets the max limits
	if(m_maxX < boxMaxX)
		m_maxX = boxMaxX;
	if(m_maxY < boxMaxY)
		m_maxY = boxMaxY;
	if(m_maxZ < boxMaxZ)
		m_maxZ = boxMaxZ;
}

//-----------------------------------------------------------------------------

void BoundingBox3D::GetAllRanges(double& minX, double& maxX,
							     double& minY, double& maxY,
								 double& minZ, double& maxZ)const
{
	minX = m_minX;
	maxX = m_maxX;

	minY = m_minY;
	maxY = m_maxY;

	minZ = m_minZ;
	maxZ = m_maxZ;

}

//-----------------------------------------------------------------------------

void BoundingBox3D::SetAllRanges(double minX,double maxX,
							     double minY, double maxY,
								 double minZ, double maxZ, double expansionTol)
{
	m_minX = minX - expansionTol;
	m_maxX = maxX + expansionTol;

	m_minY = minY - expansionTol;
	m_maxY = maxY + expansionTol;

	m_minZ = minZ - expansionTol;
	m_maxZ = maxZ + expansionTol;

}

//-----------------------------------------------------------------------------

void BoundingBox3D::ExpandBoundingBox(double value)
{
	m_minX -= value;
	m_maxX += value;

	m_minY -= value;
	m_maxY += value;

	m_minZ -= value;
	m_maxZ += value;

}

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsHavingOverlap(const BoundingBox3D &otherBox,
									double expansionTol,
									bool *coincidentBoxes)const
{
	bool status = false;
	if(coincidentBoxes)
		*coincidentBoxes = false;

	if( (m_minX - expansionTol < otherBox.m_minX &&
		otherBox.m_minX < m_maxX + expansionTol)
		||
		(m_minX - expansionTol < otherBox.m_maxX &&
		otherBox.m_maxX< m_maxX + expansionTol) )
	{
		if( (m_minY - expansionTol < otherBox.m_minY &&
			otherBox.m_minY < m_maxY + expansionTol)
			||
			(m_minY - expansionTol < otherBox.m_maxY &&
			otherBox.m_maxY < m_maxY + expansionTol) )
		{
			if( (m_minZ - expansionTol < otherBox.m_minZ &&
			otherBox.m_minZ < m_maxZ + expansionTol)
			||
			(m_minZ - expansionTol < otherBox.m_maxZ &&
			otherBox.m_maxZ < m_maxZ + expansionTol) )
			{
				status = true;
			}
		}
	}

	if(!status)
	{
		if( (otherBox.m_minX - expansionTol < m_minX &&
			m_minX < otherBox.m_maxX + expansionTol)
			||
			(otherBox.m_minX - expansionTol < m_maxX &&
			m_maxX < otherBox.m_maxX + expansionTol) )
		{
			if( (otherBox.m_minY - expansionTol < m_minY &&
				m_minY < otherBox.m_maxY + expansionTol)
				||
				(otherBox.m_minY - expansionTol < m_maxY &&
				m_maxY < otherBox.m_maxY + expansionTol) )
			{
				if( (otherBox.m_minZ - expansionTol < m_minZ &&
				m_minZ < otherBox.m_maxZ + expansionTol)
				||
				(otherBox.m_minZ - expansionTol < m_maxZ &&
				m_maxZ < otherBox.m_maxZ + expansionTol) )
				{
					status = true;
				}
			}

		}
	}

	if(!status)
	{
		if(fabs(m_minX - otherBox.m_minX) < CommonConstants::PNT_TOL &&
		   fabs(m_maxX - otherBox.m_maxX) < CommonConstants::PNT_TOL)
		{
			if(fabs(m_minY - otherBox.m_minY) < CommonConstants::PNT_TOL &&
			   fabs(m_maxY - otherBox.m_maxY) < CommonConstants::PNT_TOL)
			{
				if(fabs(m_minZ - otherBox.m_minZ) < CommonConstants::PNT_TOL&&
				   fabs(m_maxZ - otherBox.m_maxZ) < CommonConstants::PNT_TOL)
				{
					status = true;
					if(coincidentBoxes)
						*coincidentBoxes = true;
				}
			}
		}

	}

	return status;
}

//-----------------------------------------------------------------------------

void BoundingBox3D::Expand(double expansionTolerance)
{
	m_minX -= expansionTolerance;
	m_maxX += expansionTolerance;

	m_minY -= expansionTolerance;
	m_maxY += expansionTolerance;

	m_minZ -= expansionTolerance;
	m_maxZ += expansionTolerance;
}

//-----------------------------------------------------------------------------
/*
bool BoundingBox3D::IsPointInBox(const Point3D &point)const
{
	bool stat = false;
	double x = point.GetX();
	double y = point.GetX();
	double z = point.GetX();
	if((m_minX < x && x < m_maxX) && (m_minY < y && y < m_maxY) &&
													(m_minZ < z && z < m_maxZ))
		stat = true;
	return stat;
}*/

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsPointInBox(const Point3D &point)const
{
	bool stat = false;
	double x = point.GetX();
	double y = point.GetY();
	double z = point.GetZ();

	if((m_minX <= x && x <= m_maxX) && (m_minY <= y && y <= m_maxY) &&
													(m_minZ <= z && z <= m_maxZ))
		stat = true;
	return stat;
}

//----------------------------------------------------------------------------

bool BoundingBox3D::IsIntersecting(const BoundingBox3D &otherBox)const
{
#if 0 // new defination
	if ((otherBox.m_minX > m_maxX || otherBox.m_maxX < m_minX) ||
		(otherBox.m_minY > m_maxY || otherBox.m_maxY < m_minY) ||
		(otherBox.m_minZ > m_maxZ || otherBox.m_maxZ < m_minZ))
		return false;

	return true;
#endif

#if 1 // Old definition from essence
	bool status = false;

	if ((m_minX <= otherBox.m_minX && otherBox.m_minX <= m_maxX) ||
		(m_minX <= otherBox.m_maxX && otherBox.m_maxX <= m_maxX) ||
		(otherBox.m_minX <= m_minX && m_minX <= otherBox.m_maxX) ||
		(otherBox.m_minX <= m_maxX && m_maxX <= otherBox.m_maxX))
	{
		if ((m_minY <= otherBox.m_minY && otherBox.m_minY <= m_maxY) ||
			(m_minY <= otherBox.m_maxY && otherBox.m_maxY <= m_maxY) ||
			(otherBox.m_minY <= m_minY && m_minY <= otherBox.m_maxY) ||
			(otherBox.m_minY <= m_maxY && m_maxY <= otherBox.m_maxY))
		{
			if ((m_minZ <= otherBox.m_minZ && otherBox.m_minZ <= m_maxZ) ||
				(m_minZ <= otherBox.m_maxZ && otherBox.m_maxZ <= m_maxZ) ||
				(otherBox.m_minZ <= m_minZ && m_minZ <= otherBox.m_maxZ) ||
				(otherBox.m_minZ <= m_maxZ && m_maxZ <= otherBox.m_maxZ))
			{
				status = true;
			}
		}
	}

	if (!status)
	{
		if (fabs(m_minX - otherBox.m_minX) <= CommonConstants::PNT_TOL &&
			fabs(m_maxX - otherBox.m_maxX) <= CommonConstants::PNT_TOL)
		{
			if (fabs(m_minY - otherBox.m_minY) <= CommonConstants::PNT_TOL &&
				fabs(m_maxY - otherBox.m_maxY) <= CommonConstants::PNT_TOL)
			{
				if (fabs(m_minZ - otherBox.m_minZ) <= CommonConstants::PNT_TOL &&
					fabs(m_maxZ - otherBox.m_maxZ) <= CommonConstants::PNT_TOL)
				{
					status = true;
				}
			}
		}
	}
	return status;
#endif
}

//-----------------------------------------------------------------------------

void BoundingBox3D::Dump(Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);

	report.Add(tabIndent, "BoundingBox3D\n");
	report.Add(tabIndent, "MinX, MaxX : ");
	report.Add(tabIndent, "MinY, MaxY : ");
	report.Add(tabIndent, "MinZ, MaxZ : ");
	report.Add(0, "%15.6f, %15.6f\n", m_minX, m_maxX);
	report.Add(0, "%15.6f, %15.6f\n", m_minY, m_maxY);
	report.Add(0, "%15.6f, %15.6f\n", m_minZ, m_maxZ);

	report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

void BoundingBox3D::GetCornerPoints(std::vector<CommonUtil::Point3D> &points)const
{
	points.resize(8);
	points[0].Set(m_minX, m_minY, m_minZ);
	points[1].Set(m_minX, m_maxY, m_minZ);
	points[2].Set(m_maxX, m_minY, m_minZ);
	points[3].Set(m_maxX, m_maxY, m_minZ);

	points[4].Set(m_minX, m_minY, m_maxZ);
	points[5].Set(m_minX, m_maxY, m_maxZ);
	points[6].Set(m_maxX, m_minY, m_maxZ);
	points[7].Set(m_maxX, m_maxY, m_maxZ);
}

//-----------------------------------------------------------------------------

double BoundingBox3D::GetDiagonalLength()const
{
	Point3D p1(m_minX, m_minY, m_minZ);
	Point3D p2(m_maxX, m_maxY, m_maxZ);
	double dist = p1.Distance(p2);
	return dist;
}

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsInside(const Point3D* point)const
{
	bool stat = false;

	if ((point->GetX() > m_minX && point->GetX() < m_maxX) &&
		(point->GetY() > m_minY && point->GetY() < m_maxY) &&
		(point->GetZ() > m_minZ && point->GetZ() < m_maxZ))
		stat = true;

	return stat;
}

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsInside(const Point3DF* point)const
{
	Point3D tempPoint(*(CommonUtil::Point3D*)point);
	return IsInside(&tempPoint);
}

//-----------------------------------------------------------------------------

void BoundingBox3D::GetCornerPoints(double cornerPoints[24])const
{
	cornerPoints[0] = m_minX;
	cornerPoints[1] = m_minY;
	cornerPoints[2] = m_minZ;

	cornerPoints[3] = m_maxX;
	cornerPoints[4] = m_minY;
	cornerPoints[5] = m_minZ;

	cornerPoints[6] = m_maxX;
	cornerPoints[7] = m_maxY;
	cornerPoints[8] = m_minZ;

	cornerPoints[9] = m_minX;
	cornerPoints[10] = m_maxY;
	cornerPoints[11] = m_minZ;

	cornerPoints[12] = m_minX;
	cornerPoints[13] = m_minY;
	cornerPoints[14] = m_maxZ;

	cornerPoints[15] = m_maxX;
	cornerPoints[16] = m_minY;
	cornerPoints[17] = m_maxZ;

	cornerPoints[18] = m_maxX;
	cornerPoints[19] = m_maxY;
	cornerPoints[20] = m_maxZ;

	cornerPoints[21] = m_minX;
	cornerPoints[22] = m_maxY;
	cornerPoints[23] = m_maxZ;
}

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsPointOnWall(const Point3D* point, double tolerance)const
{
	if (!point)
		return false;

	double x = point->GetX();
	double y = point->GetY();
	double z = point->GetZ();

	if (fabs(x - m_minX) < tolerance || fabs(x - m_maxX) < tolerance)
	{
		if ((fabs(y - m_minY) < tolerance || y > m_minY) && (fabs(y - m_maxY) < tolerance || y < m_maxY) &&
			(fabs(z - m_minZ) < tolerance || z > m_minZ) && (fabs(z - m_maxZ) < tolerance || z < m_maxZ))
			return true;
	}
	else if (fabs(y - m_minY) < tolerance || fabs(y - m_maxY) < tolerance)
	{
		if ((fabs(x - m_minX) < tolerance || x > m_minX) && (fabs(x - m_maxX) < tolerance || x < m_maxX) &&
			(fabs(z - m_minZ) < tolerance || z > m_minZ) && (fabs(z - m_maxZ) < tolerance || z < m_maxZ))
			return true;
	}
	else if (fabs(z - m_minZ) < tolerance || fabs(z - m_maxZ) < tolerance)
	{
		if ((fabs(x - m_minX) < tolerance || x > m_minX) && (fabs(x - m_maxX) < tolerance || x < m_maxX) &&
			(fabs(y - m_minY) < tolerance || y > m_minY) && (fabs(y - m_maxY) < tolerance || y < m_maxY))
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsPointOnWall(const Point3DF* point, double tolerance)const
{
	Point3D tempPoint(*(CommonUtil::Point3D*)point);
	return IsPointOnWall(&tempPoint, tolerance);
}

//-----------------------------------------------------------------------------

bool BoundingBox3D::IsInside(const BoundingBox3D &otherBox)const
{
	if ((otherBox.m_minX > m_minX && otherBox.m_maxX < m_maxX) &&
		(otherBox.m_minY > m_minY && otherBox.m_maxY < m_maxY) &&
		(otherBox.m_minZ > m_minZ && otherBox.m_maxZ < m_maxZ))
		return true;

	return false;
}

//-----------------------------------------------------------------------------

Point3DF BoundingBox3D::GetCenterPoint3DF()
{
	float x = (float)((m_minX + m_maxX) / 2.0);
	float y = (float)((m_minY + m_maxY) / 2.0);
	float z = (float)((m_minZ + m_maxZ) / 2.0);

	Point3DF p(x, y, z);
	return p;
}

//-----------------------------------------------------------------------------

BoundingBox3D* BoundingBox3D::MergeBoxes(BoundingBox3D &box)
{
	double minX, minY, minZ, maxX, maxY, maxZ;

	if (m_maxX > box.m_maxX)
		maxX = m_maxX;
	else
		maxX = box.m_maxX;

	if (m_maxY > box.m_maxY)
		maxY = m_maxY;
	else
		maxY = box.m_maxY;

	if (m_maxZ > box.m_maxZ)
		maxZ = m_maxZ;
	else
		maxZ = box.m_maxZ;

	if (m_minX < box.m_minX)
		minX = m_minX;
	else
		minX = box.m_minX;

	if (m_minY < box.m_minY)
		minY = m_minY;
	else
		minY = box.m_minY;

	if (m_minZ < box.m_minZ)
		minZ = m_minZ;
	else
		minZ = box.m_minZ;

	BoundingBox3D *box1 = new BoundingBox3D(minX, minY, minZ, maxX, maxY, maxZ);
	return box1;
}

//-----------------------------------------------------------------------------
}
