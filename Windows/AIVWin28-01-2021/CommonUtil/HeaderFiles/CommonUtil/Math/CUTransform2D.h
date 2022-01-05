// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_TRANSFORM2D_H_
#define _COMMONUTIL_TRANSFORM2D_H_

//geom include files
#include "CUPoint2D.h"

//math include files
#include "CUSqMatrix.h"

namespace CommonUtil
{
//forward declaration
class Segment2D;

//this class represents a transformation matrix for given transformation
class Transform2D
{
	//========================== private data members =========================

	SqMatrix m_transformationMatrix;

public:

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Transform2D() : m_transformationMatrix(3, SqMatrix::IDENTITY)
	{}

	//****************************** destructor *******************************

	// Destructor
	~Transform2D()
	{}
	

	//*************************** get/set methods *****************************

	//set the value of angle given by user
	void SetRotationAngle(double angle);

	//create the translation matrix
	void SetTranslationVal(double xVal, double yVal);

	//create the scaling matrix
	void SetScalingVal(double sX, double sY);
	
	//create the shearing matrix 
	void SetShearingVal(double srX, double srY);
	
	//create the reflection matrix
	void SetReflectionThroughLine(const Segment2D &segment);

	//create the reflection matrix about the X axis
	void SetReflectionAboutX();

	//create the reflection matrix about the Y axis
	void SetReflectionAboutY();

	//create the reflection matrix about the Z axis
	void SettransformationAboutPoint(const Point2D &point);

	//get the square matrix created.
	const SqMatrix& GetTransformationMatrix()const;

	//set transformation matrix
	void SetTransformationMatrix(const SqMatrix &transMatrix)
	{
		m_transformationMatrix = transMatrix;
	}

	//**************************general methods********************************

	//dump the data.
	void Dump(Report &report, int tabIndent)const;

};

}

#endif