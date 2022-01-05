// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//geom include files
#include "CUTransform3D.h"
#include "CUSegment3D.h"
#include "CUPlane.h"

//util include files
#include "CUReport.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

void Transform3D::SetTranslationVal(double xVal, double yVal, double zVal)
{
	//create temporory Translation matrix
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(3, 0) = xVal;
	tempMatrix(3, 1) = yVal;
	tempMatrix(3, 2) = zVal;

	m_transformationMatrix.MultiplyTo(tempMatrix);
	
}
//-----------------------------------------------------------------------------

void Transform3D::SetScalingVal(double xVal, double yVal, double zVal)
{
	//create temporory Translation matrix
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(0, 0) = xVal;
	tempMatrix(1, 1) = yVal;
	tempMatrix(2, 2) = zVal;

	m_transformationMatrix.MultiplyTo(tempMatrix);

}

//-----------------------------------------------------------------------------

void Transform3D::SetRotationAboutAxisX(double angle)
{
	//creat temporory rotation matrix about the X
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(1, 1) = cos(angle);
	tempMatrix(2, 2) = cos(angle);
	tempMatrix(2, 1) = - sin(angle);
	tempMatrix(1, 2) = sin(angle);

	m_transformationMatrix.MultiplyTo(tempMatrix);

}


//-----------------------------------------------------------------------------

void Transform3D::SetRotationAboutAxisY(double angle)
{
	//creat temporory rotation matrix about the Y 
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(0, 0) = cos(angle);
	tempMatrix(2, 2) = cos(angle);
	tempMatrix(2, 0) = sin(angle);
	tempMatrix(0, 2) = - sin(angle);

	m_transformationMatrix.MultiplyTo(tempMatrix);

}


//-----------------------------------------------------------------------------

void Transform3D::SetRotationAboutAxisZ(double angle)
{
	//creat temporory rotation matrix about the Z
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(0, 0) = cos(angle);
	tempMatrix(1, 1) = cos(angle);
	tempMatrix(1, 0) = - sin(angle);
	tempMatrix(0, 1) = sin(angle);

	m_transformationMatrix.MultiplyTo(tempMatrix);

}

//-----------------------------------------------------------------------------

void Transform3D::SetTransformationAboutPoint(const Point3D &point)
{
	//creat temparory translation matrix
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);
	tempMatrix(2, 0) = -point.GetX();
	tempMatrix(2, 1) = -point.GetY();
	tempMatrix(2, 2) = -point.GetZ();

	//multiply the tempMatrix
	SqMatrix result(4, SqMatrix::IDENTITY);
	tempMatrix.Multiply(m_transformationMatrix, result);
	m_transformationMatrix = result;

	//reverse the translation operation
	SetTranslationVal(point.GetX(), point.GetY(), point.GetZ());

}

//-----------------------------------------------------------------------------

void Transform3D::SetRotationAboutArbitraryLine(double angle, const Segment3D &segment)
{
	Point3D point1, point2;
	point1 = segment.GetStartPoint();
	point2 = segment.GetEndPoint();
	Vector vec(point2, point1);
	vec.Normalize();
	double cX, cY, cZ;
	cX = vec.GetI();
	cY = vec.GetJ();
	cZ = vec.GetK();

	double d;
	d = sqrt(cY * cY + cZ * cZ);
	double angAtXaxis, angAtYaxis;
	if(fabs(d - 0) < 1e-6)
	{
		angAtXaxis = 0;//
		angAtYaxis = 1.57079632679489661923;
	}
	else
	{
		angAtXaxis = acos(cZ/d);
		angAtYaxis = acos(d);
	}

	SetTranslationVal( -point2.GetX(),  -point2.GetY(), -point2.GetZ());

	SetRotationAboutAxisX(angAtXaxis);

	SetRotationAboutAxisY(-angAtYaxis);

	SetRotationAboutAxisZ(angle);

	SetRotationAboutAxisY(angAtYaxis);
	
	SetRotationAboutAxisX(-angAtXaxis);

	SetTranslationVal( point2.GetX(),  point2.GetY(), point2.GetZ());

}


