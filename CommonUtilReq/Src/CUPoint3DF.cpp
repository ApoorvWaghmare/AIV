// ============================================================================
//			Copyright (c) 2007 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//geom include files
#include "CUPoint3DF.h"
#include "CUVector.h"
#include "CUPoint2D.h"
//util include files
#include "CUReport.h"
#include "CUSqMatrix.h"
#include "CUTransform3D.h"
#include "CUPoint3D.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

Point3DF::Point3DF(const Point2D* point):m_x(0.),m_y(0.),m_z(0.)
{
	if(point)
	{
		m_x = point->GetX();
		m_y = point->GetY();
		m_z = 0.0;
	}
}

//-----------------------------------------------------------------------------

void Point3DF::Dump(Report &report, int tabIndent)const
{
	//report.AddMarker(tabIndent);
	
	//report.Add(tabIndent, "Point3D\n");
	report.Add(tabIndent, "Point3D(");
	report.Add(0, "%0.4f, %0.4f, %0.4f)\n", m_x, m_y, m_z);

	//report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

bool Point3DF::IsCollinear(const Point3DF& point2, const Point3DF& point3,
						  double collinearityTol)const
{
	Vector vector1(*(const CommonUtil::Point3D*)this, (const CommonUtil::Point3D&)point2);
	Vector vector2(*(const CommonUtil::Point3D*)this, (const CommonUtil::Point3D&)point3);

	if(vector1.GetMagnitude() < CommonConstants::ZERO || vector2.GetMagnitude() < CommonConstants::ZERO)
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

void Point3DF::ApplyLengthUnit(double conversionFactor)
{
	m_x = m_x*conversionFactor;
	m_y = m_y*conversionFactor;
	m_z = m_z*conversionFactor;
}
//-----------------------------------------------------------------------------

Point3DF::operator Point3D() 
{
	return Point3D( m_x, m_y, m_z);
}

//-----------------------------------------------------------------------------

bool Point3DF::IsEqual(const Point3DF &point, double tolerance)const
{
	bool stat = false;
	double sqrDist = SqrDistance(point);
	if (sqrDist < tolerance*tolerance)
		stat = true;
	return stat;
}

//---------------------------------------------------------------------------
void Point3DF::Transform(SqMatrix &Rotation , std::vector<double> &translation)
{
	double tempX = (double)GetX();
	double tempY = (double)GetY();
	double tempZ = (double)GetZ();

	double x =  Rotation(0,0) * tempX + Rotation(0,1) * tempY +
			Rotation(0,2) * tempZ + translation[0];
	
	double y =  Rotation(1,0) * tempX + Rotation(1,1) * tempY +
			Rotation(1,2) * tempZ + translation[1];
	
	double z =  Rotation(2,0) * tempX + Rotation(2,1) * tempY +
			Rotation(2,2) * tempZ + translation[2];
	
	Set(x, y, z);
}

//-----------------------------------------------------------------------------

void Point3DF::Transform(const Transform3D &transform)
{
	Matrix matrix(4, 1);
	matrix(0, 0) = m_x;
	matrix(0, 1) = m_y;
	matrix(0, 2) = m_z;
	matrix(0, 3) = 1;
	
	SqMatrix transformationMatrix = transform.GetTransformationMatrix();
	
	size_t rowSize = transformationMatrix.GetNumRows();
	Matrix tmpMatrix( rowSize, rowSize);

	for(int i = 0; i < rowSize; i++)
	{
		for(int j = 0; j < rowSize; j++)
		{
			tmpMatrix(i,j) = transformationMatrix(i,j);
		}
	}

	matrix.MultiplyTo(tmpMatrix);

	m_x = (float) matrix(0, 0);
	m_y = (float) matrix(0, 1);
	m_z = (float) matrix(0, 2);
}

//-----------------------------------------------------------------------------

void Point3DF::GetAvgPoint(const Point3DF* point, double &x, double &y, double &z)const
{
	double	x2 = (double)point->GetX();
	double	y2 = (double)point->GetY();
	double	z2 = (double)point->GetZ();

	x = ((double)m_x + x2) / 2;
	y = ((double)m_y + y2) / 2;
	z = ((double)m_z + z2) / 2;
}

//---------------------------------------------------------------------------

}