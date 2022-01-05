// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <math.h>

//math include files
#include "CUMathUtil.h"
#include "CUCommonConstants.h"
#include "CUPoint2D.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

int MathUtil::GetBinomialCoefficient(int n, int r)
{
	int value = 0;

	if (n >= r)
	{
		r = ((r < n-r) ? r : n-r);

		if (r == 0)
		{
			value = 1;
		}
		else
		{
			int numerator = 1;
			int denominator = 1;
		
			for( ; r; r--, n--)
			{
				numerator *= n;
				denominator *= r;
			}
			value = numerator/denominator;
		}
	}
	return value;
}


//-----------------------------------------------------------------------------

double MathUtil::GetAngleInDegree( const double &angleInRadians )
{
	return angleInRadians * 180.0/CommonUtil::CommonConstants::PI;
}

//-----------------------------------------------------------------------------

double MathUtil::GetAngleInRadians( const double &angleInDegree )
{
	return angleInDegree * CommonUtil::CommonConstants::PI / 180.0;
}

//-----------------------------------------------------------------------------

bool MathUtil::SolveThreeLinearEquationsByCramersRule(const double &a1,
		const double &b1, const double &c1, const double &a2, const double &b2,
		const double &c2, const double &a3, const double &b3, const double &c3,
		const double &d1, const double &d2, const double &d3,
		double &x, double &y, double &z)
{
	double denom = a1*(b2*c3 - b3*c2)
					- b1*(a2*c3 - a3*c2)
					+ c1*(a2*b3 - a3*b2);
	if(fabs(denom) < CommonUtil::CommonConstants::ZERO)
		return false;

	x = (d1*(b2*c3 - b3*c2) - b1*(d2*c3 - d3*c2) + c1*(d2*b3 - d3*b2))/denom;
	y = (a1*(d2*c3 - d3*c2) - d1*(a2*c3 - a3*c2) + c1*(a2*d3 - a3*d2))/denom;
	z = (a1*(b2*d3 - b3*d2) - b1*(a2*d3 - a3*d2) + d1*(a2*b3 - a3*b2))/denom;

	return true;
}

//-----------------------------------------------------------------------------

bool MathUtil::SolveTwoLinearEquationsByCramersRule(double a1, double a2, double a3,
												 double b1, double b2, double b3,
												 double &x, double &y)
{
	//a1 y + a2 z + a3 = 0
	//b1 y + b2 z + b3 = 0

	double denom = a1*b2 - b1*a2;
	if(fabs(denom) < CommonUtil::CommonConstants::ZERO)
		return false;

	x = (a2*b3 - b2*a3)/denom;
	y = -(a1*b3 - b1*a3)/denom;

	return true;
}

//-----------------------------------------------------------------------------

bool MathUtil::GetRadius(double x1, double y1, double z1,double x2, double y2, double z2,
						                  double x3, double y3, double z3,double &radius)
{
	double a = sqrt((x1 - x2) * (x1 - x2) +
					(y1 - y2) * (y1 - y2) +
					(z1 - z2) * (z1 - z2));

	double b = sqrt((x1 - x3) * (x1 - x3) +
					(y1 - y3) * (y1 - y3) +
					(z1 - z3) * (z1 - z3));

	double c = sqrt((x3 - x2) * (x3 - x2) +
					(y3 - y2) * (y3 - y2) +
					(z3 - z2) * (z3 - z2));
	double numerator = a*b*c;
	double denomirator = sqrt(2*a*a*b*b + 2*b*b*c*c + 2*c*c*a*a-a*a*a*a-b*b*b*b-c*c*c*c);
	if(denomirator == 0.0)
		return false;
	radius = numerator / denomirator;

	return true;
}
//-----------------------------------------------------------------------------

bool MathUtil::GetCentre(const CommonUtil::Point2D &pt1, const CommonUtil::Point2D &pt2,
						 const CommonUtil::Point2D &pt3, CommonUtil::Point2D &centre)
{
	double x1 = pt1.GetX();
	double x2 = pt2.GetX();
	double x3 = pt3.GetX();

	double y1 = pt1.GetY();
	double y2 = pt2.GetY();
	double y3 = pt3.GetY();

	double s = 0.5*((x2 - x3)*(x1 - x3) - (y2 - y3)*(y3 - y1));
	double sUnder = (x1 - x2)*(y3 - y1) - (y2 - y1)*(x1 - x3);

	if(sUnder == 0)
		return false; //insufficient data to calculate center

	s /= sUnder;

	double xc = 0.5*(x1 + x2) + s*(y2 - y1); // center x coordinate
	double yc = 0.5*(y1 + y2) + s*(x1 - x2); // center y coordinate
	centre.Set(xc, yc);
	return true;
}
//-----------------------------------------------------------------------------

}