//-----------------------------------------------------------------------------

void Transform3D::SetShearingVal(double dirXshearY, double dirXshearZ,
								 double dirYshearX, double dirYshearZ,
								 double dirZshearX, double dirZshearY)
{
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(1, 0) = dirXshearY;
	tempMatrix(2, 0) = dirXshearZ;
	tempMatrix(0, 1) = dirYshearX;
	tempMatrix(2, 1) = dirYshearZ;
	tempMatrix(0, 2) = dirZshearX;
	tempMatrix(1, 2) = dirZshearY;

	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform3D::SetReflectionThroughXYplane()
{
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(2, 2) = -1;
	
	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform3D::SetReflectionThroughYZplane()
{
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(0, 0) = -1;
	
	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform3D::SetReflectionThroughZXplane()
{
	SqMatrix tempMatrix(4, SqMatrix::IDENTITY);

	tempMatrix(1, 1) = -1;
	
	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform3D::SetReflectionAtArbitraryPlane(const Plane &plane)
{
//SqMatrix m_transformationMatrix(4, SqMatrix::IDENTITY);
	m_transformationMatrix;
	for(int i = 0; i < m_transformationMatrix.GetNumRows(); i++)
	{
		for(int j = 0; j < m_transformationMatrix.GetNumCols(); j++)
		{
			m_transformationMatrix(i,j) = 0;			
		}
		m_transformationMatrix(i,i) = 1;
	}
	
	Point3D point = plane.GetBasePoint();
	Vector vector = plane.GetNormal();
	double cX, cY, cZ;
	cX = vector.GetI();
	cY = vector.GetJ();
	cZ = vector.GetK();
	double d;
	d = sqrt(cY * cY + cZ * cZ);
	double angAtXaxis, angAtYaxis;
	angAtXaxis = acos(cZ/d);
	angAtYaxis = acos(d);


	SetTranslationVal( point.GetX(),  point.GetY(), point.GetZ());
	SetRotationAboutAxisX(angAtXaxis);
	SetRotationAboutAxisY(angAtYaxis);
	SetReflectionThroughXYplane();


	angAtYaxis = 0 - angAtYaxis;
	SetRotationAboutAxisY(angAtYaxis);
	angAtXaxis = 0 - angAtXaxis;
	SetRotationAboutAxisX(angAtXaxis);
	SetTranslationVal( -point.GetX(),  -point.GetY(), -point.GetZ());
}

//-----------------------------------------------------------------------------

//void Transform3D::SetArbitraryPlaneToXYplane(const Plane &plane)
//{
//	m_transformationMatrix;
//	for(int i = 0; i < m_transformationMatrix.GetNumRows(); i++)
//	{
//		for(int j = 0; j < m_transformationMatrix.GetNumCols(); j++)
//		{
//			m_transformationMatrix(i,j) = 0;			
//		}
//		m_transformationMatrix(i,i) = 1;
//	}
//	Point3D point = plane.GetBasePoint();
//	Vector vector = plane.GetNormal();
//
//	double cX, cY, cZ;
//	cX = vector.GetI();
//	cY = vector.GetJ();
//	cZ = vector.GetK();
//
//	double d;
//	d = sqrt(cY * cY + cZ * cZ);
//
//	double angAtXaxis, angAtYaxis;
//	angAtXaxis = acos(cZ/d);
//	angAtYaxis = acos(d);
//
//	SetTranslationVal( -point.GetX(),  -point.GetY(), -point.GetZ());
//	SetRotationAboutAxisX(angAtXaxis);
//	SetRotationAboutAxisY(-angAtYaxis);
//}

//-----------------------------------------------------------------------------

//void Transform3D::SetArbitraryPlaneToXYplane2(const Plane &plane)
//{
//	//setting m_transformationMatrix identity matrix
//	for (int i = 0; i < m_transformationMatrix.GetNumRows(); i++)
//	{
//		for (int j = 0; j < m_transformationMatrix.GetNumCols(); j++)
//		{
//			if (i == j)
//				m_transformationMatrix(i, j) = 1.;
//			else
//				m_transformationMatrix(i, j) = 0.;
//		}
//	}
//
//	Point3D basePoint = plane.GetBasePoint();
//	Vector normal = plane.GetNormal();
//	normal.Normalize();
//
//	double cX, cY, cZ;
//	cX = normal.GetI();
//	cY = normal.GetJ();
//	cZ = normal.GetK();
//	double d;
//	d = sqrt(cY * cY + cZ * cZ);
//
//	SqMatrix temp1;
//	temp1 = m_transformationMatrix;
//	temp1(3, 0) = -basePoint.GetX();
//	temp1(3, 1) = -basePoint.GetY();
//	temp1(3, 2) = -basePoint.GetZ();
//
//	SqMatrix temp2;
//	temp2 = m_transformationMatrix;
//	if (d != 0)
//	{
//		temp2(1, 1) = cZ / d;
//		temp2(1, 2) = cY / d;
//		temp2(2, 1) = -cY / d;
//		temp2(2, 2) = cZ / d;
//	}
//	else
//	{
//		temp2(1, 1) = cZ;
//		temp2(1, 2) = cY;
//		temp2(2, 1) = -cY;
//		temp2(2, 2) = cZ;
//	}
//
//	SqMatrix temp3;
//	temp3 = m_transformationMatrix;
//	temp3(0, 0) = d;
//	temp3(0, 2) = cX;
//	temp3(2, 0) = -cX;
//	temp3(2, 2) = d;
//
//	SqMatrix multiplyMatrix;
//	temp1.Multiply(temp2, multiplyMatrix);
//	multiplyMatrix.Multiply(temp3, m_transformationMatrix);
//}

//-----------------------------------------------------------------------------

void Transform3D::InverseMatrix()
{
	m_transformationMatrix.GetInverse(m_transformationMatrix);
}

//-----------------------------------------------------------------------------
const SqMatrix& Transform3D::GetTransformationMatrix()const
{
	return m_transformationMatrix;
}

//-----------------------------------------------------------------------------

void Transform3D::Dump(Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);
	report.Add(tabIndent, "Matrix\n");
	m_transformationMatrix.Dump(report, tabIndent);
	report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

void Transform3D::SetXYplaneToArbitraryPlane(const Plane &plane)
{

	//setting m_transformationMatrix identity matrix
	for (int i = 0; i < m_transformationMatrix.GetNumRows(); i++)
	{
		for (int j = 0; j < m_transformationMatrix.GetNumCols(); j++)
		{
			if (i == j)
				m_transformationMatrix(i, j) = 1.;
			else
				m_transformationMatrix(i, j) = 0.;

		}
	}

	Point3D basePoint = plane.GetBasePoint();
	Vector normal = plane.GetNormal();
	normal.Normalize();

	double cX, cY, cZ;
	cX = normal.GetI();
	cY = normal.GetJ();
	cZ = normal.GetK();
	double d;
	d = sqrt(cY * cY + cZ * cZ);

	SqMatrix temp1;
	temp1 = m_transformationMatrix;
	temp1(3, 0) = -basePoint.GetX();
	temp1(3, 1) = -basePoint.GetY();
	temp1(3, 2) = -basePoint.GetZ();

	SqMatrix temp2;
	temp2 = m_transformationMatrix;
	temp2(1, 1) = cZ / d;
	temp2(1, 2) = cY / d;
	temp2(2, 1) = -cY / d;
	temp2(2, 2) = cZ / d;

	SqMatrix temp3;
	temp3 = m_transformationMatrix;
	temp3(0, 0) = d;
	temp3(0, 2) = cX;
	temp3(2, 0) = -cX;
	temp3(2, 2) = d;



	SqMatrix multiplyMatrix;
	temp1.Multiply(temp2, multiplyMatrix);
	multiplyMatrix.Multiply(temp3, m_transformationMatrix);

	SqMatrix transform3DMatrix;
	transform3DMatrix = m_transformationMatrix;
	SqMatrix inverseTransMatrix;

	transform3DMatrix.GetInverse(inverseTransMatrix);
	m_transformationMatrix = inverseTransMatrix;

}
//-----------------------------------------------------------------------------
void Transform3D::SetArbitraryPlaneToXYplane(const Plane &plane)
{
	//setting m_transformationMatrix to identity matrix
	for (size_t i = 0; i < m_transformationMatrix.GetNumRows(); i++)
	{
		for (size_t j = 0; j < m_transformationMatrix.GetNumRows(); j++)
		{
			if (i == j)
				m_transformationMatrix(i, j) = 1.;
			else
				m_transformationMatrix(i, j) = 0.;
		}
	}

	Point3D basePoint = plane.GetBasePoint();
	Vector normal = plane.GetNormal();
	
	if (!normal.Normalize())
		return;

	double cX = normal.GetI();
	double cY = normal.GetJ();
	double cZ = normal.GetK();

	// Set translation. This is required for normals parallel to coordinate axes
	m_transformationMatrix(3, 0) = -basePoint.GetX();
	m_transformationMatrix(3, 1) = -basePoint.GetY();
	m_transformationMatrix(3, 2) = -basePoint.GetZ();

	if (fabs(cX - 0.0) < CommonConstants::ZERO && fabs(cY - 0.0) < CommonConstants::ZERO)	// +/- Z axis
	{
		if (cZ > 0.0)	// +Z
		{
			// Do nothing
		}
		else			// -Z
		{
			// Rotate about X axis in 180 degrees
			SetRotationAboutAxisX(CommonConstants::PI);
		}
		return;
	}
	else if (fabs(cY - 0.0) < CommonConstants::ZERO && fabs(cZ - 0.0) < CommonConstants::ZERO)	// +/- X axis
	{
		if (cX > 0.0)	// +X
		{
			// Rotate about Y axis in 90 degrees
			SetRotationAboutAxisY(CommonConstants::PI / 2);
		}
		else			// -X
		{
			// Rotate about Y axis in -90 degrees
			SetRotationAboutAxisY(-CommonConstants::PI / 2);
		}
		return;
	}
	else if (fabs(cX - 0.0) < CommonConstants::ZERO && fabs(cZ - 0.0) < CommonConstants::ZERO)	// +/- Y axis
	{
		if (cY > 0.0)	// +Y
		{
			// Rotate about X axis in 90 degrees
			SetRotationAboutAxisX(CommonConstants::PI / 2);
		}
		else			// -Y
		{
			// Rotate about X axis in -90 degrees
			SetRotationAboutAxisX(-CommonConstants::PI / 2);
		}
		return;
	}

	double d = sqrt(cY * cY + cZ * cZ);
	if (d < CommonUtil::CommonConstants::ZERO)
		return;

	//rotation of z about y
	SqMatrix temp2(m_transformationMatrix);
	temp2(1, 1) = cZ / d;
	temp2(1, 2) = cY / d;
	temp2(2, 1) = -cY / d;
	temp2(2, 2) = cZ / d;
	
	//rotation of x about y
	SqMatrix temp3(m_transformationMatrix);
	temp3(0, 0) = d;
	temp3(0, 2) = cX;
	temp3(2, 0) = -cX;
	temp3(2, 2) = d;

	size_t numRows = m_transformationMatrix.GetNumRows();
	SqMatrix multiplyMatrix(numRows);

	m_transformationMatrix.Multiply(temp2, multiplyMatrix);
	multiplyMatrix.Multiply(temp3, m_transformationMatrix);
}
//-----------------------------------------------------------------------------

}