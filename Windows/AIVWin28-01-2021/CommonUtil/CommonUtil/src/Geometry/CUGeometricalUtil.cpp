// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// util include files
#include "CUGeometricalUtil.h"
#include "CUPoint2D.h"
#include "CUVector.h"
#include "CUPlane.h"
#include "CUSqMatrix.h"
#include "CULUDecomposition.h"
#include "CUSegment3D.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------

Point2D GeometricalUtil::Get2DPoint(const Plane *plane, const Point3D *point, double *rotationAroundX,
						 double *rotationAroundY, double *zValueOf2DPoint) 
{
	const Vector& normal = plane->GetNormal();
	double x = normal.GetI();
	double y = normal.GetJ();
	double z = normal.GetK();

	Vector pointVectorInYZPlane(0,y,z);
	Vector ZVector(0,0,1);

	Point3D tempPoint(point->GetX(), point->GetY(), point->GetZ());
	
	if(pointVectorInYZPlane.Normalize())
	{
		double thetaX;
		if(pointVectorInYZPlane.GetAngle(ZVector,thetaX))
		{
			Vector prod;
			pointVectorInYZPlane.CrossProduct(ZVector,prod);
			if(prod.GetI() < 0)
			{
				thetaX = -thetaX;
			}

			double costhetaX = cos(thetaX);
			double sinthetaX = sin(thetaX);

			SqMatrix RotateX(3,SqMatrix::IDENTITY);
			RotateX(1,1) = costhetaX;
			RotateX(1,2) = sinthetaX;
			RotateX(2,1) = -sinthetaX;
			RotateX(2,2) = costhetaX;

			std::vector<double> translation(3);
			translation[0] = 0;
			translation[1] = 0;
			translation[2] = 0;

			tempPoint.Transform(RotateX,translation);
		}
	}

	z = sqrt(z*z + y*y);
	Vector pointVectorInXZPlane(x,0,z);

	if(pointVectorInXZPlane.Normalize())
	{
		double thetaY;
		if(ZVector.GetAngle(pointVectorInXZPlane,thetaY))
		{
			Vector prod;
			pointVectorInXZPlane.CrossProduct(ZVector,prod);
			if(prod.GetJ() < 0)
			{
				thetaY = -thetaY;
			}

			double costhetaY = cos(thetaY);
			double sinthetaY = sin(thetaY);

	
			SqMatrix RotateY(3,SqMatrix::IDENTITY);
			RotateY(0,0) = costhetaY;
			RotateY(0,2) = -sinthetaY;
			RotateY(2,0) = sinthetaY;
			RotateY(2,2) = costhetaY;
	
			std::vector<double> translation(3);
			translation[0] = 0;
			translation[1] = 0;
			translation[2] = 0;

			tempPoint.Transform(RotateY,translation);
		}
	}

	return Point2D(tempPoint.GetX(),tempPoint.GetY());
}

//------------------------------------------------------------------------------------

