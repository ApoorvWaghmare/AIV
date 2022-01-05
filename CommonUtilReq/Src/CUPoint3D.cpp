// ============================================================================
//			Copyright (c) 2007 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//geom include files
#include "CUPoint2D.h"
#include "CUTransform3D.h"
#include "CUVector.h"
#include "CUPoint3DF.h"
//util include files
#include "CUReport.h"


namespace CommonUtil
{

//-----------------------------------------------------------------------------

Point3D::Point3D(const Point2D* point):m_x(0.),m_y(0.),m_z(0.)
{
	if(point)
	{
		m_x = point->GetX();
		m_y = point->GetY();
		m_z = 0.0;
	}
}
//-----------------------------------------------------------------------------

Point3D::Point3D(double radius, double theta)
{
    double x = radius * cos(theta);
    double y = radius * sin(theta);

    SetX(x);
    SetY(y);
    SetZ(0);
}
bool Point3D::IsEqual(const Point3D &point, double tolerance)const
{
	bool stat = false;
	double sqrDist = SqrDistance(point);
	if (sqrDist < tolerance*tolerance)
		stat = true;
	return stat;
}

//-----------------------------------------------------------------------------

void Point3D::Transform(const Transform3D &transform)
{

	Matrix matrix(4, 1);
	matrix(0, 0) = m_x;
	matrix(0, 1) = m_y;
	matrix(0, 2) = m_z;
	matrix(0, 3) = 1;

	const SqMatrix &transformationMatrix = transform.GetTransformationMatrix();

	matrix.MultiplyTo(transformationMatrix);

	m_x = matrix(0, 0);
	m_y = matrix(0, 1);
	m_z = matrix(0, 2);

}

//-----------------------------------------------------------------------------

void Point3D::Transform(const SqMatrix &Rotation, const std::vector<double> &translation)
{
	double x =  Rotation(0,0) * GetX() + Rotation(0,1) * GetY() +
			Rotation(0,2) * GetZ() + translation[0];

	double y =  Rotation(1,0) * GetX() + Rotation(1,1) * GetY() +
			Rotation(1,2) * GetZ() + translation[1];

	double z =  Rotation(2,0) * GetX() + Rotation(2,1) * GetY() +
			Rotation(2,2) * GetZ() + translation[2];

	Set(x, y, z);
}

//-----------------------------------------------------------------------------

void Point3D::Transformation(const Matrix &transMatrix)
{
	Matrix pointMatrix(1, 4);
	pointMatrix(0, 0) = m_x;
	pointMatrix(1, 0) = m_y;
	pointMatrix(2, 0) = m_z;
	pointMatrix(3, 0) = 1.;

	Matrix result;
	transMatrix.Multiply(pointMatrix, result);
	m_x = result(0, 0);
	m_y = result(1, 0);
	m_z = result(2, 0);
}

void Point3D::ApplyTransformation(const Matrix &matrix)
{
	std::vector<double> values;
	values.push_back(m_x);
	values.push_back(m_y);
	values.push_back(m_z);
	values.push_back(1);

	Matrix localmatrix(4, values);
	localmatrix.MultiplyTo(matrix);

	m_x = localmatrix(0, 0);
	m_y = localmatrix(0, 1);
	m_z = localmatrix(0, 2);
}
//-----------------------------------------------------------------------------

void Point3D::Dump(Report &report, int tabIndent)const
{
	//report.AddMarker(tabIndent);

	//report.Add(tabIndent, "Point3D\n");

	//define***

	report.Add(tabIndent, "Point3D(");
	report.Add(0, "%0.12f, %0.12f, %0.12f)\n", m_x, m_y, m_z);

	//define***

	//report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

bool Point3D::IsCollinear(const Point3D& point2, const Point3D& point3,
						  double collinearityTol)const
{
	Vector vector1(*this, point2);
	Vector vector2(*this, point3);

	if (vector1.GetMagnitude() < CommonConstants::ZERO || vector2.GetMagnitude() < CommonConstants::ZERO)
		return true;

	double angle;
	if(vector1.GetAngle(vector2, angle))
	{
		if(angle < collinearityTol)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

void Point3D::ApplyLengthUnit(double conversionFactor)
{
	m_x = m_x*conversionFactor;
	m_y = m_y*conversionFactor;
	m_z = m_z*conversionFactor;
}

//---------------------------------------------------------------------------

Point3D::operator Point3DF() const
{
	return Point3DF(m_x, m_y, m_z);
}

//---------------------------------------------------------------------------

}
