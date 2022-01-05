// ============================================================================
//			Copyright (c) 2008 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_SKETCHER_LUDecomposition_ALGO_H
#define _COMMONUTIL_SKETCHER_LUDecomposition_ALGO_H

#include <vector>

#include "CUVector.h"
#include "CUMatrix.h"

//#include "Sketcher2D.h"
//#include "Sketcher2DConstraint.h"
//#include "Sketcher2DEntity.h"
//#include "SketcherConstants.h"

namespace CommonUtil
{

class Constant;

class Sketcher_LUDecomposition
{
		//========================== private data members =========================

	Matrix m_coeffMatrix;
	Matrix m_rhsMatrix;

	//======================= private members functions =======================

	// LD = B
	// L{[U]{X} - {D}} = [A]{X} - {B}
	// computes D and using D it computes unKnown Matrix X
	void backSubstitution(const std::vector<int> &indices, Matrix &unknownMatrix);

	//void separateLowerUpper(std::vector<int> &indices);  ---> Method in STEPReader

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	// Constructor with coeff matrix(A) and rhs matrix(B)
	Sketcher_LUDecomposition(const Matrix &coeffMatrix, const Matrix &rhsMatrix);

	//****************************** destructor *******************************

	~Sketcher_LUDecomposition()
	{}

	//*************************** get/set methods *****************************

	// Computes the inverse of the coeff. matrix(A)
	bool GetInverse(Matrix &inverseMatrix);

	// Gets coefficient matrix
	void GetCoeffMatrix(Matrix &changedLUCoeffMatrix)const
	{
		changedLUCoeffMatrix = m_coeffMatrix;		
	}

	//**************************general methods********************************

	// Decomoses the coefficient Matrix (A) into lower(L) and upper(U)
	//triangular Matrix
	bool SeparateLowerUpper(std::vector<int> &indices, double &d);

	// Computes the unknown matrix(X), AX = B
	bool Execute(Matrix &unknownMatrix);
	
};
}
#endif



