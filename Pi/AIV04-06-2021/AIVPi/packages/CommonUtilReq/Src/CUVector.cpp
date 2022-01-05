// ============================================================================
//			Copyright (c) 2007 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//math include files
#include "CUVector.h"
#include "CUPoint2D.h"
#include "CUMatrix.h"
#include "CUSqMatrix.h"
#include "CUTransform3D.h"
#include "CUPoint3DF.h"

//util include files
//#include "CUReport.h"
//#include "SketcherConstants.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

bool Vector::Normalize(double tol)
{
	bool status = false;

	double magSqr = GetSqrMagnitude();

	if (magSqr > tol*tol)
	{
		double magInv = 1.0 / sqrt(magSqr);
		m_i *= magInv;
		m_j *= magInv;
		m_k *= magInv;
		status = true;
	}
	return status;
}

//-----------------------------------------------------------------------------

double Vector::GetAngle(const Vector &inputVector)const
{
	double magSqr = GetSqrMagnitude();
	double  magSqr1 = inputVector.GetSqrMagnitude();
		
	double angle = DBL_MAX;
	if (magSqr > CommonConstants::PNT_TOL*CommonConstants::PNT_TOL && magSqr1 >
		CommonConstants::PNT_TOL*CommonConstants::PNT_TOL)
	{
		double dotProd = DotProduct(inputVector);

		double val = dotProd/sqrt(magSqr * magSqr1);
	
		if (val < -1 + CommonConstants::ZERO)
			angle = CommonConstants::PI;
		else if (val > 1 - CommonConstants::ZERO)
			angle = 0.;
		else
			angle = acos (val);
	}
	return angle;
}

//-----------------------------------------------------------------------------

bool Vector::GetAngle(const Vector &v1, double &angle)const
{
	bool status = false;
	
	double magSqr = GetSqrMagnitude();
	double  magSqr1 = v1.GetSqrMagnitude();
		
	if (magSqr > CommonConstants::PNT_TOL*CommonConstants::PNT_TOL && magSqr1 >
		CommonConstants::PNT_TOL*CommonConstants::PNT_TOL)
	{
		double dotProd = DotProduct(v1);

		double val = dotProd/sqrt(magSqr * magSqr1);
	
		if (val < -1 + CommonConstants::ZERO)
			angle = CommonConstants::PI;
		else if (val > 1 - CommonConstants::ZERO)
			angle = 0.;
		else
		angle = acos (val);

		status = true;
	}
	return status;
}

//-----------------------------------------------------------------------------

double Vector::GetCosineAngle(const Vector &v1)const
{
	double angle = -1.;

	double magSqr = GetSqrMagnitude();
	double magSqr1 = v1.GetSqrMagnitude();

	if (magSqr > CommonUtil::CommonConstants::SQR_PNT_TOL && magSqr1 > CommonUtil::CommonConstants::SQR_PNT_TOL)
	{
		double dotProd = DotProduct(v1);
		double val = dotProd / sqrt(magSqr * magSqr1);

		if (val < -1 + CommonUtil::CommonConstants::PNT_TOL)
			angle = cos(CommonUtil::CommonConstants::PI);
		else if (val > 1 - CommonUtil::CommonConstants::PNT_TOL)
			angle = cos(0.);
		else
			angle = val;
	}
	return angle;
}

//-----------------------------------------------------------------------------

void Vector::Transform(const Transform3D &transform)
{
	Matrix matrix(1, 4);
	matrix(0, 0) = m_i;
	matrix(1, 0) = m_j;
	matrix(2, 0) = m_k;
	matrix(3, 0) = 0;

	SqMatrix transformationMatrix = transform.GetTransformationMatrix();

	/*size_t rowSize = transformationMatrix.GetNumRows();
	Matrix tmpMatrix(rowSize, rowSize);

	for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < rowSize; j++)
		{
			tmpMatrix(i, j) = transformationMatrix(i, j);
		}
	}*/

	//matrix.MultiplyTo(tmpMatrix);
	Matrix result;
	transformationMatrix.Multiply(matrix, result);

	m_i = (float)result(0, 0);
	m_j = (float)result(1, 0);
	m_k = (float)result(2, 0);
}

//-----------------------------------------------------------------------------