bool GeometricalUtil::GetLocalCoOrdinateSystem(const std::vector<CommonUtil::Point3D> &points,
										Matrix& transMatrix)
{
	// the centroid of all the points is calculated
	size_t n = points.size();
	double x = 0, y = 0, z = 0;
	for(int i = 0; i < n; ++i)
	{
		x += points[i].GetX();
		y += points[i].GetY();
		z += points[i].GetZ();
	}
	
	x /= n;
	y /= n;
	z /= n;

	Point3D centroid(x, y, z);

	// compute the covariance matrix
	SqMatrix covariance(3); 
	for(int i = 0; i < n; ++i)
	{
		SqMatrix temp(3), temp1(3), temp2(3);
	
		temp2(0,0) = points[i].GetX() - centroid.GetX();
		temp2(0,1) = points[i].GetY() - centroid.GetY();
		temp2(0,2) = points[i].GetZ() - centroid.GetZ();
		
		temp2.GetTranspose(temp1);
		temp1.Multiply(temp2, temp);
		covariance.AddTo(temp);
	}

	Vector axis[3];
	double eigenValues[3] = {0.0,0.0,0.0};
	Vector eigenVectors[3];
	covariance.GetEigenInfo(eigenValues, eigenVectors);

	double minVal = eigenValues[0];
	int minIndex = 0;
	if(minVal > eigenValues[1])
	{
		minVal = eigenValues[1];
		minIndex = 1;
	}
	
	if(minVal > eigenValues[2])
	{
		minVal = eigenValues[2];
		minIndex = 2;
	}

	int maxIndex = 0;
	double maxVal = eigenValues[0];
	if(maxVal < eigenValues[1])
	{
		maxVal = eigenValues[1];
		maxIndex = 1;
	}
	
	if(maxVal < eigenValues[2])
	{
		maxVal = eigenValues[2];
		maxIndex = 2;
	}
    
	int midIndex = 2;
	if((minIndex == 1 && maxIndex == 2) || (minIndex == 2 && maxIndex == 1))
		midIndex = 0;
	
	if((minIndex == 0 && maxIndex == 2) || (minIndex == 2 && maxIndex == 0))
		midIndex = 1;

	// x axis
	axis[0] =  eigenVectors[maxIndex];
	// y axis
	axis[1] =  eigenVectors[midIndex];
	// z axis
	axis[2] =  eigenVectors[minIndex];

	// added om 11 aug 06
	if(fabs(eigenValues[0]) < CommonConstants::PNT_TOL &&
		fabs(eigenValues[1]) < CommonConstants::PNT_TOL &&
		fabs(eigenValues[2]) < CommonConstants::PNT_TOL)
	{
		axis[0].Set(1,0,0);
		axis[1].Set(0,1,0);
		axis[2].Set(0,0,1);
	}

	transMatrix.Resize(4,4);
	
	//	 _			   _
	//	|	xi yi zi tx |
	//	|	xj yj zj ty	|
	//	|	xk yk zk tz	|
	//	|	0   0  0  1	|
	//   -			   -

	Vector zAxis;
	axis[0].CrossProduct(axis[1], zAxis);
	double dotProd = zAxis.DotProduct(axis[2]);
	if(dotProd < 0.0)
	{
		Vector xAxis = axis[0];
		axis[0] = axis[1];
		axis[1] = xAxis;
	}

	// set x axis
	transMatrix(0, 0) = axis[0].GetI();
	transMatrix(1, 0) = axis[0].GetJ();
	transMatrix(2, 0) = axis[0].GetK();

	// set y axis
	transMatrix(0, 1) = axis[1].GetI();
	transMatrix(1, 1) = axis[1].GetJ();
	transMatrix(2, 1) = axis[1].GetK();

	// set z axis
	transMatrix(0, 2) = axis[2].GetI();
	transMatrix(1, 2) = axis[2].GetJ();
	transMatrix(2, 2) = axis[2].GetK();

	transMatrix(0, 3) = centroid.GetX();
	transMatrix(1, 3) = centroid.GetY();
	transMatrix(2, 3) = centroid.GetZ();

	transMatrix(3, 3) = 1;

	return true;
}

//------------------------------------------------------------------------------------

bool GeometricalUtil::TranslatePointsInXYPlane(const Matrix& transMatrix,
										std::vector<CommonUtil::Point3D> &points)
{
	bool inXYPlane = false;
	if(fabs(transMatrix(0,2)) < CommonConstants::ZERO &&
		fabs(transMatrix(1,2)) < CommonConstants::ZERO &&
		(fabs(transMatrix(2,2))-1) < CommonConstants::ZERO)
	{
		double z = points[0].GetZ();
		if(fabs(z) < CommonConstants::ZERO)
            inXYPlane = true;
	}

	if(!inXYPlane)
	{
		Matrix inverseTrans;
		Matrix dummyMatrix(1, transMatrix.GetNumRows()); 
		LUDecomposition solver(transMatrix, dummyMatrix);
		if(!solver.GetInverse(inverseTrans))
			return false;

		for(int i = 0; i < points.size(); i++)
			points[i].Transformation(inverseTrans);
	}

	return true;
}

//------------------------------------------------------------------------------------

