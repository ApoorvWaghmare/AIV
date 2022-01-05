//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _COMMONUTIL_MATHUTIL_H_
#define _COMMONUTIL_MATHUTIL_H_

namespace CommonUtil
{

//forward declaration
class Plane;
class Point2D;
class Point3D;

//This class has utility functions
class MathUtil
{

public:

	//======================== public member functions ========================

	//**************************general methods********************************

	//Returns binomial coefficient.
	static int GetBinomialCoefficient(int n, int r);

	//Returns angle in degree
	static double GetAngleInDegree( const double &angleInRadians );

	//Returns angle in radians
	static double GetAngleInRadians( const double &angleInDegree );

	//Solves three simultaneous linear eq by Cramer's rule
	static bool SolveThreeLinearEquationsByCramersRule(const double &a1,
		const double &b1, const double &c1, const double &a2, const double &b2,
		const double &c2, const double &a3, const double &b3, const double &c3,
		const double &d1, const double &d2, const double &d3,
		double &x, double &y, double &z);

	//Solves two simultaneous linear eq by Cramer's rule
	static bool SolveTwoLinearEquationsByCramersRule(double a1, double a2, double a3,
												 double b1, double b2, double b3,
												 double &x, double &y);

	//This method gives radius from 3D three points.
	// If return false - three points are co-linear
	static bool GetRadius(double x1, double y1, double z1,
						  double x2, double y2, double z2,
						  double x3, double y3, double z3,
						    			  double &radius);

	static bool GetCentre(const CommonUtil::Point2D &pt1, const CommonUtil::Point2D &pt2,
						 const CommonUtil::Point2D &pt3, CommonUtil::Point2D &centre);

};


class CmpDoubleAscend
{
public:
	bool operator() (double a, double b)
	{
		return a < b;
	}
};

class CmpDoubleDecend
{
public:
	bool operator() (double a, double b)
	{
		return a > b;
	}
};

}
#endif