// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Std Include Files
#include <algorithm>
#include <float.h>

// Include Files
#include "CUplane.h"
#include "CUsegment3d.h"
#include "CUtriangle3d.h"
#include "CUsqmatrix.h"
#include "CUhouseholder.h"
#include "CUCPPoint3D.h"
#include "CUMathUtil.h"
#include "CUReport.h"

namespace CommonUtil
{
//MemManager* Plane::m_MemMgr = 0;

//-----------------------------------------------------------------------------

bool Plane::rmsUsingCrossProducts(const Point3D &basePoint,
								  std::vector <CommonUtil::Point3D> &surroundingPoints)
{
	bool stat = !checkCollinearity1(basePoint, surroundingPoints);

	if (!stat)
		return stat;

	float x =0, y=0, z=0;
	
	surroundingPoints.push_back(basePoint);

	size_t n = surroundingPoints.size();
	for(int i=0; i< n; i++)
	{
		 x += surroundingPoints[i].GetX();
		 y += surroundingPoints[i].GetY();
		 z += surroundingPoints[i].GetZ();
	}
	x /= n;
	y /= n;
	z /= n;

	std::vector<Vector> vectors;
	Vector tmpVec, summationVector;
	
	for (int i = 0; i < n; i++)
	{
		tmpVec.Set(surroundingPoints[i].GetX() - x,
				   surroundingPoints[i].GetY() - y,
				   surroundingPoints[i].GetZ() - z);

		double sqrMag = tmpVec.GetSqrMagnitude();
		if(sqrMag > CommonConstants::SQR_PNT_TOL)
		{
			vectors.push_back(tmpVec);
			summationVector += tmpVec;
			//tmpVec.scalarmultiply((float) 1./sqrMag);
		}
	}

	size_t vectorSize = vectors.size();

	std::vector<Vector> crossProds;

	Vector crossProd;
	
	double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	for( int k = 0; k < vectorSize-1; k++)
	{
		if(!(vectors[k].IsValid()))
			continue;

		vectors[k].Normalize();

		for(int j = k+1; j < vectorSize; j++)
		{	
			if(!(vectors[j].IsValid()))
				continue;

			vectors[j].Normalize();
			vectors[k].CrossProduct(vectors[j], crossProd);
			crossProds.push_back(crossProd);

			sumX += fabs(crossProd.GetI());
			sumY += fabs(crossProd.GetJ());
			sumZ += fabs(crossProd.GetK());
		}		
	}
	Vector crossProductSum, dir;

	if (sumX > sumY)
	{
		if (sumX > sumZ)
			dir.SetI(1.0);
		else
			dir.SetK(1.0);
	}
	else
	{
		if (sumY > sumZ)
			dir.SetJ(1.0);
		else
			dir.SetK(1.0);
	}

	size_t size = crossProds.size();

	for (int i = 0; i < size; i++)
	{
		if (crossProds[i].DotProduct(dir) < 0)
			crossProds[i].Reverse();
		crossProductSum += crossProds[i];
	}

	SetNormal(crossProductSum);

	m_point = basePoint;

	//checking if size_t resulting RMS plane is right
	double sqrMag = summationVector.GetSqrMagnitude();
	
	double edgeTol = 20.0;
	if (sqrMag > edgeTol * edgeTol)
	{
		stat = true;
	}

	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::checkCollinearity(const Point3D &basePoint, std::vector <const Point3D *> &surroundingPoints,
							  double tolerance)
{
	bool stat = true;

	size_t n = surroundingPoints.size();

	if (n > 1) // there are more than two points including size_t base point
	{
		// There will be an issue if size_t two points are coincident
		Vector firstVec;
		int i;
		for (i = 0; i < n; i++)
		{
			firstVec.Set(basePoint, *surroundingPoints[i]);
			if (firstVec.Normalize())
				break;
		}
		Vector crossProd;
		for (i++; i < n; i++)
		{
			Vector tmpVec(basePoint, *surroundingPoints[i]);
			
			if (tmpVec.Normalize())
			{
				firstVec.CrossProduct(tmpVec, crossProd);
				if (crossProd.GetSqrMagnitude() > tolerance * tolerance) // justify this magic figure
				{
					stat = false;
					break;
				}
			}
		}
	}
	else
	{
	}

	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::checkCollinearity1(const Point3D &basePoint, std::vector <CommonUtil::Point3D> &surroundingPoints,
							  double tolerance)
{
	bool stat = true;

	size_t n = surroundingPoints.size();

	if (n > 1) // there are more than two points including size_t base point
	{
		// There will be an issue if size_t two points are coincident
		Vector firstVec;
		int i;
		for (i = 0; i < n; i++)
		{
			firstVec.Set(basePoint, (surroundingPoints[i]));
			if (firstVec.Normalize())
				break;
		}
		Vector crossProd;
		for (i++; i < n; i++)
		{
			Vector tmpVec(basePoint, (surroundingPoints[i]));
			
			if (tmpVec.Normalize())
			{
				firstVec.CrossProduct(tmpVec, crossProd);
				if (crossProd.GetSqrMagnitude() > tolerance*tolerance) // justify this magic figure
				{
					stat = false;
					break;
				}
			}
		}
	}
	else
	{
	}

	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::checkCollinearity(const std::vector <CommonUtil::Point3D> &surroundingPoints,
	double tolerance)const
{
	bool stat = true;

	size_t n = surroundingPoints.size();
	if (n < 3)
		return false;

	const Point3D &basePoint = surroundingPoints[0];

	Vector baseVec;
	for (int i = 1; i < n; ++i)
	{
		baseVec.Set(basePoint, surroundingPoints[i]);
		if (baseVec.IsValid())
		{
			//Normalize() returns false, if the vector is zero vector
			//Here, it has returned true. It means that the baseVec
			//is valid
			break;
		}
	}

	//Create all the vectors from the base point to the surrounding points.
	//Check the parallelism between base vector and these vectors.
	for (int i = 0; i < n; ++i)
	{
		Vector tmpVec(basePoint, surroundingPoints[i]);

		if (tmpVec.Normalize())
		{
			Vector::Direction tmpDir;
			if (!baseVec.IsParallel(tmpVec, tmpDir, tolerance))
			{
				stat = false;
				break;
			}
		}
	}

	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::CreateUsingEigenVectors(const Point3D &basePoint,
								 std::vector <const Point3D *> surroundingPoints)
{
	bool stat = !checkCollinearity(basePoint, surroundingPoints);

	if (!stat)
		return stat;

	Point3D centroid;
	float x =0, y=0, z=0;
	
	surroundingPoints.push_back(&basePoint);

	size_t n = surroundingPoints.size();
	for(int i=0; i<n; i++)
	{
		 x += surroundingPoints[i]->GetX();
		 y += surroundingPoints[i]->GetY();
		 z += surroundingPoints[i]->GetZ();
	}
	x /= n;
	y /= n;
	z /= n;
	centroid.SetX(x);
	centroid.SetY(y);
	centroid.SetZ(z);

	SqMatrix covariance(3);
	for(size_t i=0; i<n; i++)
	{
		SqMatrix temp(3), temp1(3), temp2(3);
	
		temp2(0,0) = surroundingPoints[i]->GetX() - centroid.GetX() ;
			
		temp2(0,1) = surroundingPoints[i]->GetY() - centroid.GetY() ;
				
		temp2(0,2) = surroundingPoints[i]->GetZ() - centroid.GetZ() ;

		temp2.GetTranspose(temp1);
		temp1.Multiply(temp2, temp);
		covariance.AddTo(temp);
	}

	std::vector<double> vec;

	Householder rootvalue;
	rootvalue.Execute(covariance, vec);

	float min = (float) vec[0];
	int min_index = 0;
	for(size_t i = 0; i<3; i++)
	{
		if(vec[i] < min)
		{
			min= (float) vec[i];
			min_index = i;
		}
	}

	for(size_t i = 0; i <3; i++)
	{
		vec.push_back( covariance(i, min_index));
	}
	
	SetNormal((float) vec[0], (float) vec[1], (float) vec[2]);

	m_point = basePoint;
	return true;
}

//-----------------------------------------------------------------------------

bool Plane::CreateUsingEigenVectors(const Point3D &basePoint,
								 std::vector <const Point3DF *> surroundingPoints)
{
	std::vector <const Point3D *> surrounding3DPoints;
	for(size_t pointCount = 0; pointCount < surroundingPoints.size(); ++pointCount)
	{
		const Point3D * p = new Point3D(*(Point3D*)surroundingPoints[pointCount]);
		surrounding3DPoints.push_back(p);
	}
	bool stat = CreateUsingEigenVectors( basePoint, surrounding3DPoints);

	for(size_t pointCount = 0; pointCount < surrounding3DPoints.size(); ++pointCount)
	{
		const Point3D * p = surrounding3DPoints[pointCount];
		delete p;
	}
	return stat;
}

//-----------------------------------------------------------------------------

double Plane::GetSignedDistance(const Point3D &point)const
{
	Vector v(m_point.GetX() - point.GetX(),
		m_point.GetY() - point.GetY(),
		m_point.GetZ() - point.GetZ());

	return m_normal.DotProduct(v);
}

//-----------------------------------------------------------------------------

bool Plane::CreateFromThreePoints(const Point3D *p1, const Point3D *p2, const Point3D *p3)
{
	Vector v1 (*p1, *p2);
	Vector v2 (*p1, *p3);
	v1.Normalize();
	v2.Normalize();

	Vector crossProd;
	v1.CrossProduct(v2, crossProd);
	if(crossProd.GetSqrMagnitude() > 1e-8)
	{
		SetNormal(crossProd);

		m_point = *p1;
	}
	else
		return false;

	return true;
}

//-----------------------------------------------------------------------------

bool Plane::CreateFromThreePoints(const Point3DF *p1, const Point3DF *p2, const Point3DF *p3)
{
	Point3D pt1(p1->GetX(), p1->GetY(), p1->GetZ());
	Point3D pt2(p2->GetX(), p2->GetY(), p2->GetZ());
	Point3D pt3(p3->GetX(), p3->GetY(), p3->GetZ());
	return CreateFromThreePoints(&pt1, &pt2, &pt3);
}

//-----------------------------------------------------------------------------

bool Plane::Create(const Point3D &basePoint, std::vector <CommonUtil::Point3D> &surroundingPoints)
{
	return rmsUsingCrossProducts(basePoint, surroundingPoints);
}

//-----------------------------------------------------------------------------

bool Plane::Create(const Point3D &p1, const Point3D &p2, const Point3D &p3)
{
	std::vector <CommonUtil::Point3D> surroundingPoints(3);

	surroundingPoints[0] = p1;
	surroundingPoints[1] = p2;
	surroundingPoints[2] = p3;

	bool stat = !checkCollinearity(surroundingPoints);

	if (stat)
	{
		Vector v1(p1, p2);
		Vector v2(p1, p3);

		v1.Normalize();
		v2.Normalize();

		Vector normalVector;
		v1.CrossProduct(v2, normalVector);
		if (!normalVector.IsValid())
			return false;

		SetNormal(normalVector);
		m_point = p1;
	}
	return stat;
}

//-----------------------------------------------------------------------------

/*bool Plane::Create1(const Point3D &basePoint, std::vector <const Point3D> surroundingPoints,
					bool &edgePoint)
{
	return rmsUsingCrossProducts(basePoint, surroundingPoints, edgePoint);
	//return rmsUsingEigenVectors(basePoint, surroundingPoints);
}*/

//-----------------------------------------------------------------------------

/*bool Plane::Create1(const Point3D &basePoint, std::vector <const Point3DF*> surroundingPoints,
					bool &edgePoint)
{
	std::vector <const Point3DF*> surrounding3DPoints;
	for(size_t pointCount = 0; pointCount < surroundingPoints.size(); ++pointCount) 
	{
		Point3DF *p = new Point3DF(*surroundingPoints[pointCount]);
		surrounding3DPoints.push_back(p);
	}
	//bool stat =  Create1(basePoint, surrounding3DPoints, edgePoint);
	bool stat = rmsUsingCrossProducts(basePoint, surroundingPoints, edgePoint);
	for(size_t pointCount = 0; pointCount < surroundingPoints.size(); ++pointCount) 
	{
		const Point3DF *p = surrounding3DPoints[pointCount];
		delete p; 
	}
	return stat;
	//return rmsUsingEigenVectors(basePoint, surroundingPoints);
}*/

//-----------------------------------------------------------------------------


bool Plane::Create2(const Point3D &basePoint, 
			 std::vector <CommonUtil::Point3D> &surroundingPoints,
			 bool &badPoint)
{
	bool stat = !checkCollinearity1(basePoint, surroundingPoints);

	if (!stat)
		return stat;

	float x =0, y=0, z=0;
	
 	surroundingPoints.push_back(basePoint);

	size_t n = surroundingPoints.size();
	for(int i=0; i< n; i++)
	{
		 x += surroundingPoints[i].GetX();
		 y += surroundingPoints[i].GetY();
		 z += surroundingPoints[i].GetZ();
	}
	x /= n;
	y /= n;
	z /= n;

	std::vector<Vector> vectors;
	vectors.reserve(n);
	Vector tmpVec, summationVector;
	
	for (size_t i = 0; i < n; i++)
	{
		tmpVec.Set(surroundingPoints[i].GetX() - x,
				   surroundingPoints[i].GetY() - y,
				   surroundingPoints[i].GetZ() - z);

		double sqrMag = tmpVec.GetSqrMagnitude();
		if(sqrMag > CommonConstants::SQR_PNT_TOL)
		{
			vectors.push_back(tmpVec);
			summationVector += tmpVec;
			//tmpVec.scalarmultiply((float)(1./sqrMag));
		}
	}

	size_t vectorSize = vectors.size();

	std::vector<Vector> crossProds;
	crossProds.reserve(vectorSize);
	Vector crossProd;
	
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	for( int k = 0; k < vectorSize-1; k++)
	{
		if(!(vectors[k].IsValid()))
			continue;

		vectors[k].Normalize();

		for(int j = k+1; j < vectorSize; j++)
		{	
			if(!(vectors[j].IsValid()))
				continue;

			vectors[j].Normalize();			
			vectors[k].CrossProduct(vectors[j], crossProd);
			crossProds.push_back(crossProd);

			sumX += fabs(crossProd.GetI());
			sumY += fabs(crossProd.GetJ());
			sumZ += fabs(crossProd.GetK());
		}			
	}

	Vector crossProductSum, dir;

	if (sumX > sumY)
	{
		if (sumX > sumZ)
			dir.SetI(1.0);
		else
			dir.SetK(1.0);
	}
	else
	{
		if (sumY > sumZ)
			dir.SetJ(1.0);
		else
			dir.SetK(1.0);
	}

	size_t size = crossProds.size();

	for (size_t i = 0; i < size; i++)
	{
		if (crossProds[i].DotProduct(dir) < 0)
			crossProds[i].Reverse();
		crossProductSum += crossProds[i];
	}

	SetNormal(crossProductSum);

	m_point = basePoint;

	//checking if size_t resulting RMS plane is right
	double sqrMag = summationVector.GetSqrMagnitude();
	
	double edgeTol = 20.0;
	badPoint = false;
	if (sqrMag > edgeTol * edgeTol)
	{
		badPoint = false;
		stat = true;
	}

	return stat;
}


//-----------------------------------------------------------------------------


bool Plane::Create3(const Point3D &basePoint, 
			 std::vector <CommonUtil::Point3D> &surroundingPoints,
			 double &projectedSqDistanceAvg)
{
	bool stat = !checkCollinearity1(basePoint, surroundingPoints);

	if (!stat)
		return stat;

	float x =0, y=0, z=0;
	
 	surroundingPoints.push_back(basePoint);

	size_t n = surroundingPoints.size();
	for(int i=0; i< n; i++)
	{
		 x += surroundingPoints[i].GetX();
		 y += surroundingPoints[i].GetY();
		 z += surroundingPoints[i].GetZ();
	}
	x /= n;
	y /= n;
	z /= n;

	std::vector<Vector> vectors;
	vectors.reserve(n);
	Vector tmpVec, summationVector;
	
	for (size_t i = 0; i < n; i++)
	{
		tmpVec.Set(surroundingPoints[i].GetX() - x,
				   surroundingPoints[i].GetY() - y,
				   surroundingPoints[i].GetZ() - z);

		double sqrMag = tmpVec.GetSqrMagnitude();
		if(sqrMag > CommonConstants::SQR_PNT_TOL)
		{
			vectors.push_back(tmpVec);
			summationVector += tmpVec;
			//tmpVec.scalarmultiply((float)(1./sqrMag));
		}
	}

	size_t vectorSize = vectors.size();

	std::vector<Vector> crossProds;
	crossProds.reserve(vectorSize);
	Vector crossProd;
	
	size_t bestCrossProdIndex;
	double maxMag = 0.;
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	for( int k = 0; k < vectorSize-1; k++)
	{
		if(!(vectors[k].IsValid()))
			continue;

		vectors[k].Normalize();

		for(int j = k+1; j < vectorSize; j++)
		{	
			if(!(vectors[j].IsValid()))
				continue;

			vectors[j].Normalize();			
			vectors[k].CrossProduct(vectors[j], crossProd);
			crossProds.push_back(crossProd);

			double mag = crossProd.GetMagnitude();
			if(mag > maxMag)
			{
				maxMag = mag;
				bestCrossProdIndex = crossProds.size() - 1;
			}
			crossProd.Normalize();
			crossProd.ScalarMultiply(mag*mag*mag*mag);
			sumX += fabs(crossProd.GetI());
			sumY += fabs(crossProd.GetJ());
			sumZ += fabs(crossProd.GetK());
		}			
	}

	Vector dir = crossProds[bestCrossProdIndex];
	
	size_t size = crossProds.size();

	Vector crossProductSum;
	for (size_t i = 0; i < size; i++)
	{
		if (crossProds[i].DotProduct(dir) < 0)
			crossProds[i].Reverse();

		crossProductSum += crossProds[i];
	}

	if( dir.DotProduct(crossProductSum) < 0 )
		crossProductSum.Reverse();
	SetNormal(crossProductSum);

	
	m_point = CommonUtil::CPPoint3D(x,y,z);

	//checking if size_t resulting RMS plane is right
	double sqrMag = summationVector.GetSqrMagnitude();
	projectedSqDistanceAvg = 0.;
	double dist = 0.;
	for (size_t i = 0; i < n; i++)
	{
		dist = GetProjectedDistance(&surroundingPoints[i]);
		projectedSqDistanceAvg+= (dist*dist);
	}
	projectedSqDistanceAvg/=n;
	return stat;
}


//-----------------------------------------------------------------------------

bool Plane::ProjectPoint(const Point3D &point, Point3D &projection, double projTol)const
{	
	//This forms a line connecting basepoint of plane and 
	//size_t given point to be projected

	Vector vec(m_point, point);

	double perDistance = m_normal.DotProduct(vec);

	float x = (float)((double)point.GetX() - (double)m_normal.GetI() * perDistance);
	float y = (float)((double)point.GetY() - (double)m_normal.GetJ() * perDistance);
	float z = (float)((double)point.GetZ() - (double)m_normal.GetK() * perDistance);

	projection.Set(x, y, z);

	if (projection.SqrDistance(point) < projTol*projTol)
		return true;

	return false;
}
//-----------------------------------------------------------------------------

bool Plane::ProjectPoint(const Point3D *point, Point3D &projection, double projTol)const
{
	bool stat = ProjectPoint( *point, projection, projTol);
	return stat;
}
//-----------------------------------------------------------------------------
bool Plane::ProjectPoint(const Point3DF *point, Point3D &projection, double projTol)const
{
	Point3D p(point->GetX(), point->GetY(), point->GetZ());
	bool stat = ProjectPoint(p, projection, projTol);
	return stat;
}

//-----------------------------------------------------------------------------
Vector Plane::ProjectVector(const Vector &vec)const
{
	Point3D tmpPoint;
	vec.GetPointAlongVector(m_point, tmpPoint, vec.GetMagnitude());
	
	Point3D p1;
	ProjectPoint(tmpPoint, p1, DBL_MAX);

	return Vector(m_point, p1);
}

//-----------------------------------------------------------------------------

double Plane::GetProjectedDistance(const Point3D *point)const
{	
	//This forms a line connecting basepoint of plane and 
	//size_t given point to be projected

	Vector vec(m_point, *point);

	double perDistance = m_normal.DotProduct(vec);

	float x = (float)((double)point->GetX() - (double)m_normal.GetI() * perDistance);
	float y = (float)((double)point->GetY() - (double)m_normal.GetJ() * perDistance);
	float z = (float)((double)point->GetZ() - (double)m_normal.GetK() * perDistance);

	Point3D projection;
	projection.Set(x, y, z);

	return projection.Distance(*point);
}

//-----------------------------------------------------------------------------

double Plane::GetProjectedDistance(const Point3DF *point)const
{
	Point3D tempPoint(*(Point3D*)point);
	return GetProjectedDistance(&tempPoint);
}

//-----------------------------------------------------------------------------

bool Plane::Intersect(const Segment3D &segment, double snappingDistTol,
	double &resultUval, bool &inPlane)const
{
	bool stat = false;

	inPlane = false;
	resultUval = -1;

	//compute the d term of equation of plane ax + by + cz + d = 0
	double d = -(m_normal.GetI()*m_point.GetX() +
		m_normal.GetJ()*m_point.GetY() +
		m_normal.GetK()*m_point.GetZ());

	Point3D p1 = segment.GetStartPoint();
	//put the point p1 in equation of plane
	double signedVal1 = m_normal.GetI()*p1.GetX() +
		m_normal.GetJ()*p1.GetY() +
		m_normal.GetK()*p1.GetZ() + d;

	Point3D p2 = segment.GetEndPoint();

	//put the point p2 in equation of plane	
	double signedVal2 = m_normal.GetI()*p2.GetX() +
		m_normal.GetJ()*p2.GetY() +
		m_normal.GetK()*p2.GetZ() + d;

	// signedVal1 and signedVal2 indicate the signed distances from the plane

	if (fabs(signedVal1) < CommonUtil::CommonConstants::ZERO &&
		fabs(signedVal2) < CommonUtil::CommonConstants::ZERO)
	{
		//both signed distances are zero; therefore the segment is in the plane
		inPlane = true;
		stat = true;
	}
	else if (signedVal1 * signedVal2 < CommonUtil::CommonConstants::ZERO)
	{
		resultUval = fabs(signedVal1) /
			(fabs(signedVal1) + fabs(signedVal2));
		stat = true;

		//check if the intersection point can be snapped to the ends
		double segmentLength = segment.GetLength();
		double distanceFromP1 = resultUval * segmentLength;
		double distanceFromP2 = segmentLength - distanceFromP1;

		if (distanceFromP1 < snappingDistTol)
			resultUval = 0.;
		else if (distanceFromP2 < snappingDistTol)
			resultUval = 1.0;
	}
	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::IsTriangleCoplanar(const Triangle3D& triangle)const
{
	Point3D triPoints[3];
	triangle.GetPoints(triPoints);

	// Compute Signed Distances of Triangle points from the plane.
	double dist = SignedDistance(triPoints[0]);
	if(fabs(dist) > CommonConstants::PNT_TOL)
		return false;

	dist = SignedDistance(triPoints[1]);
	if(fabs(dist) > CommonConstants::PNT_TOL)
		return false;

	dist = SignedDistance(triPoints[2]);
	if(fabs(dist) > CommonConstants::PNT_TOL)
		return false;		
	
	return true; 
}

//-----------------------------------------------------------------------------

bool Plane::IsTriangleIntersecting(const Triangle3D& triangle, Point3D intersectionPoints[2],
								   PlaneIntersectionType& planeIntersectionType, 
								   const std::vector <CommonUtil::Point3D>& trianglePointsOnPlane)const
{
	planeIntersectionType  = NONE;

	Point3D triPoints[3];
	triangle.GetPoints(triPoints);

	//---------------------- EARLY EXIT --------------------------------------
	// Compute distance signs of Triangle points V0,V1,V2 to the plane.

	double d0 = SignedDistance(triPoints[0]);
	double d1 = SignedDistance(triPoints[1]);
	double d2 = SignedDistance(triPoints[2]);
	
	// If any triPoint is on plane, set its signed distance as 0.0f
	for(size_t iPoint = 0; iPoint < trianglePointsOnPlane.size(); ++iPoint)
	{
		const Point3D& pointOnPlane = trianglePointsOnPlane[iPoint];

		if(pointOnPlane.SqrDistance(triPoints[0]) < CommonConstants::SQR_PNT_TOL)
			d0 = 0.0f;
		else if(pointOnPlane.SqrDistance(triPoints[1]) < CommonConstants::SQR_PNT_TOL)	
			d1 = 0.0f;
		else if(pointOnPlane.SqrDistance(triPoints[2]) < CommonConstants::SQR_PNT_TOL)
			d2 = 0.0f;
	}

	if ((d0*d1 > 0.0f) && (d0*d2 > 0.0f)) 
	{	
		// same signs on all of them & not equal 0 ? No intersection occurs					   
		// all points are on same side of plane.
		planeIntersectionType = NONE;
		return false;					  
	}		

	if(trianglePointsOnPlane.size() == 3 )
	{
		// all points on plane.  return true;  type = COPLANAR 
		planeIntersectionType = COPLANAR;
	}
	else if( trianglePointsOnPlane.size() == 2 )
	{
		// two points on plane.	 return true;	type = SIDE_ON_PLANE
		intersectionPoints[0] = trianglePointsOnPlane[0];
		intersectionPoints[1] = trianglePointsOnPlane[1];

		planeIntersectionType = SIDE_ON_PLANE;
	}
	else if( trianglePointsOnPlane.size() == 1 )
	{
		// one point on plane.	 return true;	 type = POINT_ON_PLANE
		// Intersection Points - first is the point on plane & may be there is second point or not 

		Segment3D segment;
		double paramVal = -1;

		if((d0!=0.0f && d1!=0.0f) && (d0*d1 < 0.0f))		// D2 point is on plane && D0,D1 are opposite 
		{
			paramVal = (double)(fabs(d0) / (fabs(d0) + fabs(d1)));
			segment.Set(&triPoints[0],&triPoints[1]);								
		}
		else if((d0!=0.0f && d2!=0.0f) && (d0*d2 < 0.0f))	// D1 point is on plane && D0,D2 are opposite
		{
			paramVal = (double)(fabs(d0) / (fabs(d0) + fabs(d2)));
			segment.Set(&triPoints[0],&triPoints[2]);				
		}
		else if((d1!=0.0f && d2!=0.0f) && (d1*d2 < 0.0f))	// D0 point is on plane && D1,D2 are opposite
		{
			paramVal = (double)(fabs(d1) / (fabs(d1) + fabs(d2)));
			segment.Set(&triPoints[1],&triPoints[2]);				
		}

		intersectionPoints[0] = trianglePointsOnPlane[0];
		if( paramVal >= 0.0f && paramVal <= 1.0f)
			intersectionPoints[1] = segment.GetPoint(paramVal);
		else
			intersectionPoints[1] = trianglePointsOnPlane[0];

		planeIntersectionType = POINT_ON_PLANE;
	}
	else if( trianglePointsOnPlane.size() == 0 )
	{
		// no point on plane.	return true;	 type = INTERSECTING
		// Intersection Points - Two intesection points of two segments

		Segment3D segment1, segment2;

		double paramVal1 = -1;
		double paramVal2 = -1;

		if(d0*d1 > 0.0f)	// D2 point is opposite 
		{
			paramVal1 = (double)(fabs(d2) / (fabs(d2) + fabs(d0)));
			paramVal2 = (double)(fabs(d2) / (fabs(d2) + fabs(d1)));

			segment1.Set(&triPoints[2],&triPoints[0]);
			segment2.Set(&triPoints[2],&triPoints[1]);
		}
		else if(d0*d2 > 0.0f)	// D1 point is opposite 
		{
			paramVal1 = (double)(fabs(d1) / (fabs(d1) + fabs(d0)));
			paramVal2 = (double)(fabs(d1) / (fabs(d1) + fabs(d2)));

			segment1.Set(&triPoints[1],&triPoints[0]);
			segment2.Set(&triPoints[1],&triPoints[2]);
		}
		else if(d1*d2 > 0.0f)	// D0 point is opposite 
		{
			paramVal1 = (double)(fabs(d0) / (fabs(d0) + fabs(d1)));
			paramVal2 = (double)(fabs(d0) / (fabs(d0) + fabs(d2)));

			segment1.Set(&triPoints[0],&triPoints[1]);
			segment2.Set(&triPoints[0],&triPoints[2]);
		}			

		if( paramVal1 >= 0.0f && paramVal1 <= 1.0f)
			intersectionPoints[0] = segment1.GetPoint(paramVal1);

		if( paramVal2 >= 0.0f && paramVal2 <= 1.0f)
			intersectionPoints[1] = segment2.GetPoint(paramVal2);
				
		planeIntersectionType = INTERSECTING;
	}

	return true;	
}

//-----------------------------------------------------------------------------

double Plane::GetPlaneConstant()
{
	//calculate d(D gives the perpendicular distance from the origin to the plane.)
	Point3D origin(0,0,0);
	return GetProjectedDistance(&origin);
}

//-----------------------------------------------------------------------------

void Plane::CreateUsingPoints(const std::vector<Point3D >& dataPoints)
{
	//We should check the collinearity of the input points
	//If the input points are collinear, we cannot fit the plane.

	bool stat = !checkCollinearity(dataPoints);

	if (stat)
	{
		//input points are not collinear
		// the centroid of all the points is calculated
		size_t n = dataPoints.size();
		double x = 0, y = 0, z = 0;
		for (int i = 0; i < n; ++i)
		{
			x += dataPoints[i].GetX();
			y += dataPoints[i].GetY();
			z += dataPoints[i].GetZ();
		}

		x /= n;
		y /= n;
		z /= n;

		Point3D centroid(x, y, z);

		// compute the covariance matrix
		SqMatrix covariance(3);
		for (int i = 0; i < n; ++i)
		{
			SqMatrix temp(3), temp1(3), temp2(3);

			temp2(0, 0) = dataPoints[i].GetX() - centroid.GetX();
			temp2(0, 1) = dataPoints[i].GetY() - centroid.GetY();
			temp2(0, 2) = dataPoints[i].GetZ() - centroid.GetZ();

			temp2.GetTranspose(temp1);
			temp1.Multiply(temp2, temp);
			covariance.AddTo(temp);
		}

		std::vector<double> minEigenVector(3);

		// The eigen Vector corresponding to the minimum eigen value for the 
		// matrix "covariance" is obtained. This is the normal of the tangent plane.
		covariance.GetMinEigenVector(minEigenVector);

		SetNormal(minEigenVector[0], minEigenVector[1], minEigenVector[2]);

		m_point.Set(x, y, z);
	}
}

//-----------------------------------------------------------------------------

void Plane::Dump(CommonUtil::Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);

	report.Add(tabIndent, "Plane\n");
	report.Add(tabIndent, "m_point\n");
	m_point.Dump(report, tabIndent + 1);

	report.Add(tabIndent, "m_normal\n");
	m_normal.Dump(report, tabIndent + 1);

	report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

bool Plane::IsPlaneIntersecting(Vector& alongIntersection, Point3D& point,
	const Plane& otherPlane)const
{
	bool stat = false;
	const Vector &n2 = otherPlane.GetNormal();

	m_normal.CrossProduct(n2, alongIntersection);

	double xComp = fabs(alongIntersection.GetI());
	double yComp = fabs(alongIntersection.GetJ());
	double zComp = fabs(alongIntersection.GetK());

	if (alongIntersection.GetMagnitude() < CommonConstants::ZERO)
		return false;

	Point3D p1, p2;
	p1 = m_point;
	p2 = otherPlane.GetBasePoint();

	Vector n1 = m_normal;
	double a1 = n1.GetI();
	double b1 = n1.GetJ();
	double c1 = n1.GetK();

	double a2 = n2.GetI();
	double b2 = n2.GetJ();
	double c2 = n2.GetK();

	double x1 = p1.GetX();
	double y1 = p1.GetY();
	double z1 = p1.GetZ();

	double x2 = p2.GetX();
	double y2 = p2.GetY();
	double z2 = p2.GetZ();

	double d1 = (a1*x1 + b1*y1 + c1*z1);
	double d2 = (a2*x2 + b2*y2 + c2*z2);

	// a1 ( x - x1 ) + b1 ( y - y1 ) + c1 ( z - z1 ) = 0
	// a2 ( x - x2 ) + b2 ( y - y2 ) + c2 ( z - z2 ) = 0
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	if (xComp >= yComp && xComp >= zComp && xComp > CommonConstants::ZERO)
	{
		// solve for y and z for x = 0
		// eqns become
		// b1 y + c1 z - d1 = 0
		// b2 y + c2 z - d2 = 0

		MathUtil::SolveTwoLinearEquationsByCramersRule(b1, c1, -d1, b2,
			c2, -d2, y, z);
		x = 0.0;
		stat = true;
	}
	else if (yComp >= xComp && yComp >= zComp && yComp > CommonConstants::ZERO)
	{
		// solve for x and z for y = 0
		// eqns become
		// a1 x + c1 z - d1 = 0
		// a2 x + c2 z - d2 = 0

		MathUtil::SolveTwoLinearEquationsByCramersRule(a1, c1, -d1, a2,
			c2, -d2, x, z);
		y = 0.0;
		stat = true;
	}
	else if (zComp >= xComp && zComp >= yComp && zComp > CommonConstants::ZERO)
	{
		// solve for x and y for z = 0
		// eqns become
		// a1 x + b1 y - d1 = 0
		// a2 x + b2 y - d2 = 0

		MathUtil::SolveTwoLinearEquationsByCramersRule(a1, b1, -d1, a2,
			b2, -d2, x, y);
		z = 0.0;
		stat = true;
	}
	else
	{
		const Point3D &thisPlanePoint = GetBasePoint();
		const Point3D &otherPoint = otherPlane.GetBasePoint();

		double dist = thisPlanePoint.Distance(otherPoint);
		if (dist < CommonConstants::ZERO)
		{
			x = thisPlanePoint.GetX();
			y = thisPlanePoint.GetY();
			z = thisPlanePoint.GetZ();
			stat = true;
		}
	}

	point.Set(x, y, z);

	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::Transform(const Transform3D &transform)
{
	bool stat = true;

	//transforming vector
	Point3D otherPoint;
	Point3D firstPoint(m_point);
	m_normal.GetPointAlongVector(firstPoint, otherPoint, m_normal.GetMagnitude());
	firstPoint.Transform(transform);
	otherPoint.Transform(transform);
	Vector tmpVector(firstPoint, otherPoint);
	SetNormal(tmpVector);
	//Point On the Plane
	m_point.Transform(transform);

	return stat;
}

//-----------------------------------------------------------------------------

bool Plane::IsSegment3DIntersecting(const Segment3D &segment, Point3D& point)const
{
	// represent equation of plane in the form of
	// ax + bx + cz + d = 0

	// Segment is represented as p1 (x1,y1,z1) and p2 (x2,y2,z2) 
	// Segment = p1 + u (p2 - p1)
	// put two points in eqn of plane
	// a(x1 + u (x2 - x1)) + b(y1 + u (y2 - y1)) + c(z1 + u (z2 - z1)) + d = 0

	// solve for unknown u
	// u = numerator/denominator

	// numerator = a*x1 + b*y1 + c*z1 + d
	// denominator = a*(x1-x2) + b*(y1-y2) + c*(z1-z2)

	// if denominator is zero then segment is parallel to normal or
	// there are no solutions or the line is on the plane in which 
	// case are infinite solutions

	double a = m_normal.GetI();
	double b = m_normal.GetJ();
	double c = m_normal.GetK();

	double x = m_point.GetX();
	double y = m_point.GetY();
	double z = m_point.GetZ();

	double d = -(a*x + b*y + c*z);

	Point3D p1 = segment.GetStartPoint();
	Point3D p2 = segment.GetEndPoint();

	double x1 = p1.GetX();
	double y1 = p1.GetY();
	double z1 = p1.GetZ();

	double x2 = p2.GetX();
	double y2 = p2.GetY();
	double z2 = p2.GetZ();

	double numerator = a*x1 + b*y1 + c*z1 + d;
	double denominator = a*(x1 - x2) + b*(y1 - y2) + c*(z1 - z2);

	if (fabs(denominator) < CommonUtil::CommonConstants::ZERO)
		return false;

	double u = numerator / denominator;

	point = segment.GetPoint(u);
	return true;
}

//-----------------------------------------------------------------------------

bool Plane::IsPointOnPlane(const Point3D& point, double tol)const
{
	bool status = false;
	if (m_point.Distance(point) < CommonConstants::PNT_TOL)
		status = true;
	else
	{
		double distance = GetProjectedDistance(&point);
		if (distance < tol)
		{
			Vector tempAxis(m_point, point);
			tempAxis.Normalize();
			if (m_normal.IsPependicular(tempAxis, 0.0008))
			{
				status = true;
			}
		}
	}
	return status;
}

//-----------------------------------------------------------------------------
}