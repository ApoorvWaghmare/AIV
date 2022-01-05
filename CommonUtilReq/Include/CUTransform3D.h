// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_TRANSFORM3D_H_
#define _COMMONUTIL_TRANSFORM3D_H_

//math include files
#include "CUSqMatrix.h"

namespace CommonUtil
{

//forward declaration
class Segment3D;
class Plane;
class Point3D;

//this class represents a transformaton matrix for given transformation
class Transform3D
{
	//========================== private data members =========================

	SqMatrix m_transformationMatrix;

public :


	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Transform3D():m_transformationMatrix(4, SqMatrix::IDENTITY)
	{}

	//****************************** destructor *******************************

	// Destructor
	~Transform3D()
	{}

	//*************************** get/set methods *****************************

	//create the translation matrix
	void SetTranslationVal(double xVal, double yVal, double zVal);

	//create the scaling matrix
	void SetScalingVal(double xVal, double yVal, double zVal);

	//create the roatation matrix about the X axis.
	void SetRotationAboutAxisX(double angle);

	//create the roatation matrix about the Y axis.
	void SetRotationAboutAxisY(double angle);

	//create the roatation matrix about the Z axis.
	void SetRotationAboutAxisZ(double angle);

	//create the transformation matrix for roatation about the arbitrary line
	void SetRotationAboutArbitraryLine(double angle, const Segment3D &segment);

	//create the transformation matrix about the point
	void SetTransformationAboutPoint(const Point3D &point);

	//create the shearing val matrix
	void SetShearingVal(double dirXshearY, double dirXshearZ, double dirYshearX,
						double dirYshearZ, double dirZshearX, double dirZshearY);

	//create the  reflection matrix for the XY plane.
	void SetReflectionThroughXYplane();

	//create the reflction matrix for the YZ plane
	void SetReflectionThroughYZplane();

	//create the reflection matrix for the ZX plane.
	void SetReflectionThroughZXplane();

	//create the reflection matrix for the arbitrary Plane
	void SetReflectionAtArbitraryPlane(const Plane &plane);

	//void SetArbitraryPlaneToXYplane(const Plane &plane);

	//void SetArbitraryPlaneToXYplane2(const Plane &plane);
	void SetArbitraryPlaneToXYplane(const Plane &plane);

	//set transformation matrix
	void SetTransformationMatrix(const SqMatrix &transMatrix)
	{
		m_transformationMatrix = transMatrix;
	}
	
	//inverse own transformation matrix
	void InverseMatrix();

	//get the transformatiom matrix
	const SqMatrix& GetTransformationMatrix()const;

	//**************************general methods********************************

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	void SetXYplaneToArbitraryPlane(const Plane &plane);

};

}

#endif
