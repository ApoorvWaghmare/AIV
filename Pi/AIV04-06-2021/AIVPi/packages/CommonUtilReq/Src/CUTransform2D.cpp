// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include<vector>
#include<cmath>

//geom include files
#include "CUTransform2D.h"
#include "CUSegment2D.h"

//util include files
#include "CUReport.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

void Transform2D::SetRotationAngle(double angle)
{
	//create temporory rotation matrix
	SqMatrix tempMatrix(3,SqMatrix::IDENTITY);

	tempMatrix(0, 0) = cos(angle);
	tempMatrix(1, 1) = cos(angle);
	tempMatrix(1, 0) = - sin(angle);
	tempMatrix(0, 1) = sin(angle);

	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform2D::SetTranslationVal(double xVal, double yVal)
{
	//creat temparory translation matrix
	SqMatrix tempMatrix(3, SqMatrix::IDENTITY);

	tempMatrix(2, 0) = xVal;
	tempMatrix(2, 1) = yVal;

	m_transformationMatrix.MultiplyTo(tempMatrix);

}

//-----------------------------------------------------------------------------

void Transform2D::SetScalingVal(double sX, double sY)
{

	//creat temparory scaling  matrix
	SqMatrix tempMatrix(3, SqMatrix::IDENTITY);

	tempMatrix(0, 0) = sX;
	tempMatrix(1, 1) = sY;


	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform2D::SetReflectionAboutX()
{
	//creat temparory reflection matrix
	SqMatrix tempMatrix(3, SqMatrix::IDENTITY);

	tempMatrix(1, 1) = -1;

	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform2D::SetReflectionAboutY()
{
	//creat temparory reflection matrix
	SqMatrix tempMatrix(3, SqMatrix::IDENTITY);

	tempMatrix(0, 0) = -1;

	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform2D::SetShearingVal(double srX, double srY)
{

	//creat temparory shearing matrix
	SqMatrix tempMatrix(3, SqMatrix::IDENTITY);

	tempMatrix(0, 1) = srX;
	tempMatrix(1, 0) = srY;

	m_transformationMatrix.MultiplyTo(tempMatrix);
}

//-----------------------------------------------------------------------------

void Transform2D::SetReflectionThroughLine(const Segment2D &segment)
{
	//get y intersept when the x = 0
	//create temporory reflection matrix
	m_transformationMatrix;
	for(int i = 0; i < m_transformationMatrix.GetNumRows(); i++)
	{
		for(int j = 0; j < m_transformationMatrix.GetNumCols(); j++)
		{
			m_transformationMatrix(i,j) = 0;
		}
		m_transformationMatrix(i,i) = 1;
	}

	//create the translation matrix and update m_transformation matrix
	bool parallelToYAxis = false;
	double yIntecept = segment.GetYIntercept(parallelToYAxis);
	double yVal = 0 - yIntecept;
	SetTranslationVal(0, yVal);

	//create the rotation matrixand update m_transformation matrix
	bool stat1;
	double angle = atan(segment.GetSlope(stat1));
	double angle1 = 0 - angle;
	SetRotationAngle(angle1);

	//reflection about the X axis
	SetReflectionAboutX();

	//create the reverse rotation matrix
	SetRotationAngle(angle);

	//create the reverse translation matrix
	SetTranslationVal(0, yIntecept);

}

//-----------------------------------------------------------------------------

//gives the transformation about the point
void Transform2D::SettransformationAboutPoint(const Point2D &point)
{
	//creat temparory translation matrix
	SqMatrix tempMatrix(3, SqMatrix::IDENTITY);
	tempMatrix(2, 0) = -point.GetX();
	tempMatrix(2, 1) = -point.GetY();

	//multiply the tempMatrix
	SqMatrix result(3, SqMatrix::IDENTITY);
	tempMatrix.Multiply(m_transformationMatrix, result);
	m_transformationMatrix = result;

	//reverse the translation operation
	SetTranslationVal(point.GetX(), point.GetY());

}

//-----------------------------------------------------------------------------

//return the created matrix
const SqMatrix& Transform2D::GetTransformationMatrix()const
{
	return m_transformationMatrix;
}

//-----------------------------------------------------------------------------

//dump the data
void Transform2D::Dump(Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);

	report.Add(tabIndent, "Matrix\n");
	m_transformationMatrix.Dump(report, tabIndent);
	report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

}

