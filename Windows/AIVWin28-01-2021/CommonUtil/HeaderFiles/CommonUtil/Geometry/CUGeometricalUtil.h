// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_GEOMUTIL_H_
#define _COMMONUTIL_GEOMUTIL_H_

#include <vector>
#include "CUMatrix.h"

namespace CommonUtil
{

// Forward class declarations
class Plane;
class Point2D;
class Point3D;
class Vector;

// This class has utility functions
class GeometricalUtil
{
	/*static bool getGuessSkewIntersection(const BSpline* curve1, const BSpline* curve2,
									double curve2U1, double curve2U2,
									double &curve1Val, double &curve2Val);*/
public :

	//======================== public member functions ========================

	//**************************general methods********************************

	// The input plane is a plane inwhich the 3D point is lying. 
	// The input plane is transformed to XY plane and the new 2D
	// point is computed. The base point of the input plane is treated as the origin
	static Point2D Get2DPoint(const Plane *plane, const Point3D *point,
							  double *rotationAroundX = 0, double *rotationAroundY =0,
							  double *zValueOf2DPoint = 0);

	// Input is 3D data points
	// Output is Matrix& transMatrix of the local co-ordinate system
	//transMatrix (4,4)
	//	 _			   _
	//	|	xi yi zi tx |
	//	|	xj yj zj ty	|
	//	|	xk yk zk tz	|
	//	|	0   0  0  1	|
	//   -			   -
	// Local co-ordinate system : xAxis(xi,xj,xk), 
	//							  yAxis(yi,yj,yk),
	//							  zAxis(zi,zj,zk)
	static bool GetLocalCoOrdinateSystem(const std::vector<CommonUtil::Point3D> &points,
										 CommonUtil::Matrix& transMatrix);

	// this method translates the given points
	//		given input points must be planar
	//		transMatrix - current local co-ordinate system
	//		points - input points - will get tranlated in x,y plane
	static bool TranslatePointsInXYPlane(const CommonUtil::Matrix& transMatrix,
											std::vector<CommonUtil::Point3D> &points);

	static Point3D GetCentroid(const std::vector<CommonUtil::Point3D> &points);

	// Input :- Point3D point- point to rotated
	//			Vector axisDir- Axis direction
	//			bool revolveDir- direction of revolution
	//					e.g. suppose z axis is axisDir, 
	//					then revolveDir = 1 means x to y 
	//					revolveDir = 0 means y to x
	//			Point3D axisLocation- Axis location
	//			double revolveAngle- angle of revolution
	static Point3D GetPointAtRequiredAngle(const Point3D &point, const Vector& axisDir, 
									bool revolveDir, const Point3D &axisLocation, 
									double revolveAngle);

	/*static void findSkewIntersectionPoint(const BSpline* curve1,const BSpline* curve2,
										double uVal1, double uVal2, Surface* surface,
										Point3D& intersectionPt, double& intersectionPtUVal,
										double& distance);*/
};

}
#endif