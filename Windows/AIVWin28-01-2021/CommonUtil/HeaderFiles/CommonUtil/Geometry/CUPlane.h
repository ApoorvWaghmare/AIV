// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _PLANE_H_
#define _PLANE_H_

// Std Include Files
#pragma warning (disable: 4786)
#include <vector>

// Include Files

#include "CUvector.h"

namespace CommonUtil
{
// Forward Declarations
class Segment3D;
class Triangle3D;
class Point3D;
class Point3DF;

// This class represents the infinite plane in 3 dimensions
class Plane
{
	//point on the plane
	Point3D m_point;

	//normal of the plane
	Vector m_normal;

	//computes rms plane using cross product
	bool rmsUsingCrossProducts(const Point3D &basePoint,
							   std::vector <CommonUtil::Point3D> &surroundingPoints);

	//checks if the input points are collinear
	bool checkCollinearity(const Point3D &basePoint,
						   std::vector <const Point3D*> &surroundingPoints,
						   double tolerance = 0.0001);

	bool checkCollinearity1(const Point3D &basePoint, std::vector <CommonUtil::Point3D> &surroundingPoints,
							 double tolerance = 0.0001);

	//checks if the input points are collinear
	bool checkCollinearity(const std::vector <CommonUtil::Point3D> &surroundingPoints,
		double tolerance = CommonUtil::CommonConstants::COLLINEARITY_TOL)const;

public:

	// Default constructor
	Plane()
	{}

	// Constructor using base point and normal
	Plane(const Point3D &point, const Vector &normal):
		  m_point(point), m_normal(normal)
	{
		m_normal.Normalize();
	}

	// Constructor using base point and normal
	Plane(const Point3DF &point, const Vector &normal) :
		m_normal(normal)
	{
		m_point.Set(point.GetX(), point.GetY(), point.GetZ());
		m_normal.Normalize();
	}
	// Destructor
	~Plane(){}

	enum PlaneIntersectionType
	{
		INTERSECTING,
		POINT_ON_PLANE,
		SIDE_ON_PLANE,
		COPLANAR,
		NONE
	};

	//creates plane by using three points
	bool CreateFromThreePoints(const Point3D *p1, const Point3D *p2, const Point3D *p3);

	bool CreateFromThreePoints(const Point3DF *p1, const Point3DF *p2, const Point3DF *p3);

	//creates plane using base point and its surrounding points
	//computes root mean square plane passing through the base point
	bool Create(const Point3D &basePoint, 
				std::vector <CommonUtil::Point3D> &surroundingPoints);

	//creates plane by using three points
	bool Create(const Point3D &p1, const Point3D &p2, const Point3D &p3);

	/*bool Create1(const Point3D &basePoint, 
				 std::vector <const Point3D> surroundingPoints,
				 bool &badPoint);*/

	/*bool Create1(const Point3D &basePoint, 
				 std::vector <const Point3DF*> surroundingPoints,
				 bool &badPoint);*/

	bool Create2(const Point3D &basePoint, 
				 std::vector <CommonUtil::Point3D> &surroundingPoints,
				 bool &badPoint);

	bool Create3(const Point3D &basePoint, 
				 std::vector <CommonUtil::Point3D> &surroundingPoints, 
				 double &projectedDistance);

	//computes rms plane using eigen vectors
	bool CreateUsingEigenVectors(const Point3D &basePoint,
							     std::vector <const Point3D *> surroundingPoints);

	bool CreateUsingEigenVectors(const Point3D &basePoint,
							     std::vector <const Point3DF *> surroundingPoints);

	//sets the normal using a vector
	bool SetNormal(const Vector &normal)
	{
		m_normal = normal;
		return m_normal.Normalize(0.0);
	}

	//computes signed distance of a point from the plane
	//sign is -ve if the normal is pointing towards the input point
	double GetSignedDistance(const Point3D &point)const;

	void SetPoint(const Point3D &point)
	{
		m_point.SetX(point.GetX());
		m_point.SetY(point.GetY());
		m_point.SetZ(point.GetZ());
	}

	void SetPoint(float x, float y, float z)
	{
		m_point.SetX(x);
		m_point.SetY(y);
		m_point.SetZ(z);
	}

	// sets the point of the plane to the location of input point
	void SetBasePoint(const Point3D &point)
	{
		m_point = point;
	}
	
	//sets normal using 3 components
	bool SetNormal(float i, float j, float k)
	{
		m_normal.SetI(i);
		m_normal.SetJ(j);
		m_normal.SetK(k);
		
		return m_normal.Normalize();;
	}

	// sets the point of the plane using given co-ordinates
	void SetBasePoint(double x, double y, double z)
	{
		m_point.Set(x, y, z);
	}
		
	//returns the base point of the plane
	const Point3D& GetBasePoint()const
	{
		return m_point;
	}

	//projects input point on the plane and returns the resulting point
	bool ProjectPoint(const Point3D &point, Point3D &projection, double projTol)const;

	bool ProjectPoint(const Point3D *point, Point3D &projection, double projTol)const;
	
	bool ProjectPoint(const Point3DF *point, Point3D &projection, double projTol)const;

	Vector ProjectVector(const Vector &vec)const;

	double GetProjectedDistance(const Point3D *point)const;
	
	double GetProjectedDistance(const Point3DF *point)const;

	//computes signed distance of a point from the plane
	//sign is -ve if the normal is pointing towards the input point
	double SignedDistance(const Point3D &point)const
	{
		Vector v(m_point.GetX() - point.GetX(),
				 m_point.GetY() - point.GetY(),
				 m_point.GetZ() - point.GetZ());

		return m_normal.DotProduct(v);
	}

	//reverses the direction of the normal
	void ReverseNormal()
	{
		m_normal.Reverse();
	}

	//returns the plane normal
	const Vector& GetNormal()const
	{
		return m_normal;
	}

	// computes the intersection of a segment with the plane.
	// if intersection is found, then parametric value of the intersection point
	// on the input segment is returned
	bool Intersect(const Segment3D &segment, double snappingDistTol,
		double &resultUval, bool &inPlane)const;

	// check that whether the given triangle is coplanar with the plane or not 
	bool IsTriangleCoplanar(const Triangle3D& triangle)const;

	// computes the intersection of a Triangle with the plane
	// Here, trianglePointsOnPlane are points of input triangle which are extactly on plane
	// trianglePointsOnPlane are useful because function Plane::SignedDistance()
	// fails to give zero signed distance for points exactly on plane
	bool IsTriangleIntersecting(const Triangle3D& triangle, Point3D intersectionPoints[2],
		PlaneIntersectionType& planeIntersectionType, const std::vector <CommonUtil::Point3D>& trianglePointsOnPlane)const;

	double GetPlaneConstant();

	// Create plane passing through points
	void CreateUsingPoints(const std::vector<Point3D >& dataPoints);

	//dump the data
	void Dump(CommonUtil::Report &report, int tabIndent)const;

	//computes plane plane intersection
	bool IsPlaneIntersecting(Vector& alongIntersection, Point3D& point,
		const Plane& otherPlane)const;

	//this method accepts reference to object of Transform3D
	//this methods transforms self
	//if successful returns true
	bool Transform(const Transform3D &transform);

	//computes intersection of segment with plane
	bool IsSegment3DIntersecting(const Segment3D &segment, Point3D& point)const;

	//Is point on plane
	bool IsPointOnPlane(const Point3D& point, double tol)const;
};
}
#endif