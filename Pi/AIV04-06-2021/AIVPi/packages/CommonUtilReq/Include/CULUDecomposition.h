// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_LUDECOMPOSITIOIN_H_
#define _COMMONUTIL_LUDECOMPOSITIOIN_H_

// standard include files
#include <vector>
#include<cmath>

// util include files
#include "CUMatrix.h"

namespace CommonUtil
{

// Forward declaration
class Matrix;

//This class implements the method for solving system of linear equations
class LUDecomposition
{
	//========================== private data members =========================

	Matrix m_coeffMatrix;
	Matrix m_rhsMatrix;
	bool m_useIntel = false;

	//======================= private members functions =======================

	// LD = B
	// L{[U]{X} - {D}} = [A]{X} - {B}
	// computes D and using D it computes unKnown Matrix X
	void backSubstitution(std::vector<int> &indices, Matrix &unknownMatrix);

	//void separateLowerUpper(std::vector<int> &indices);  ---> Method in STEPReader

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	// Constructor with coeff matrix(A) and rhs matrix(B)
	LUDecomposition(const Matrix &coeffMatrix, const Matrix &rhsMatrix);

	//****************************** destructor *******************************

	~LUDecomposition()
	{}

	//*************************** get/set methods *****************************

	void SetUseIntelFlag(bool useIntel)
	{
		m_useIntel = useIntel;
	}

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