bool Vector::IsParallel(const Vector &v1, Direction &dir, double angTol)const
{
#ifdef _DEBUG
	PS_VERIFY (fabs(v1.GetI()) > CommonConstants::ZERO || 
				fabs(v1.GetJ()) > CommonConstants::ZERO || 
					fabs(v1.GetK()) > CommonConstants::ZERO);

	PS_VERIFY (fabs(m_i) > CommonConstants::ZERO || 
				fabs(m_j) > CommonConstants::ZERO || 
					fabs(m_k) > CommonConstants::ZERO);
#endif // _DEBUG

	bool status = false;

	double dotProd = DotProduct(v1);

	double angle;
	GetAngle(v1, angle);
	
	dir = DIFFERENT_DIRECTION;
	if (fabs(angle) <= angTol || fabs(CommonConstants::PI - angle) <= angTol)
	{
		status = true;
		if (dotProd < 0.)
			dir = OPPOSITE_DIRECTION;
		else
			dir = SAME_DIRECTION;
	}
	
	return status;
}

//-----------------------------------------------------------------------------

bool Vector::IsPependicular(const Vector &v1, double angTol)const
{
#ifdef _DEBUG
	PS_VERIFY (fabs(v1.GetI()) > CommonConstants::ZERO || 
				fabs(v1.GetJ()) > CommonConstants::ZERO ||
					fabs(v1.GetK()) > CommonConstants::ZERO);

	PS_VERIFY (fabs(m_i) > CommonConstants::ZERO || 
				fabs(m_j) > CommonConstants::ZERO || 
					fabs(m_k) > CommonConstants::ZERO);
#endif // _DEBUG

	double angle;
	bool status = GetAngle(v1, angle);
	
	if (status)
	{
		double fangle = fabs(angle);

		if (fangle < (CommonConstants::PI / 2. + angTol) &&
			fangle >(CommonConstants::PI / 2. - angTol))
			status = true;
		else
			status = false;
	}
	return status;
}

//-----------------------------------------------------------------------------

Vector Vector::GetOtherVectorFromCrossProd(const Vector &crossProduct, double angle,
										   bool &vectorObtained)const
{

#ifdef DEBUG_MODE

#ifdef _DEBUG
	PS_VERIFY (fabs(crossProduct.GetI()) > CommonConstants::ZERO ||
		fabs(crossProduct.GetJ()) > CommonConstants::ZERO ||
		fabs(crossProduct.GetK()) > CommonConstants::ZERO);

	PS_VERIFY (fabs(m_i) > CommonConstants::ZERO ||
		fabs(m_j) > CommonConstants::ZERO ||
		fabs(m_k) > CommonConstants::ZERO);
#endif // _DEBUG

#endif

	Vector tmpCrossProduct = crossProduct;	//Copied to remove CppCheck warning

	vectorObtained = false;

	// cross product is computed
	tmpCrossProduct.Normalize();
	double multFactor = GetSqrMagnitude() * sin(angle);
	tmpCrossProduct.ScalarMultiply(multFactor);

	Vector otherVector;
	Point3D basePoint;
	
	double a1,a2,a3;
	double b1,b2,b3;
	double n1,n2,n3;
	
	n1 = tmpCrossProduct.GetI();
	n2 = tmpCrossProduct.GetJ();
	n3 = tmpCrossProduct.GetK();

	a1 = GetI();
	a2 = GetJ();
	a3 = GetK();
	Vector movedVec;
	bool vectorSet = false;

	if (fabs(a1) > CommonConstants::ZERO)
	{
		double k1,k2,k3;
		k1 = a1*a1 + a2*a2 + a3*a3;
		k2 = 2*(a2*n3 - n2*a3);
		k3 = n3*n3 + n2*n2 - a1*a1*k1;
		double roots[2];
		double deltaTerm = k2*k2 - 4*k1*k3;
		if (deltaTerm > -CommonConstants::ZERO)
		{
			if(deltaTerm < 0)
				deltaTerm = 0;

			roots[0] =  ( -k2 + sqrt( (deltaTerm) ) )/ (2* k1);
			roots[1] =  ( -k2 - sqrt( (deltaTerm) ) )/ (2* k1);
			for(int i = 0; i < 2; i++)
			{
				b1 = roots[i];
				b2 = (n3 + a2*b1)/(a1);
				b3 = (a3*b1 - n2)/(a1);
				movedVec.Set(b1, b2, b3);
				double tempAngle;
				movedVec.GetAngle(*this, tempAngle);
				if(fabs(tempAngle- angle) < 0.1)
				{
					vectorSet = true;
					break;
				}
			}
		}
	}

	if (fabs(a2) > CommonConstants::ZERO && !vectorSet)
	{
		double k1,k2,k3;
		k1 = a1*a1 + a2*a2 + a3*a3;
		k2 = 2*(a3*n1 - n3*a1);
		k3 = n1*n1 + n3*n3 - a2*a2*k1;
		double roots[2];
		double deltaTerm = k2*k2 - 4*k1*k3;
		if (deltaTerm > -CommonConstants::ZERO)
		{
			if(deltaTerm < 0)
				deltaTerm = 0;

			roots[0] =  ( -k2 + sqrt(deltaTerm) )/ (2* k1);
			roots[1] =  ( -k2 - sqrt(deltaTerm) )/ (2* k1);
			for(int i = 0; i < 2; i++)
			{
				b2 = roots[i];
				b3 = (n1 + a3*b2)/(a2);
				b1 = (a1*b2 - n3)/(a2);
				movedVec.Set(b1, b2, b3);
				double tempAngle = movedVec.GetAngle(*this);
				if(fabs(tempAngle- angle) < 0.1)
				{
					vectorSet = true;
					break;
				}
			}
		}
	}

	if (fabs(a3) > CommonConstants::ZERO && !vectorSet)
	{

		double k1,k2,k3;
		k1 = a1*a1 + a2*a2 + a3*a3;
		k2 = 2*(a1*n2 - n1*a2);
		k3 = n1*n1 + n2*n2 - a3*a3*k1;
		double roots[2];
		double deltaTerm = k2*k2 - 4*k1*k3;
		if (deltaTerm > -CommonConstants::ZERO)
		{
			if(deltaTerm < 0)
				deltaTerm = 0;

			roots[0] =  ( -k2 + sqrt(deltaTerm) )/ (2* k1);
			roots[1] =  ( -k2 - sqrt(deltaTerm) )/ (2* k1);
			for(int i = 0; i < 2; i++)
			{
				b3 = roots[i];
				b1 = (n2 + a1*b3)/(a3);
				b2 = (a2*b3 - n1)/(a3);
				movedVec.Set(b1, b2, b3);
				double tempAngle = movedVec.GetAngle(*this);
				if(fabs(tempAngle- angle) < 0.1)
				{
					vectorSet = true;
					break;
				}
			}
		}
	}
	if(vectorSet)
	{
		vectorObtained = true;
		otherVector = movedVec;
	}
	return otherVector;
}

