// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _ESSENCE_OBLIQUEBOUNDINGBOX_H_
#define _ESSENCE_OBLIQUEBOUNDINGBOX_H_

//geom include files
#include "CUPoint3D.h"
#include "CUPoint2D.h"
#include "CUSegment3D.h"
#include "CUSegment2D.h"
#include "CUVector.h"

//math include files
#include "CUMatrix.h"

namespace CommonUtil
{

//forward declaration
class Report;
class SqMatrix;

// This class implements oblique bounding box
class ObliqueBoundingBox
{
	// Arrangement of global corner points

	//						(7)*----------------------*(6)
	//						  /|					 /|
	//						 / |					/ |
	//						/  |				   /  |
	//					   /   *------------------/-- *(2)
	//					  /	  /	(3)				 /   /
	//					 /	 /				(5)	/   /
	//				 (4)* ---------------------*   /
	//					|  /				   |  /
	//					| /					   | /
	//				 (0)*----------------------*(1)

	//========================== private data members =========================

	//transformation matrix 
	//Global -> local
	Matrix m_transformationMatrix;

	//transformation matrix 
	//local -> Global
	Matrix m_inverseTransMatrix;

	//corner points in global coordinate system
	std::vector<CommonUtil::Point3D> m_cornerPoints;

	//corner points in local coordinate system
	// m_cornerPoints	---m_inverseTransMatrix --> m_localPoints
	std::vector<CommonUtil::Point3D> m_localPoints;

	// to go from m_localPoints to m_cornerPoints points
	// m_localPoints    ---m_transformationMatrix --> m_cornerPoints

	//======================= private members functions =======================

	enum BoxPlanePosition
	{
		XMIN,
		XMAX,
		YMIN,
		YMAX,
		ZMIN,
		ZMAX
	};

	//checks whether the given segment intersects with the box
	bool isSegmentIntersecting(const Segment3D &segment)const;

	//computes the inverse transformation matrix
	void computeInveseTransMatrix();

	//compute box edges
	void getBoxEdges(const std::vector<CommonUtil::Point3D> &localPoints,
					 std::vector<Segment3D> &segments)const;

	//set local points to the corner points using transformation matrix
	void setLocalPoints();

	//this function transforms given point from global coordinate system 
	//to local coordinate system
	void transformGlobalPointToLocalCoordinateSystem(const Point3D &globalPoint, 
													 Point3D &localPoint)const;



	//computes the extents of box in local coordinate system
	void getLocalExtents(double &minX, double &maxX, double &minY, 
						 double &maxY, double &minZ, double &maxZ)const;

	void createInput(const std::vector<CommonUtil::Point3D> &points, 
									 Matrix &transMatrix, 
									 std::vector<Point3D > &cornerPoints)const;
	void computeFaceCornerPts(const BoundingBox3D &box,
							  const ObliqueBoundingBox::BoxPlanePosition &position,
							  std::vector<Point2D > &points)const;
public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	ObliqueBoundingBox()
	{}

	//constructor using transformation matrix and corner points as input
	ObliqueBoundingBox(const Matrix &transformationMatrix, 
					   const std::vector<Point3D > &cornerPoints);


	ObliqueBoundingBox(const std::vector<CommonUtil::Point3D> &points);

	//Create box for given cylinder params
	ObliqueBoundingBox(const CommonUtil::Point3D &cylBasePt, const CommonUtil::Vector &cylNormal, const double & radius, const double & height);

	//****************************** destructor *******************************

	// Destructor
	~ObliqueBoundingBox()
	{}

	//*************************** get/set methods *****************************

	//checks whether given global point is in bounding box
	bool IsPointInBox(const Point3D &point, double* pMaxDist_ = NULL )const;

	const Matrix& GetTransMatrix()const
	{
		return m_transformationMatrix;
	}

	const Matrix& GetInverseTransMatrix()const
	{
		return m_inverseTransMatrix;
	}

	//get length of bounding box in local z direction
	double GetZLength()const;

	//get global centroid of bounding box
	Point3D GetCentroid()const;

	//get dimensions of bounding box
	// length = xLength
	// width = yLength
	// height = zLength
	void GetLengths(double &width, double &length, double &height)const;

	//sets parameters to bounding box
	void Set(const Matrix &transformationMatrix, 
			 const std::vector<CommonUtil::Point3D> &cornerPoints);

	//**************************general methods********************************

	//checks for intersection of given box with this
	bool IsIntersecting(const ObliqueBoundingBox &box)const;
	
	//function is used for getting global corner points
	void GetGlobalCornerPoints(std::vector<CommonUtil::Point3D> &cornerPoints)const;

	//dump the information box
	void Dump(Report &report)const;

	//dumps extents of box in local coordinate system
	void DumpLocalExtents(Report &report)const;

	//computes digonal length
	double ComputeDiagonalLength()const;

	void GetPlanarCornerPoints(std::vector<CommonUtil::Point3D> &points)const;

	void Expand(bool xDir, bool yDir, bool zDir, double value);
};
}

#endif