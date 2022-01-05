// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_SQMATRIX_H_
#define _COMMONUTIL_SQMATRIX_H_

// standard include files
#include<float.h>
//#include <vector>
#include<cmath>
#include<assert.h>

// math includes
#include "CUMatrix.h"
#include "CUVector.h"

namespace CommonUtil
{

class Vector;

// This class represents a square matrix
class SqMatrix : public Matrix
{

	//======================= private members functions =======================

	// Returns cofactor of the matrix
	void getCofactorMatrix( SqMatrix &cofactor ,int m, int n)const;

public :
	
	//======================== public member functions ========================

	enum MatrixType
	{
		EMPTYMATRIX,
		IDENTITY
	};

	//************************** list of constructors *************************

	// Default constructor
	SqMatrix()
	{
	}
	// Constructor which constructs empty or identity matrix
	// of given size
	SqMatrix(size_t colSize, MatrixType type = EMPTYMATRIX);
	
	// Constructor which sets the elements according to the input
	SqMatrix(int colSize, const std::vector<double> &values);

	// Copy constructor
	SqMatrix (const SqMatrix &matrix);

	//**************************general methods********************************
	
	// Equal to operator
	const SqMatrix & operator = (const SqMatrix &matrix);

	void operator - (SqMatrix &matrix);
	
	// Computes and returns the value of the determinent
	double ComputeDeterminent()const;

	//returns maximum eigen vector
	void GetMaxEigenVector(std::vector <double> &eigenVector);

	//returns the minimum eigen vector
	void GetMinEigenVector(std::vector <double> &eigenVector);

	//returns eigen vector corresponding to minimum eigen value
	void GetEigenVectorForMinEigenValue(std::vector <double> &minEigenVector, double &minEigenVal, double &cost, double* eigenValueArray = nullptr)const;

	void GetEigenValues(std::vector<double> &eigenValues)const;

	void GetEigenInfo(double eigenValues[3], Vector eigenVectors[3])const;

	//Check whetere matrix is symmetric or not
	bool IsSymmetric()const;

	void GetInverse(SqMatrix &inverseMatrix)const;

};

}

#endif
