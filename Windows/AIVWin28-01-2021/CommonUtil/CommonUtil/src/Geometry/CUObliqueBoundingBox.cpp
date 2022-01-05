// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// standard include files
#include <float.h>
#include <xutility>

//geom include files
#include "CUObliqueBoundingBox.h"
#include "CUSegment3d.h"
#include "CUBoundingBox3D.h"
#include "CUBoundingBox2D.h"

//util include files
#include "CULUDecomposition.h"
#include "CUReport.h"

#include "CUVector.h"
#include "CUSqMatrix.h"

#include "CUGeometricalUtil.h"
#include "CUPlane.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

ObliqueBoundingBox::ObliqueBoundingBox(const Matrix &transformationMatrix, 
									  const std::vector<Point3D > &cornerPoints)
{
	m_transformationMatrix = transformationMatrix;
	computeInveseTransMatrix();
	m_cornerPoints = cornerPoints;
	setLocalPoints();
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::Set(const Matrix &transformationMatrix, 
		 const std::vector<CommonUtil::Point3D> &cornerPoints)
{
	m_transformationMatrix = transformationMatrix;
	computeInveseTransMatrix();
	m_cornerPoints = cornerPoints;
	setLocalPoints();
}

//-----------------------------------------------------------------------------

//Create box for given cylinder params
ObliqueBoundingBox::ObliqueBoundingBox(const CommonUtil::Point3D &cylBasePt, const CommonUtil::Vector &cylNormal, const double & cylRadius, const double & cylHeight)
{
	CommonUtil::Plane plane(cylBasePt, cylNormal);

	//Boiunding Box For Plne Creation 
	CommonUtil::Point3D tempPt;
	CommonUtil::Point3D projectedTempPt;
	CommonUtil::Vector xAxisGlob(1, 0, 0);
	CommonUtil::Vector::Direction directn;

	if (!cylNormal.IsParallel(xAxisGlob, directn, CommonUtil::CommonConstants::COLLINEARITY_TOL))
	{
		tempPt.Set(10, 0, 0);
		if (!plane.ProjectPoint(tempPt, projectedTempPt, 0.0))
		{
			tempPt.Set(0, 0, 0);
			plane.ProjectPoint(tempPt, projectedTempPt, 0.0);
		}
		else if (projectedTempPt.Distance(tempPt) <= CommonUtil::CommonConstants::PNT_TOL)
		{
			tempPt.Set(20, 0, 0);
			plane.ProjectPoint(tempPt, projectedTempPt, 0.0);
		}
	}
	else
	{
		tempPt.Set(0, 10, 0);
		if (!plane.ProjectPoint(tempPt, projectedTempPt, 0.0))
		{
			tempPt.Set(0, 0, 0);
			plane.ProjectPoint(tempPt, projectedTempPt, 0.0);
		}
		else if (projectedTempPt.Distance(tempPt) <= CommonUtil::CommonConstants::PNT_TOL)
		{
			tempPt.Set(0, 20, 0);
			plane.ProjectPoint(tempPt, projectedTempPt, 0.0);
		}
	}

	CommonUtil::Vector vect1InPlane(cylBasePt, projectedTempPt);
	vect1InPlane.Normalize();
	CommonUtil::Vector vect2InPlane;
	cylNormal.CrossProduct(vect1InPlane, vect2InPlane);
	vect2InPlane.Normalize();

	//Collect points on Cylinder
	std::vector<CommonUtil::Point3D> pointsForObbOfCylinder;
	pointsForObbOfCylinder.push_back(cylBasePt);
	double planeBoundary = cylRadius * 1.2;

	CommonUtil::Point3D planeBoundaryPoint1;
	vect1InPlane.GetPointAlongVector(cylBasePt, planeBoundaryPoint1, planeBoundary);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint1);

	vect1InPlane.Reverse();
	CommonUtil::Point3D planeBoundaryPoint2;
	vect1InPlane.GetPointAlongVector(cylBasePt, planeBoundaryPoint2, planeBoundary);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint2);

	CommonUtil::Point3D planeBoundaryPoint3;
	vect2InPlane.GetPointAlongVector(cylBasePt, planeBoundaryPoint3, planeBoundary);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint3);

	vect2InPlane.Reverse();
	CommonUtil::Point3D planeBoundaryPoint4;
	vect2InPlane.GetPointAlongVector(cylBasePt, planeBoundaryPoint4, planeBoundary);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint4);

	CommonUtil::Point3D planeBoundaryPoint5;
	cylNormal.GetPointAlongVector(cylBasePt, planeBoundaryPoint5, cylHeight);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint5);

	CommonUtil::Point3D planeBoundaryPoint6;
	cylNormal.GetPointAlongVector(planeBoundaryPoint1, planeBoundaryPoint6, cylHeight);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint6);

	CommonUtil::Point3D planeBoundaryPoint7;
	cylNormal.GetPointAlongVector(planeBoundaryPoint2, planeBoundaryPoint7, cylHeight);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint7);

	CommonUtil::Point3D planeBoundaryPoint8;
	cylNormal.GetPointAlongVector(planeBoundaryPoint3, planeBoundaryPoint8, cylHeight);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint8);

	CommonUtil::Point3D planeBoundaryPoint9;
	cylNormal.GetPointAlongVector(planeBoundaryPoint4, planeBoundaryPoint9, cylHeight);
	pointsForObbOfCylinder.push_back(planeBoundaryPoint9);


	Matrix transMatrix;
	std::vector<CommonUtil::Point3D> cornerPoints;
	createInput(pointsForObbOfCylinder, transMatrix, cornerPoints);

	m_transformationMatrix = transMatrix;
	computeInveseTransMatrix();
	m_cornerPoints = cornerPoints;
	setLocalPoints();
}