//-----------------------------------------------------------------------------

void Vector::Dump(Report &report, int tabIndent)const
{
	//report.AddMarker(tabIndent);
	
	//define***

	/*report.Add(tabIndent, "Vector: ");
	report.Add(tabIndent, "m_i, m_j, m_k : ");
	report.Add(0, "%15.6f, %15.6f, %15.6f\n", m_i, m_j, m_k);*/

	//define***

	//report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

void Vector::GetPointAlongVector(const Point3D &basePoint, Point3D &point,
							double distance)const
{
#ifdef _DEBUG
	PS_VERIFY(IsValid());
#endif // _DEBUG
	double ratio = 0.0;
    const double magnitudeSqr = GetSqrMagnitude();
	if (fabs(magnitudeSqr - 1.0) < CommonConstants::ZERO*CommonConstants::ZERO)
    {
        // already normalized.
        ratio = distance;
    }
    else
    {
	    double magnitude = sqrt( magnitudeSqr );
	
		if (magnitude > CommonConstants::ZERO)
	    	ratio = distance/magnitude;
    }
	// get the x,y,z values of the point along the vector
	double x = basePoint.GetX() + ratio * GetI();
	double y = basePoint.GetY() + ratio * GetJ();
	double z = basePoint.GetZ() + ratio * GetK();

	// set the values
	point.Set(x,y,z);
}


//-----------------------------------------------------------------------------

void Vector::GetPointAlongVector(const Point3DF &basePoint, Point3DF &point, double distance)const
{
	Point3D p;
	CommonUtil::Point3D p2(basePoint.GetX(), basePoint.GetY(), basePoint.GetZ());
	GetPointAlongVector(p2, p, distance);
	point.Set(p.GetX(), p.GetY(), p.GetZ());
}
//-----------------------------------------------------------------------------

void Vector::GetPointAlongVector(const Point2D &basePoint, Point2D &point,
							 double distance)const
{
	Point3D basept_3D(basePoint.GetX(),basePoint.GetY(),0.);
	Point3D point_3D;
	GetPointAlongVector(basept_3D, point_3D, distance);
	point.Set(point_3D.GetX(), point_3D.GetY());
}

//-----------------------------------------------------------------------------

bool Vector::IsPointAlongVector(const Point2D &basePoint, const Point2D &point)const 
{
	bool stat = false;
	Vector vec(basePoint, point);
	Direction dirOfVec;

	//define***

	/*if(IsParallel(vec, dirOfVec, SketcherConstants::ANGLE_TOL))
	{
		if(dirOfVec == SAME_DIRECTION)
			stat = true;
	}*/

	//define***

	return stat;
}

//---------------------------------------------------------------------------------------

}