Point3D GeometricalUtil::GetCentroid(const std::vector<CommonUtil::Point3D> &points)
{
	size_t n = points.size();
	double x = 0, y = 0, z = 0;
	for(int i = 0; i < n; i++)
	{
		x += points[i].GetX();
		y += points[i].GetY();
		z += points[i].GetZ();
	}
	if(n)
	{
		x /= n;
		y /= n;
		z /= n;
	}

	return Point3D(x, y, z);
}

//------------------------------------------------------------------------------------

Point3D GeometricalUtil::GetPointAtRequiredAngle(const Point3D &point, const Vector& axisDir,
									bool revolveDir, const Point3D &axisLocation, 
									double revolveAngle)
{
	//get segment of the axis dir
	Point3D pointOnAxis;
	axisDir.GetPointAlongVector(axisLocation, pointOnAxis, 10);
	Segment3D segment(axisLocation, pointOnAxis);

	//project point on above seg
	double uVal;
	segment.Project(point, 0, uVal);
	Point3D projPoint = segment.GetPoint (uVal);

	//get vector vertex and proj point
	Vector vertexVector(projPoint, point);
	vertexVector.Normalize(0.0);
		
	//using the vector and axis dir get teh rotated vector
	double radius = point.Distance(projPoint);
	
	if(radius < CommonConstants::PNT_TOL)
		return point;

	bool stat = false;
	Vector axisVector = axisDir;
	axisVector.Normalize(0.0);
	if(!revolveDir)
		axisVector.Reverse();
	Vector rotatedVector = vertexVector.GetOtherVectorFromCrossProd(axisVector, revolveAngle, stat);  
	
	Point3D newVertex;
	if(stat)
		rotatedVector.GetPointAlongVector(projPoint, newVertex, radius);

	return newVertex;
}

//------------------------------------------------------------------------------------