//-----------------------------------------------------------------------------

ObliqueBoundingBox::ObliqueBoundingBox(const std::vector<CommonUtil::Point3D> &points)
{
	Matrix transMatrix;
	std::vector<CommonUtil::Point3D> cornerPoints;
	createInput(points, transMatrix, cornerPoints);

	m_transformationMatrix = transMatrix;
	computeInveseTransMatrix();
	m_cornerPoints = cornerPoints;
	setLocalPoints();
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::setLocalPoints()
{
	size_t numPoints = m_cornerPoints.size();
	m_localPoints.resize(numPoints);
	for(int i = 0; i < numPoints; i++)
	{
		transformGlobalPointToLocalCoordinateSystem(m_cornerPoints[i], 
													m_localPoints[i]);
	}
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::GetGlobalCornerPoints(
									std::vector<CommonUtil::Point3D> &cornerPoints)const
{
	cornerPoints = m_cornerPoints;
}

//-----------------------------------------------------------------------------

bool ObliqueBoundingBox::IsPointInBox(const Point3D &point, double* pMaxDist_ )const
{
	bool stat = false;

	Point3D localPoint;
	transformGlobalPointToLocalCoordinateSystem(point, localPoint);

	double minX, maxX, minY, maxY, minZ, maxZ;
	getLocalExtents(minX, maxX, minY, maxY, minZ, maxZ);

	double x, y, z;
	x = localPoint.GetX();
	y = localPoint.GetY();
	z = localPoint.GetZ();

	if((minX-CommonConstants::ZERO < x  && x < maxX+CommonConstants::ZERO) && 
			(minY-CommonConstants::ZERO < y && y < maxY+CommonConstants::ZERO) && 
				(minZ-CommonConstants::ZERO < z && z < maxZ+CommonConstants::ZERO))
		stat = true;

    if( pMaxDist_ != NULL )
    {
        *pMaxDist_ = 0.0;
        double maxDistX = 0.0;
        double maxDistY = 0.0;
        double maxDistZ = 0.0;
        if( !stat )
        {
            if( x < minX )
            {
                maxDistX = minX-x;
            }
            else if( x > maxX )
            {
                maxDistX = x-maxX;
            }
            if( y < minY )
            {
                maxDistY = minY-y;
            }
            else if( y > maxY )
            {
                maxDistY = y-maxY;
            }
            if( z < minZ )
            {
                maxDistZ = minZ-z;
            }
            else if( z > maxZ )
            {
                maxDistZ = z-maxZ;
            }
            *pMaxDist_ = sqrt( maxDistX*maxDistX + 
                               maxDistY*maxDistY + 
                               maxDistZ*maxDistZ );
        }
    }
	return stat;
}

//-----------------------------------------------------------------------------

bool ObliqueBoundingBox::IsIntersecting(const ObliqueBoundingBox &box)const
{
	bool stat = false;

	std::vector<CommonUtil::Point3D> globalPoints;
	box.GetGlobalCornerPoints(globalPoints);

	//checks wether the corner points of the box are inside box
	std::vector<CommonUtil::Point3D> localPoints;
	for(int i = 0; i < globalPoints.size(); i++)
	{
		Point3D tmpPoint = globalPoints[i];
		
		stat = IsPointInBox(globalPoints[i]);
		if(stat)
			return true;

		tmpPoint.Transformation(m_inverseTransMatrix);
		localPoints.push_back(tmpPoint);
	}

	// reverse i.e whether this->GlobalPoints is inside other box
	globalPoints.clear();
	GetGlobalCornerPoints(globalPoints);
	//const Matrix& otherBoxInverseTransMatrix = box.GetInverseTransMatrix();
	for(int i = 0; i < globalPoints.size(); i++)
	{
		Point3D &tmpPoint = globalPoints[i];
		stat = box.IsPointInBox(tmpPoint);
		if(stat)
			return true;
	}


	//checks the given box edges intesrsecting with box or not
	std::vector<Segment3D> segments;
	box.getBoxEdges(localPoints,segments);

	size_t numSegments = segments.size();
	for(int j = 0; j < numSegments && !stat; j++)
	{
		Segment3D tmpSegment = segments[j];
		stat = isSegmentIntersecting(tmpSegment);
	}

	return stat;
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::getBoxEdges(const std::vector<CommonUtil::Point3D> &localPoints,
									 std::vector<Segment3D> &segments)const
{
	//first segment
	Segment3D tmpSEgment(localPoints[0], localPoints[1]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//second segment
	tmpSEgment.Set(localPoints[1], localPoints[3]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//third segment
	tmpSEgment.Set(localPoints[3], localPoints[2]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//fourth segment
	tmpSEgment.Set(localPoints[2], localPoints[0]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//fifth segment
	tmpSEgment.Set(localPoints[4], localPoints[5]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//sixth segment
	tmpSEgment.Set(localPoints[6], localPoints[7]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//seventh segment
	tmpSEgment.Set(localPoints[0], localPoints[4]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//eighth segment
	tmpSEgment.Set(localPoints[1], localPoints[5]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//ninth segment
	tmpSEgment.Set(localPoints[2], localPoints[6]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//tenth segment
	tmpSEgment.Set(localPoints[3], localPoints[7]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//eleventh segment
	tmpSEgment.Set(localPoints[5], localPoints[7]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);

	//twelveth segment
	tmpSEgment.Set(localPoints[4], localPoints[6]);
	if(tmpSEgment.GetLength() > CommonConstants::ZERO)
		segments.push_back(tmpSEgment);
}

//-----------------------------------------------------------------------------

bool ObliqueBoundingBox::isSegmentIntersecting(const Segment3D &segment)const
{
	bool stat = false;

	double minX, maxX, minY, maxY, minZ, maxZ;
	getLocalExtents(minX, maxX, minY, maxY, minZ, maxZ);

	BoundingBox3D box3d(minX, maxX, minY, maxY, minZ, maxZ);

	std::vector<Point2D > cornerPoints;
	Plane plane;
	double uVal;
	bool inPlane = false;
	//case XMIN:
	plane.SetBasePoint(minX, minY, minZ);
	plane.SetNormal(1,0,0);
	plane.Intersect(segment, 0.0, uVal, inPlane);
	if((uVal > CommonConstants::ZERO && uVal < 1 + CommonConstants::ZERO))
	{
		computeFaceCornerPts(box3d, XMIN, cornerPoints);
		BoundingBox2D box(cornerPoints, CommonConstants::ZERO);
		Point3D pt = segment.GetPoint(uVal);

		if(box.IsInside(Point2D(pt.GetY(), pt.GetZ())))
			return true;
	}

	//case XMAX:
	plane.SetBasePoint(maxX, minY, minZ);
	plane.Intersect(segment, 0.0, uVal, inPlane);
	if((uVal > -CommonConstants::ZERO && uVal < 1 + CommonConstants::ZERO))
	{
		computeFaceCornerPts(box3d, XMAX, cornerPoints);
		BoundingBox2D box(cornerPoints, CommonConstants::ZERO);
		Point3D pt = segment.GetPoint(uVal);

		if(box.IsInside(Point2D(pt.GetY(), pt.GetZ())))
			return true;
	}
	
	//case YMIN:
	plane.SetBasePoint(minX, minY, minZ);
	plane.SetNormal(0, 1, 0);
	plane.Intersect(segment, 0.0, uVal, inPlane);
	if((uVal > -CommonConstants::ZERO && uVal < 1 + CommonConstants::ZERO))
	{
		computeFaceCornerPts(box3d, YMIN, cornerPoints);
		BoundingBox2D box(cornerPoints, CommonConstants::ZERO);
		Point3D pt = segment.GetPoint(uVal);

		if(box.IsInside(Point2D(pt.GetX(), pt.GetZ())))
			return true;
	}
	
	//case YMAX:
	plane.SetBasePoint(minX, maxY, minZ);
	plane.Intersect(segment, 0.0, uVal, inPlane);
	if((uVal > -CommonConstants::ZERO && uVal < 1 + CommonConstants::ZERO))
	{
		computeFaceCornerPts(box3d, YMAX, cornerPoints);
		BoundingBox2D box(cornerPoints, CommonConstants::ZERO);
		Point3D pt = segment.GetPoint(uVal);

		if(box.IsInside(Point2D(pt.GetX(), pt.GetZ())))
			return true;
	}
	
	//case ZMIN:
	plane.SetBasePoint(minX, minY, minZ);
	plane.SetNormal(0, 0,1);
	plane.Intersect(segment, 0.0, uVal, inPlane);
	if((uVal > -CommonConstants::ZERO && uVal < 1 + CommonConstants::ZERO))
	{
		computeFaceCornerPts(box3d, ZMIN, cornerPoints);
		BoundingBox2D box(cornerPoints, CommonConstants::ZERO);
		Point3D pt = segment.GetPoint(uVal);

		if(box.IsInside(Point2D(pt.GetX(), pt.GetY())))
			return true;
	}
	
	//case ZMAX:
	plane.SetBasePoint(minX, minY, maxZ);
	plane.Intersect(segment, 0.0, uVal, inPlane);
	if((uVal > -CommonConstants::ZERO && uVal < 1 + CommonConstants::ZERO))
	{
		computeFaceCornerPts(box3d, ZMAX, cornerPoints);
		BoundingBox2D box(cornerPoints, CommonConstants::ZERO);
		Point3D pt = segment.GetPoint(uVal);

		if(box.IsInside(Point2D(pt.GetX(), pt.GetY())))
			return true;
	}

	return stat;
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::computeFaceCornerPts(const BoundingBox3D &box,
											const ObliqueBoundingBox::BoxPlanePosition &position,
											 std::vector<Point2D > &points)const
{
	points.resize(4);
	double minX, maxX, minY, maxY, minZ, maxZ;
	box.GetAllRanges(minX, maxX, minY, maxY, minZ, maxZ);
	switch (position)
	{
	case XMIN:
	case XMAX:
		{
			points[0].Set(minY, minZ); //(0, 0, 0)
			points[1].Set(maxY, minZ); //(0, 1, 0)
			points[2].Set(maxY, maxZ); //(0, 1, 1)
			points[3].Set(minY, maxZ); //(0, 0, 1)
			
			break;
		}

	case YMIN:
	case YMAX:
		{
			points[0].Set(minX, minZ);//(0, 0, 0)
			points[1].Set(minX, maxZ);//(0, 0, 1)
			points[2].Set(maxX, maxZ);//(1, 0, 1)
			points[3].Set(maxX, minZ);//(1, 0, 0)

			break;
		}

	case ZMIN:
	case ZMAX:
		{
			points[0].Set(minX, minY);//(0, 0, 0)
			points[3].Set(maxX, minY);//(1, 0, 0)
			points[1].Set(maxX, maxY);//(1, 1, 0)
			points[2].Set(minX, maxY);//(0, 1, 0)

			break;
		}
	}
}
//-----------------------------------------------------------------------------

void ObliqueBoundingBox::getLocalExtents(double &minX, double &maxX, 
										 double &minY, double &maxY, 
										 double &minZ, double &maxZ)const
{
	minX = maxX = m_localPoints[0].GetX();
	minY = maxY = m_localPoints[0].GetY();
	minZ = maxZ = m_localPoints[0].GetZ();

	size_t size = m_localPoints.size();

	for(int i = 1; i < size; i++)
	{
		double x = m_localPoints[i].GetX();
		double y = m_localPoints[i].GetY();
		double z = m_localPoints[i].GetZ();

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
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::transformGlobalPointToLocalCoordinateSystem(
						const Point3D &globalPoint, Point3D &localPoint)const
{
	localPoint = globalPoint;
	localPoint.Transformation(m_inverseTransMatrix);
}

//-----------------------------------------------------------------------------

double ObliqueBoundingBox::GetZLength()const
{
	BoundingBox3D box(m_localPoints);
	double zMin = box.GetMinZ();
	double zMax = box.GetMaxZ();
	double length = fabs(zMax - zMin);

	return length;
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::Dump(Report &report)const
{
	//global coordinates
	report.AddMarker(0);
	report.Add(0, "Global coordinates are:\n");
	for(size_t iPt = 0; iPt < m_cornerPoints.size(); ++iPt)
	{
		report.Add(0, "Point %f,%f,%f\n", m_cornerPoints[iPt].GetX(),  
											m_cornerPoints[iPt].GetY(), 
											m_cornerPoints[iPt].GetZ());
	}

	//local coordinates
	report.AddMarker(0);
	report.Add(0, "Local coordinates are:\n");
	for(size_t iPt = 0; iPt < m_localPoints.size(); ++iPt)
	{
		report.Add(0, "%f,%f,%f\n", m_localPoints[iPt].GetX(),
									m_localPoints[iPt].GetY(),
									m_localPoints[iPt].GetZ());
	}

	//transformation matrix
	report.AddMarker(0);
	report.Add(0, "TransformationMatrix is:\n");
	m_transformationMatrix.Dump(report, 0);

	//sides of box
	report.Add(0, "Sides\n");
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[0].GetX(), m_cornerPoints[0].GetY(), m_cornerPoints[0].GetZ(),
											m_cornerPoints[1].GetX(), m_cornerPoints[1].GetY(), m_cornerPoints[1].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[0].GetX(), m_cornerPoints[0].GetY(), m_cornerPoints[0].GetZ(),
											m_cornerPoints[2].GetX(), m_cornerPoints[2].GetY(), m_cornerPoints[2].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[2].GetX(), m_cornerPoints[2].GetY(), m_cornerPoints[2].GetZ(),
											m_cornerPoints[3].GetX(), m_cornerPoints[3].GetY(), m_cornerPoints[3].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[3].GetX(), m_cornerPoints[3].GetY(), m_cornerPoints[3].GetZ(),
											m_cornerPoints[1].GetX(), m_cornerPoints[1].GetY(), m_cornerPoints[1].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[4].GetX(), m_cornerPoints[4].GetY(), m_cornerPoints[4].GetZ(),
											m_cornerPoints[5].GetX(), m_cornerPoints[5].GetY(), m_cornerPoints[5].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[4].GetX(), m_cornerPoints[4].GetY(), m_cornerPoints[4].GetZ(),
											m_cornerPoints[6].GetX(), m_cornerPoints[6].GetY(), m_cornerPoints[6].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[6].GetX(), m_cornerPoints[6].GetY(), m_cornerPoints[6].GetZ(),
											m_cornerPoints[7].GetX(), m_cornerPoints[7].GetY(), m_cornerPoints[7].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[7].GetX(), m_cornerPoints[7].GetY(), m_cornerPoints[7].GetZ(),
											m_cornerPoints[5].GetX(), m_cornerPoints[5].GetY(), m_cornerPoints[5].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[0].GetX(), m_cornerPoints[0].GetY(), m_cornerPoints[0].GetZ(),
											m_cornerPoints[4].GetX(), m_cornerPoints[4].GetY(), m_cornerPoints[4].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[1].GetX(), m_cornerPoints[1].GetY(), m_cornerPoints[1].GetZ(),
											m_cornerPoints[5].GetX(), m_cornerPoints[5].GetY(), m_cornerPoints[5].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[2].GetX(), m_cornerPoints[2].GetY(), m_cornerPoints[2].GetZ(),
											m_cornerPoints[6].GetX(), m_cornerPoints[6].GetY(), m_cornerPoints[6].GetZ());
	report.Add(0, "Line %f,%f,%f %f,%f,%f\n", m_cornerPoints[3].GetX(), m_cornerPoints[3].GetY(), m_cornerPoints[3].GetZ(),
											m_cornerPoints[7].GetX(), m_cornerPoints[7].GetY(), m_cornerPoints[7].GetZ());
	
	report.AddMarker(0);
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::DumpLocalExtents(Report &report)const
{
	double xMin, xMax, yMin, yMax, zMin, zMax;
	getLocalExtents(xMin, xMax, yMin, yMax, zMin, zMax);
	report.Add(0, "xMin is: %f\n", xMin);
	report.Add(0, "xMax is: %f\n", xMax);
	report.Add(0, "yMin is: %f\n", yMin);
	report.Add(0, "yMax is: %f\n", yMax);
	report.Add(0, "zMin is: %f\n", zMin);
	report.Add(0, "zMax is: %f\n", zMax);
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::computeInveseTransMatrix()
{
	Matrix dummyMatrix(1, m_transformationMatrix.GetNumRows());
	LUDecomposition solver(m_transformationMatrix, dummyMatrix);
	solver.GetInverse(m_inverseTransMatrix);
}

//-----------------------------------------------------------------------------

Point3D ObliqueBoundingBox::GetCentroid()const
{
	double x = 0.;
	double y = 0.;
	double z = 0.;
	size_t numPoints = m_cornerPoints.size();
	for(int i = 0; i < numPoints; i++)
	{
		x += m_cornerPoints[i].GetX();
		y += m_cornerPoints[i].GetY();
		z += m_cornerPoints[i].GetZ();		
	}

	x /= numPoints;
	y /= numPoints;
	z /= numPoints;

	Point3D centroid(x, y, z);
	
	return centroid;
}

//-----------------------------------------------------------------------------

double ObliqueBoundingBox::ComputeDiagonalLength()const
{
	double digonalDist = m_cornerPoints[0].Distance(m_cornerPoints[7]);
	return digonalDist;
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::GetLengths(double &width, double &length,
									double &height)const
{
	double xMin, xMax, yMin, yMax, zMin, zMax;
	getLocalExtents(xMin, xMax, yMin, yMax, zMin, zMax);

	width = fabs(yMax - yMin);
	length = fabs(xMax - xMin);
	height = fabs(zMax - zMin);
	
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::createInput(const std::vector<CommonUtil::Point3D> &points, 
									 Matrix &transMatrix, 
									 std::vector<Point3D > &cornerPoints)const
{
	if(!points.size())
		return;

	GeometricalUtil::GetLocalCoOrdinateSystem(points, transMatrix);

	std::vector<CommonUtil::Point3D> tmpPoints = points;
	
	// translate the points to local co-ordinate system
		//get inverse tranformation matrix
	Matrix inverseTrans;
	Matrix dummyMatrix(1, transMatrix.GetNumRows()); 
	LUDecomposition solver(transMatrix, dummyMatrix);
	solver.GetInverse(inverseTrans);

	for(int i = 0; i < tmpPoints.size(); ++i)
	{
		tmpPoints[i].Transformation(inverseTrans);
	}

	// create bounding box 3d
	BoundingBox3D box(tmpPoints);


	// collect corner points of this bounding box
	box.GetCornerPoints(cornerPoints);
	
	// translate these points to global co-ordinate system
	for(int i = 0; i < cornerPoints.size(); ++i)
	{
		cornerPoints[i].Transformation(transMatrix);
	}
}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::GetPlanarCornerPoints(std::vector<CommonUtil::Point3D> &points)const
{
	double minX, maxX, minY, maxY, minZ, maxZ;
	getLocalExtents(minX, maxX, minY, maxY, minZ, maxZ);

	// x axis index = 0
	// y axis index = 1;
	// z axis index = 2;
	double yLength, xLength, zLength;
	GetLengths(yLength, xLength, zLength);
	bool xyPlane = false;
	bool yzPlane = false;
	bool xzPlane = false;
	double minLength = zLength;
	xyPlane = true;
	if(minLength > yLength)
	{
		minLength = yLength;
		xzPlane = true;
		xyPlane = false;
		yzPlane = false;
	}
	if(minLength > xLength)
	{
		minLength = xLength;
		xzPlane = false;
		xyPlane = false;
		yzPlane = true;
	}

	/*double xHeight = maxX - minX;
	double yHeight = maxY - minY;
	double zHeight = maxZ - minZ;*/

	if(yzPlane)//xHeight < 1e-3)//CommonConstants::ZERO) // y-z plane
	{
		points.push_back(Point3D(minX, minY, minZ));
		points.push_back(Point3D(minX, maxY, maxZ));
		points.push_back(Point3D(minX, minY, minZ));
		points.push_back(Point3D(minX, maxY, maxZ));
	}
	else if(xzPlane)//yHeight < 1e-3) // x-z plane
	{
		points.push_back(Point3D(minX, minY, minZ));
		points.push_back(Point3D(minX, minY, maxZ));
		points.push_back(Point3D(maxX, minY, minZ));
		points.push_back(Point3D(maxX, minY, maxZ));
	}
	else if(xyPlane)//zHeight < 1e-3) // x-y plane
	{
		points.push_back(Point3D(minX, minY, minZ));
		points.push_back(Point3D(minX, maxY, minZ));
		points.push_back(Point3D(maxX, minY, minZ));
		points.push_back(Point3D(maxX, maxY, minZ));
	}

//	Report report("C:\\temp\\obbPoints.txt");
	for(int i = 0; i < points.size(); i++)
	{
		points[i].Transformation(m_transformationMatrix);
//		report.Add(0, "%f,%f,%f\n", points[i].GetX(),points[i].GetY(),points[i].GetZ());
	}
//	report.Add(0, "MinX: %f, MaxX: %f, MinY: %f, MaxY: %f, MinZ: %f, MaxZ: %f\n", minX, maxX, minY, maxY, minZ, maxZ); 
//	report.Flush();

}

//-----------------------------------------------------------------------------

void ObliqueBoundingBox::Expand(bool xDir, bool yDir, bool zDir, double value)
{
	double minX, maxX, minY, maxY, minZ, maxZ;
	getLocalExtents(minX, maxX, minY, maxY, minZ, maxZ);

	if(xDir)
	{
		minX -= value;
		maxX += value;
	}

	if(yDir)
	{
		minY -= value;
		maxY += value;
	}

	if(zDir)
	{
		minZ -= value;
		maxZ += value;
	}

	BoundingBox3D box(minX, maxX, minY, maxY, minZ, maxZ);
	box.GetCornerPoints(m_localPoints);
	
	m_cornerPoints.resize(m_localPoints.size());
	for(int i = 0; i < m_localPoints.size(); i++)
	{
		Point3D tmpPoint = m_localPoints[i];
		tmpPoint.Transformation(m_transformationMatrix);
		m_cornerPoints[i] = tmpPoint;
	}
}

//-----------------------------------------------------------------------------

}