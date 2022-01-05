// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//geom include files
#include "CUPoint2D.h"
#include "CUPoint3D.h"
#include "CUTransform2D.h"

//math include files
#include "CUSqMatrix.h"

//util include files
#include "CUReport.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

void Point2D::Set(double x, double y)
{
	m_x = x;
	m_y = y;
}

//-----------------------------------------------------------------------------

Point2D::operator Point3D()const
{
	double z = 0.0;
	return Point3D(m_x, m_y, z);
}

//-----------------------------------------------------------------------------

bool Point2D::IsEqual(const Point2D &point, double tolerance)const
{
	bool stat = false;
	double sqrDist = SqrDistance(point);
	if (sqrDist < tolerance*tolerance)
		stat = true;
	return stat;
}

//-----------------------------------------------------------------------------

void Point2D::Transform(const Transform2D &transform)
{
	//Old code
/*	Matrix matrix(3, 1);
	matrix(0, 0) = m_x;
	matrix(1, 0) = m_y;
	matrix(2, 0) = 1;*/

	//Modified code
	Matrix matrix(3, 1);
	matrix(0, 0) = m_x;
	matrix(0, 1) = m_y;
	matrix(0, 2) = 1;


	const SqMatrix& transformationMatrix = transform.GetTransformationMatrix();

	matrix.MultiplyTo(transformationMatrix);
	//transformationMatrix.MultiplyTo(matrix);

	//Old code
//	m_x = matrix(0, 0);
//	m_y = matrix(1, 0);

	//Modified code
	m_x = matrix(0, 0);
	m_y = matrix(0, 1);
}

//-----------------------------------------------------------------------------

void Point2D::Dump(Report &report, int tabIndent)const
{
	/*report.AddMarker(tabIndent);

	report.Add(tabIndent, "Point2D\n");
	report.Add(tabIndent, "m_x, m_y : ");
	report.Add(0, "%15.6f, %15.6f\n", m_x, m_y);

	report.AddMarker(tabIndent);*/
}

//-----------------------------------------------------------------------------
// method for Sketcher... SDP
bool Point2D::IsSame(const Point2D &point)const
{
	bool same = false;
	if(this->Distance(point) < 1e-3)
		same = true;

	return same;
}

//---------------------------------------------------------------------------------------

}