/*void GeomUtil::findSkewIntersectionPoint(
									const BSpline* curve1,const BSpline* curve2,
									double uVal1, double uVal2, Surface* surface,
									Point3D& intersectionPt, double& intersectionPtUVal,
									double& distance)
{
	double curve1Val = 0.0;
	bool guessComputed = getGuessSkewIntersection(curve1, curve2, uVal1, uVal2, curve1Val, intersectionPtUVal);
	if(!guessComputed) // can happen when both curves are parallel
		intersectionPtUVal = (uVal1+uVal2)/2.0;

	int ctr = 0;
	Point3D ptOnCurve1;
	do
	{
		intersectionPt = curve2->GetPoint(intersectionPtUVal);
		double newVal = curve1->Project(intersectionPt);
		ptOnCurve1 = curve1->GetPoint(newVal);

		// proejct this point on curve2
		double guessVal = intersectionPtUVal;
		intersectionPtUVal = curve2->Project(ptOnCurve1, &guessVal);
		if(intersectionPtUVal < uVal1)
		{
			intersectionPtUVal = uVal1;
			break;
		}
		else if(intersectionPtUVal > uVal2)
		{
			intersectionPtUVal = uVal2;
			break;
		}

		ctr++;
	}while(ctr < 100);
	intersectionPt = curve2->GetPoint(intersectionPtUVal);

	// project point on gCurve (intersectionPt) on surface
	double uVal, vVal;
	surface->ProjectPoint(intersectionPt,uVal, vVal);
	Point3D pointOnSurf = surface->GetPoint(uVal, vVal);
	double uValOnSeam = curve1->Project(pointOnSurf);
	Point3D pointOnSeam = curve1->GetPoint(uValOnSeam);
	// shortest dist between point on surface and seam curve
	distance = pointOnSurf.Distance(pointOnSeam);
}

//------------------------------------------------------------------------------------

bool GeomUtil::getGuessSkewIntersection(const BSpline* curve1, const BSpline* curve2,
												double curve2U1, double curve2U2,
												double &curve1Val, double &curve2Val)
{
	bool guessComputed = false;
	std::vector<CommonUtil::Point3D> curve1Points;
	std::vector<CommonUtil::Point3D> curve2Points;

	std::vector<double> curve1Vals;
	std::vector<double> curve2Vals;

	// get 5 points on curve2
	size_t numPts = 5;
	curve1Points.reserve(numPts);
	curve2Points.reserve(numPts);
	curve1Vals.reserve(numPts);
	curve2Vals.reserve(numPts);

	double curve2UIncrement = (curve2U2-curve2U1)/(numPts-1);
	for(size_t iPt=0; iPt<numPts; ++iPt)
	{
		double tmpVal = curve2U1 + iPt*curve2UIncrement;
		if(iPt == numPts-1)
			tmpVal = curve2U2;
		Point3D curve2Pt = curve2->GetPoint(tmpVal);
		curve2Points.push_back(curve2Pt);
		curve2Vals.push_back(tmpVal);
	}

	// identify curve1 range
	// project curve2Points on curve1
	// get min UVal and max UVal (curve1U1, curve1U2)
	double curve1U1 = DBL_MAX, curve1U2 = -DBL_MAX;
	for(size_t iPt=0; iPt<numPts; ++iPt)
	{
		double tmpVal = curve1->Project(curve2Points[iPt]);

		if(tmpVal < curve1U1)
			curve1U1 = tmpVal;

		if(tmpVal > curve1U2)
			curve1U2 = tmpVal;
	}

	// get 5 points on curve1 using curve1U1, curve1U2
	double curve1UIncrement = (curve1U2-curve1U1)/(numPts-1);
	for(size_t iPt=0; iPt<numPts; ++iPt)
	{
		double tmpVal = curve1U1 + iPt*curve1UIncrement;
		if(iPt == numPts-1)
			tmpVal = curve1U2;
		Point3D curve1Pt = curve1->GetPoint(tmpVal);
		curve1Points.push_back(curve1Pt);
		curve1Vals.push_back(tmpVal);
	}

	// for loop (first curve points)
	double minSqrDist = DBL_MAX;
	Segment3D curve1Seg, curve2Seg;
	for(size_t iCurve1Pt = 0; iCurve1Pt < curve1Points.size()-1; ++iCurve1Pt)
	{
		// segment of curve1
		curve1Seg.Set(curve1Points[iCurve1Pt], curve1Points[iCurve1Pt+1]);
		double u1OnCurve1 = curve1Vals[iCurve1Pt];
		double u2OnCurve1 = curve1Vals[iCurve1Pt+1];
		double spanOnCurve1 = u2OnCurve1-u1OnCurve1;
		
		for(size_t iCurve2Pt = 0; iCurve2Pt < curve2Points.size()-1; ++iCurve2Pt)
		{
			curve2Seg.Set(curve2Points[iCurve2Pt], curve2Points[iCurve2Pt+1]);
			double u1OnCurve2 = curve2Vals[iCurve2Pt];
			double u2OnCurve2 = curve2Vals[iCurve2Pt+1];	
			double spanOnCurve2 = u2OnCurve2-u1OnCurve2;			

			double u = -1.0,  v = -1.0;
			double skewDistTol = CommonConstants::PNT_TOL;
			double uSnappingFactor = 0.0;
			double vSnappingFactor = 0.0;
			double angTol = 1e-3;

			Segment3D::ParallelInterSectionInfo parallelInfo;
			bool status = curve1Seg.GetSkewIntersection(curve2Seg, u, v, skewDistTol, uSnappingFactor, vSnappingFactor,
									 angTol, &parallelInfo);

			if(status && !parallelInfo.m_collinear)
			{
				Point3D pointOnSeg1 = curve1Seg.GetPoint(u);
				Point3D pointOnSeg2 = curve2Seg.GetPoint(v);
				double sqrDist = pointOnSeg1.SqrDistance(pointOnSeg2);
				if(sqrDist < minSqrDist)
				{
					minSqrDist = sqrDist;

					// linear interpolate
					curve1Val = u1OnCurve1 + spanOnCurve1*u;
					curve2Val = u1OnCurve2 + spanOnCurve2*v;

					guessComputed = true;
				}
			}
		}
	}

	return guessComputed;
}*/

//------------------------------------------------------------------------------------

}